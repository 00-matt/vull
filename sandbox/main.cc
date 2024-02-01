#include "free_camera.hh"

#include <vull/container/array.hh>
#include <vull/container/hash_map.hh>
#include <vull/container/vector.hh>
#include <vull/core/input.hh>
#include <vull/core/log.hh>
#include <vull/core/main.hh>
#include <vull/core/window.hh>
#include <vull/ecs/world.hh>
#include <vull/graphics/default_renderer.hh>
#include <vull/graphics/deferred_renderer.hh>
#include <vull/graphics/frame_pacer.hh>
#include <vull/graphics/gbuffer.hh>
#include <vull/graphics/skybox_renderer.hh>
#include <vull/maths/colour.hh>
#include <vull/maths/common.hh>
#include <vull/maths/random.hh>
#include <vull/maths/vec.hh>
#include <vull/physics/collider.hh>
#include <vull/physics/physics_engine.hh>
#include <vull/physics/rigid_body.hh>
#include <vull/platform/timer.hh>
#include <vull/scene/scene.hh>
#include <vull/support/algorithm.hh>
#include <vull/support/assert.hh>
#include <vull/support/result.hh>
#include <vull/support/span.hh>
#include <vull/support/string_builder.hh>
#include <vull/support/string_view.hh>
#include <vull/support/unique_ptr.hh>
#include <vull/support/utility.hh>
#include <vull/ui/element.hh>
#include <vull/ui/font.hh>
#include <vull/ui/font_atlas.hh>
#include <vull/ui/layout/pane.hh>
#include <vull/ui/layout/screen_pane.hh>
#include <vull/ui/painter.hh>
#include <vull/ui/renderer.hh>
#include <vull/ui/style.hh>
#include <vull/ui/tree.hh>
#include <vull/ui/widget/button.hh>
#include <vull/ui/widget/label.hh>
#include <vull/ui/widget/slider.hh>
#include <vull/ui/widget/time_graph.hh>
#include <vull/ui/window.hh>
#include <vull/vpak/file_system.hh>
#include <vull/vpak/reader.hh>
#include <vull/vulkan/command_buffer.hh>
#include <vull/vulkan/context.hh>
#include <vull/vulkan/fence.hh>
#include <vull/vulkan/query_pool.hh>
#include <vull/vulkan/queue.hh>
#include <vull/vulkan/render_graph.hh>
#include <vull/vulkan/semaphore.hh>
#include <vull/vulkan/swapchain.hh>
#include <vull/vulkan/vulkan.hh>

#include <stdint.h>

using namespace vull;

void vull_main(Vector<StringView> &&args) {
    if (args.size() < 2) {
        vull::println("usage: {} [--enable-vvl] <scene-name>", args[0]);
        return;
    }

    StringView scene_name;
    bool enable_validation = false;
    for (const auto &arg : vull::slice(args, 1u)) {
        if (arg == "--enable-vvl") {
            enable_validation = true;
        } else if (arg[0] == '-') {
            vull::println("fatal: unknown option {}", arg);
            return;
        } else if (scene_name.empty()) {
            scene_name = arg;
        } else {
            vull::println("fatal: unexpected argument {}", arg);
            return;
        }
    }

    Window window({}, {}, true);
    vk::Context context(enable_validation);
    auto swapchain = window.create_swapchain(context, vk::SwapchainMode::LowPower);

    Scene scene;
    scene.load(scene_name);

    DeferredRenderer deferred_renderer(context, swapchain.extent_3D());
    DefaultRenderer default_renderer(context, swapchain.extent_3D());
    default_renderer.load_scene(scene);

    SkyboxRenderer skybox_renderer(context);
    if (auto stream = vpak::open("/skybox")) {
        skybox_renderer.load(*stream);
    }

    auto main_font = VULL_EXPECT(ui::Font::load("/fonts/Inter-Medium", 18));
    auto monospace_font = VULL_EXPECT(ui::Font::load("/fonts/RobotoMono-Regular", 18));
    ui::Style ui_style(vull::move(main_font), vull::move(monospace_font));
    ui::Tree ui_tree(ui_style, window.ppcm());
    ui::Renderer ui_renderer(context);
    ui::FontAtlas atlas(context, Vec2u(512, 512));

    auto &world = scene.world();
    world.register_component<RigidBody>();
    world.register_component<Collider>();

    FreeCamera free_camera(window.aspect_ratio());
    free_camera.set_position(50.0f);
    free_camera.set_pitch(-0.2f);
    free_camera.set_yaw(-2.0f);
    free_camera.handle_mouse_move({});

    bool mouse_visible = false;
    window.on_mouse_release(MouseButton::Middle, [&](Vec2f) {
        mouse_visible = !mouse_visible;
        mouse_visible ? window.show_cursor() : window.hide_cursor();
    });

    // TODO: Delta and position shouldn't be floats.
    window.on_mouse_move([&](Vec2f delta, Vec2f position, MouseButtonMask buttons) {
        if (!window.cursor_hidden()) {
            ui_tree.handle_mouse_move(delta, position, buttons);
            return;
        }
        free_camera.handle_mouse_move(delta);
    });
    window.on_mouse_press(MouseButton::Left, [&](Vec2f) {
        if (!window.cursor_hidden()) {
            ui_tree.handle_mouse_press(MouseButton::Left);
        }
    });
    window.on_mouse_release(MouseButton::Left, [&](Vec2f) {
        if (!window.cursor_hidden()) {
            ui_tree.handle_mouse_release(MouseButton::Left);
        }
    });

    FramePacer frame_pacer(swapchain, 2);
    PhysicsEngine physics_engine;
    vull::seed_rand(5);

    auto &screen_pane = ui_tree.set_root<ui::ScreenPane>();
    auto &main_window = screen_pane.add_child<ui::Window>("Main");
    auto &cpu_time_graph =
        main_window.content_pane().add_child<ui::TimeGraph>(Colour::from_rgb(0.4f, 0.6f, 0.5f), "CPU time");
    auto &gpu_time_graph =
        main_window.content_pane().add_child<ui::TimeGraph>(Colour::from_rgb(0.8f, 0.5f, 0.7f), "GPU time");
    auto &quit_button = main_window.content_pane().add_child<ui::Button>("Quit");
    quit_button.set_on_release([&] {
        window.close();
    });

    auto &pipeline_statistics_window = screen_pane.add_child<ui::Window>("Pipeline statistics");
    Vector<ui::Label &> pipeline_statistics_labels;
    for (uint32_t i = 0; i < 5; i++) {
        auto &label = pipeline_statistics_window.content_pane().add_child<ui::Label>();
        label.set_align(ui::Align::Right);
        label.set_font(ui_style.monospace_font());
        pipeline_statistics_labels.push(label);
    }

    auto &camera_window = screen_pane.add_child<ui::Window>("Camera settings");
    camera_window.content_pane().add_child<ui::Label>("Exposure");
    auto &exposure_slider = camera_window.content_pane().add_child<ui::Slider>(0.0f, 20.0f);
    camera_window.content_pane().add_child<ui::Label>("FOV");
    auto &fov_slider = camera_window.content_pane().add_child<ui::Slider>(0.0f, 180.0f);
    exposure_slider.set_value(5.0f);
    fov_slider.set_value(90.0f);

    vk::QueryPool pipeline_statistics_pool(context, frame_pacer.queue_length(),
                                           vkb::QueryPipelineStatisticFlags::InputAssemblyVertices |
                                               vkb::QueryPipelineStatisticFlags::InputAssemblyPrimitives |
                                               vkb::QueryPipelineStatisticFlags::VertexShaderInvocations |
                                               vkb::QueryPipelineStatisticFlags::FragmentShaderInvocations |
                                               vkb::QueryPipelineStatisticFlags::ComputeShaderInvocations);

    Timer frame_timer;
    cpu_time_graph.new_bar();
    while (!window.should_close()) {
        Timer acquire_frame_timer;
        auto &frame = frame_pacer.request_frame();
        cpu_time_graph.push_section("acquire-frame", acquire_frame_timer.elapsed());

        float dt = frame_timer.elapsed();
        frame_timer.reset();

        // Poll input.
        window.poll_events();

        // Collect previous frame N's timestamp data.
        const auto pass_times = frame.pass_times();
        gpu_time_graph.new_bar();
        for (const auto &[name, time] : pass_times) {
            if (name != "submit") {
                gpu_time_graph.push_section(name, time);
            }
        }

        // Collect pipeline statistics.
        constexpr Array pipeline_statistics_strings{
            "Assembled vertices: {d8 }", "Assembled primitives: {d8 }", "VS invocations: {d8 }",
            "FS invocations: {d8 }",     "CS invocations: {d8 }",
        };
        Array<uint64_t, 5> pipeline_statistics{};
        pipeline_statistics_pool.read_host(pipeline_statistics.span(), 1, frame_pacer.frame_index());
        for (uint32_t i = 0; i < pipeline_statistics.size(); i++) {
            static_cast<ui::Label &>(pipeline_statistics_labels[i])
                .set_text(vull::format(pipeline_statistics_strings[i], pipeline_statistics[i]));
        }

        // Step physics.
        Timer physics_timer;
        physics_engine.step(world, dt);
        cpu_time_graph.push_section("step-physics", physics_timer.elapsed());

        // Update camera.
        free_camera.update(window, dt);

        Timer ui_timer;
        ui::Painter ui_painter;
        ui_painter.bind_atlas(atlas);
        ui_tree.render(ui_painter);
        cpu_time_graph.new_bar();
        cpu_time_graph.push_section("render-ui", ui_timer.elapsed());

        deferred_renderer.set_exposure(exposure_slider.value());
        default_renderer.set_cull_view_locked(window.is_key_pressed(Key::H));
        default_renderer.set_camera(free_camera);
        free_camera.set_fov(fov_slider.value() * (vull::pi<float> / 180.0f));

        Timer build_rg_timer;
        auto &graph = frame.new_graph(context);
        auto output_id = graph.import("output-image", swapchain.image(frame_pacer.image_index()));

        auto gbuffer = deferred_renderer.create_gbuffer(graph);
        auto frame_ubo = default_renderer.build_pass(graph, gbuffer);
        deferred_renderer.build_pass(graph, gbuffer, frame_ubo, output_id);
        skybox_renderer.build_pass(graph, gbuffer.depth, frame_ubo, output_id);
        ui_renderer.build_pass(graph, output_id, vull::move(ui_painter));

        graph.add_pass("submit", vk::PassFlags::None).read(output_id, vk::ReadFlags::Present);
        cpu_time_graph.push_section("build-rg", build_rg_timer.elapsed());

        Timer compile_rg_timer;
        graph.compile(output_id);
        cpu_time_graph.push_section("compile-rg", compile_rg_timer.elapsed());

        Timer execute_rg_timer;
        auto queue = context.lock_queue(vk::QueueKind::Graphics);
        auto &cmd_buf = queue->request_cmd_buf();
        cmd_buf.reset_query(pipeline_statistics_pool, frame_pacer.frame_index());
        cmd_buf.begin_query(pipeline_statistics_pool, frame_pacer.frame_index());
        graph.execute(cmd_buf, true);
        cmd_buf.end_query(pipeline_statistics_pool, frame_pacer.frame_index());

        Array signal_semaphores{
            vkb::SemaphoreSubmitInfo{
                .sType = vkb::StructureType::SemaphoreSubmitInfo,
                .semaphore = *frame.present_semaphore(),
            },
        };
        Array wait_semaphores{
            vkb::SemaphoreSubmitInfo{
                .sType = vkb::StructureType::SemaphoreSubmitInfo,
                .semaphore = *frame.acquire_semaphore(),
                .stageMask = vkb::PipelineStage2::ColorAttachmentOutput,
            },
        };
        queue->submit(cmd_buf, *frame.fence(), signal_semaphores.span(), wait_semaphores.span());
        cpu_time_graph.push_section("execute-rg", execute_rg_timer.elapsed());
    }
    context.vkDeviceWaitIdle();
}
