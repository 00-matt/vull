#include <vull/ui/Renderer.hh>

#include <vull/container/Array.hh>
#include <vull/maths/Vec.hh>
#include <vull/support/Assert.hh>
#include <vull/support/Result.hh>
#include <vull/support/Utility.hh>
#include <vull/ui/Painter.hh>
#include <vull/vulkan/CommandBuffer.hh>
#include <vull/vulkan/Context.hh>
#include <vull/vulkan/Image.hh>
#include <vull/vulkan/MemoryUsage.hh>
#include <vull/vulkan/Pipeline.hh>
#include <vull/vulkan/PipelineBuilder.hh>
#include <vull/vulkan/Queue.hh>
#include <vull/vulkan/RenderGraph.hh>
#include <vull/vulkan/Sampler.hh>
#include <vull/vulkan/Shader.hh>
#include <vull/vulkan/Vulkan.hh>

#include <stdint.h>

namespace vull::ui {

Renderer::Renderer(vk::Context &context) : m_context(context) {
    Array set_bindings{
        vkb::DescriptorSetLayoutBinding{
            .binding = 0,
            .descriptorType = vkb::DescriptorType::CombinedImageSampler,
            .descriptorCount = 128,
            .stageFlags = vkb::ShaderStage::Fragment,
        },
    };
    Array set_binding_flags{
        vkb::DescriptorBindingFlags::PartiallyBound | vkb::DescriptorBindingFlags::VariableDescriptorCount,
    };
    vkb::DescriptorSetLayoutBindingFlagsCreateInfo set_binding_flags_ci{
        .sType = vkb::StructureType::DescriptorSetLayoutBindingFlagsCreateInfo,
        .bindingCount = set_bindings.size(),
        .pBindingFlags = set_binding_flags.data(),
    };
    vkb::DescriptorSetLayoutCreateInfo set_layout_ci{
        .sType = vkb::StructureType::DescriptorSetLayoutCreateInfo,
        .pNext = &set_binding_flags_ci,
        .flags = vkb::DescriptorSetLayoutCreateFlags::DescriptorBufferEXT,
        .bindingCount = set_bindings.size(),
        .pBindings = set_bindings.data(),
    };
    VULL_ENSURE(context.vkCreateDescriptorSetLayout(&set_layout_ci, &m_descriptor_set_layout) == vkb::Result::Success);

    vkb::PushConstantRange push_constant_range{
        .stageFlags = vkb::ShaderStage::Vertex | vkb::ShaderStage::Fragment,
        .size = sizeof(Vec2f) + sizeof(uint32_t),
    };
    vkb::PipelineColorBlendAttachmentState blend_state{
        .blendEnable = true,
        .srcColorBlendFactor = vkb::BlendFactor::SrcAlpha,
        .dstColorBlendFactor = vkb::BlendFactor::OneMinusSrcAlpha,
        .colorBlendOp = vkb::BlendOp::Add,
        .srcAlphaBlendFactor = vkb::BlendFactor::One,
        .dstAlphaBlendFactor = vkb::BlendFactor::Zero,
        .alphaBlendOp = vkb::BlendOp::Add,
        .colorWriteMask =
            vkb::ColorComponent::R | vkb::ColorComponent::G | vkb::ColorComponent::B | vkb::ColorComponent::A,
    };
    auto vertex_shader = VULL_EXPECT(vk::Shader::load(m_context, "/shaders/ui.vert"));
    auto fragment_shader = VULL_EXPECT(vk::Shader::load(m_context, "/shaders/ui.frag"));
    m_pipeline = VULL_EXPECT(vk::PipelineBuilder()
                                 // TODO(swapchain-format): Don't hardcode format.
                                 .add_colour_attachment(vkb::Format::B8G8R8A8Srgb, blend_state)
                                 .add_set_layout(m_descriptor_set_layout)
                                 .add_shader(vertex_shader)
                                 .add_shader(fragment_shader)
                                 .set_push_constant_range(push_constant_range)
                                 .set_topology(vkb::PrimitiveTopology::TriangleList)
                                 .build(m_context));

    vkb::ImageCreateInfo image_ci{
        .sType = vkb::StructureType::ImageCreateInfo,
        .imageType = vkb::ImageType::_2D,
        .format = vkb::Format::R8Unorm,
        .extent = {1, 1, 1},
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = vkb::SampleCount::_1,
        .tiling = vkb::ImageTiling::Optimal,
        .usage = vkb::ImageUsage::Sampled,
        .sharingMode = vkb::SharingMode::Exclusive,
        .initialLayout = vkb::ImageLayout::Undefined,
    };
    m_null_image = context.create_image(image_ci, vk::MemoryUsage::DeviceOnly);

    auto queue = context.lock_queue(vk::QueueKind::Graphics);
    queue->immediate_submit([this](vk::CommandBuffer &cmd_buf) {
        cmd_buf.image_barrier({
            .sType = vkb::StructureType::ImageMemoryBarrier2,
            .dstStageMask = vkb::PipelineStage2::AllGraphics,
            .dstAccessMask = vkb::Access2::ShaderSampledRead,
            .oldLayout = vkb::ImageLayout::Undefined,
            .newLayout = vkb::ImageLayout::ReadOnlyOptimal,
            .image = *m_null_image,
            .subresourceRange = m_null_image.full_view().range(),
        });
    });
}

Renderer::~Renderer() {
    m_context.vkDestroyDescriptorSetLayout(m_descriptor_set_layout);
}

void Renderer::build_pass(vk::RenderGraph &graph, vk::ResourceId &target, Painter &&painter) {
    auto &pass = graph.add_pass("ui-pass", vk::PassFlags::Graphics).write(target, vk::WriteFlags::Additive);
    pass.set_on_execute([=, this, &graph, painter = vull::move(painter)](vk::CommandBuffer &cmd_buf) mutable {
        const auto output_extent = graph.get_image(target).extent();
        cmd_buf.bind_pipeline(m_pipeline);
        painter.compile(m_context, cmd_buf, Vec2f(output_extent.width, output_extent.height),
                        m_null_image
                            .swizzle_view({
                                .r = vkb::ComponentSwizzle::One,
                                .g = vkb::ComponentSwizzle::One,
                                .b = vkb::ComponentSwizzle::One,
                                .a = vkb::ComponentSwizzle::One,
                            })
                            .sampled(vk::Sampler::Nearest));
    });
}

} // namespace vull::ui
