#include "GltfParser.hh"

#include "FloatImage.hh"
#include "MadInst.hh"
#include "PngStream.hh"

#include <vull/container/HashMap.hh>
#include <vull/container/PerfectMap.hh>
#include <vull/container/Vector.hh>
#include <vull/core/BoundingBox.hh>
#include <vull/core/BoundingSphere.hh>
#include <vull/core/Log.hh>
#include <vull/core/Transform.hh>
#include <vull/ecs/World.hh>
#include <vull/graphics/Material.hh>
#include <vull/graphics/Mesh.hh>
#include <vull/graphics/Vertex.hh>
#include <vull/json/Parser.hh>
#include <vull/json/Tree.hh>
#include <vull/maths/Vec.hh>
#include <vull/platform/SystemLatch.hh>
#include <vull/platform/Timer.hh>
#include <vull/support/Assert.hh>
#include <vull/support/Atomic.hh>
#include <vull/support/Format.hh>
#include <vull/support/ScopedLock.hh>
#include <vull/support/SpanStream.hh>
#include <vull/tasklet/Latch.hh>
#include <vull/tasklet/Mutex.hh>
#include <vull/tasklet/Scheduler.hh>
#include <vull/tasklet/Tasklet.hh>
#include <vull/vpak/PackFile.hh>
#include <vull/vpak/Writer.hh>

#include <float.h>
#include <meshoptimizer.h>

// TODO(json): Lots of casting to uint64_t from a get<int64_t>().
// TODO(json): Range-based for loops on arrays.

namespace vull {
namespace {

enum class TextureType {
    Albedo,
    Normal,
};

class Converter {
    Span<uint8_t> m_binary_blob;
    vpak::Writer &m_pack_writer;
    json::Value &m_document;
    const bool m_max_resolution;

    HashMap<uint64_t, String> m_albedo_paths;
    HashMap<uint64_t, String> m_normal_paths;
    Mutex m_material_map_mutex;

    struct MeshBounds {
        BoundingBox box;
        BoundingSphere sphere;
    };
    HashMap<String, MeshBounds> m_mesh_bounds;
    Mutex m_mesh_bounds_mutex;

public:
    Converter(Span<uint8_t> binary_blob, vpak::Writer &pack_writer, json::Value &document, bool max_resolution)
        : m_binary_blob(binary_blob), m_pack_writer(pack_writer), m_document(document),
          m_max_resolution(max_resolution) {}

    Result<Tuple<uint64_t, uint64_t>, GltfParser::Error, json::TreeError> get_blob_info(const json::Object &accessor);
    Result<uint64_t, GltfParser::Error, json::TreeError> get_blob_offset(const json::Object &accessor);

    Result<void, GltfParser::Error, StreamError, json::TreeError, PngError>
    process_texture(TextureType type, String &path, String desired_path, Optional<uint64_t> index,
                    Optional<Vec4f> colour_factor);
    Result<void, GltfParser::Error, StreamError, json::TreeError, PngError>
    process_material(const json::Object &material, uint64_t index);

    Result<void, GltfParser::Error, StreamError, json::TreeError> process_primitive(const json::Object &primitive,
                                                                                    String &&name);

    Result<Material, json::TreeError> make_material(const json::Object &primitive);
    Result<void, GltfParser::Error, StreamError, json::TreeError> visit_node(World &world, EntityId parent_id,
                                                                             uint64_t index);
    Result<void, GltfParser::Error, StreamError, json::TreeError> process_scene(const json::Object &scene,
                                                                                StringView name);

    Result<void, GltfParser::Error, StreamError, json::TreeError> convert();
};

Result<Tuple<uint64_t, uint64_t>, GltfParser::Error, json::TreeError>
Converter::get_blob_info(const json::Object &accessor) {
    uint64_t accessor_offset = 0;
    if (accessor["byteOffset"].has<int64_t>()) {
        accessor_offset = static_cast<uint64_t>(VULL_ASSUME(accessor["byteOffset"].get<int64_t>()));
    }

    const auto buffer_view_index = static_cast<uint64_t>(VULL_TRY(accessor["bufferView"].get<int64_t>()));
    const auto &buffer_view = VULL_TRY(m_document["bufferViews"][buffer_view_index].get<json::Object>());
    const auto view_length = static_cast<uint64_t>(VULL_TRY(buffer_view["byteLength"].get<int64_t>()));

    uint64_t view_offset = 0;
    if (buffer_view["byteOffset"].has<int64_t>()) {
        view_offset = static_cast<uint64_t>(VULL_ASSUME(buffer_view["byteOffset"].get<int64_t>()));
    }

    // TODO: Assuming buffer == 0 here; support external blobs.
    const auto combined_offset = accessor_offset + view_offset;
    if (combined_offset + view_length > m_binary_blob.size()) {
        return GltfParser::Error::OffsetOutOfBounds;
    }
    return vull::make_tuple(combined_offset, static_cast<uint64_t>(view_length));
}

Result<uint64_t, GltfParser::Error, json::TreeError> Converter::get_blob_offset(const json::Object &accessor) {
    return vull::get<0>(VULL_TRY(get_blob_info(accessor)));
}

Result<void, GltfParser::Error> validate_accessor(const json::Object &accessor) {
    // TODO: Should probably check componentType and type as well.
    if (accessor["normalized"].has<bool>() && VULL_ASSUME(accessor["normalized"].get<bool>())) {
        return GltfParser::Error::UnsupportedNormalisedAccessor;
    }
    if (accessor["sparse"].has<bool>() && VULL_ASSUME(accessor["sparse"].get<bool>())) {
        return GltfParser::Error::UnsupportedSparseAccessor;
    }
    return {};
}

vpak::ImageFilter convert_filter(int64_t type) {
    switch (type) {
    case 9729:
        return vpak::ImageFilter::Linear;
    case 9984:
        return vpak::ImageFilter::NearestMipmapNearest;
    case 9985:
        return vpak::ImageFilter::LinearMipmapNearest;
    case 9986:
        return vpak::ImageFilter::NearestMipmapLinear;
    case 9987:
        return vpak::ImageFilter::LinearMipmapLinear;
    default:
        return vpak::ImageFilter::Nearest;
    }
}

vpak::ImageWrapMode convert_wrap_mode(int64_t type) {
    switch (type) {
    case 33071:
        return vpak::ImageWrapMode::ClampToEdge;
    case 33648:
        return vpak::ImageWrapMode::MirroredRepeat;
    default:
        return vpak::ImageWrapMode::Repeat;
    }
}

Result<void, GltfParser::Error, StreamError, json::TreeError, PngError>
Converter::process_texture(TextureType type, String &path, String desired_path, Optional<uint64_t> index,
                           Optional<Vec4f> colour_factor) {
    Filter mipmap_filter;
    vpak::ImageFormat vpak_format;
    switch (type) {
    case TextureType::Albedo:
        mipmap_filter = Filter::Box;
        vpak_format = vpak::ImageFormat::Bc7Srgb;
        break;
    case TextureType::Normal:
        mipmap_filter = Filter::Gaussian;
        vpak_format = vpak::ImageFormat::Bc5Unorm;
        break;
    }

    // When undefined, a sampler with repeat wrapping and auto filtering SHOULD be used.
    auto mag_filter = vpak::ImageFilter::Linear;
    auto min_filter = vpak::ImageFilter::Linear;
    auto wrap_u = vpak::ImageWrapMode::Repeat;
    auto wrap_v = vpak::ImageWrapMode::Repeat;

    FloatImage float_image;
    if (index) {
        const auto &texture = VULL_TRY(m_document["textures"][*index]);
        const auto image_index = VULL_TRY(texture["source"].get<int64_t>());
        const auto &image = VULL_TRY(m_document["images"][image_index].get<json::Object>());

        const auto &mime_type = VULL_TRY(image["mimeType"].get<json::String>());
        if (mime_type != "image/png") {
            // TODO: Don't error, just fallback to error texture?
            return GltfParser::Error::UnsupportedImageMimeType;
        }

        auto blob_info = VULL_TRY(get_blob_info(image));
        auto span_stream =
            vull::make_unique<SpanStream>(m_binary_blob.subspan(vull::get<0>(blob_info), vull::get<1>(blob_info)));
        auto png_stream = VULL_TRY(PngStream::create(vull::move(span_stream)));

        auto unorm_data = ByteBuffer::create_uninitialised(png_stream.row_byte_count() * png_stream.height());
        for (uint32_t y = 0; y < png_stream.height(); y++) {
            png_stream.read_row(unorm_data.span().subspan(y * png_stream.row_byte_count()));
        }

        float_image = FloatImage::from_unorm(unorm_data.span(), Vec2u(png_stream.width(), png_stream.height()),
                                             png_stream.pixel_byte_count());

        if (texture["sampler"]) {
            const auto sampler_index = VULL_TRY(texture["sampler"].get<int64_t>());
            const auto &sampler = VULL_TRY(m_document["samplers"][sampler_index].get<json::Object>());
            if (sampler["magFilter"]) {
                mag_filter = convert_filter(VULL_TRY(sampler["magFilter"].get<int64_t>()));
            }
            if (sampler["minFilter"]) {
                min_filter = convert_filter(VULL_TRY(sampler["minFilter"].get<int64_t>()));
            }
            if (sampler["wrapS"]) {
                wrap_u = convert_wrap_mode(VULL_TRY(sampler["wrapS"].get<int64_t>()));
            }
            if (sampler["wrapT"]) {
                wrap_v = convert_wrap_mode(VULL_TRY(sampler["wrapT"].get<int64_t>()));
            }
        }
    }

    if (colour_factor && float_image.mip_count() != 0) {
        // Explicit colour factor to multiply with image.
        // TODO: Multiply with image.
        VULL_ENSURE_NOT_REACHED();
    } else if (colour_factor) {
        // Explicit colour factor but no image, solid colour.
        float_image = FloatImage::from_colour(Colour::from_rgb(*colour_factor));
    } else if (float_image.mip_count() == 0) {
        // No image or explicit colour factor, assume white.
        float_image = FloatImage::from_colour(Colour::white());
    }

    if (type == TextureType::Normal) {
        float_image.colours_to_vectors();
    }
    float_image.build_mipchain(mipmap_filter);
    if (type == TextureType::Normal) {
        float_image.normalise();
        float_image.vectors_to_colours();
    }

    // Drop first mip if not wanting max resolution textures.
    constexpr uint32_t log_threshold_resolution = 11u;
    if (!m_max_resolution && float_image.mip_count() > log_threshold_resolution) {
        float_image.drop_mips(1);
    }

    auto stream = m_pack_writer.start_entry(path = vull::move(desired_path), vpak::EntryType::Image);
    VULL_TRY(stream.write_byte(vull::to_underlying(vpak_format)));
    VULL_TRY(stream.write_byte(vull::to_underlying(mag_filter)));
    VULL_TRY(stream.write_byte(vull::to_underlying(min_filter)));
    VULL_TRY(stream.write_byte(vull::to_underlying(wrap_u)));
    VULL_TRY(stream.write_byte(vull::to_underlying(wrap_v)));
    VULL_TRY(stream.write_varint(float_image.size().x()));
    VULL_TRY(stream.write_varint(float_image.size().y()));
    VULL_TRY(stream.write_varint(float_image.mip_count()));
    VULL_TRY(float_image.block_compress(stream, vpak_format == vpak::ImageFormat::Bc5Unorm));
    stream.finish();
    return {};
}

Result<void, GltfParser::Error, StreamError, json::TreeError, PngError>
Converter::process_material(const json::Object &material, uint64_t index) {
    String name;
    if (material["name"].has<String>()) {
        name = String(VULL_ASSUME(material["name"].get<String>()));
    } else {
        name = vull::format("material{}", index);
    }

    if (material["occlusionTexture"]) {
        vull::warn("[gltf] Ignoring unsupported occlusion texture on material '{}'", name);
    }
    if (material["emissiveTexture"] || material["emissiveFactor"]) {
        vull::warn("[gltf] Ignoring unsupported emissive properties on material '{}'", name);
    }

    // TODO(json): .get_or(fallback_value) function.
    StringView alpha_mode = "OPAQUE";
    double alpha_cutoff = 0.5;
    bool double_sided = false;
    if (material["alphaMode"].has<String>()) {
        alpha_mode = VULL_ASSUME(material["alphaMode"].get<String>());
    }
    if (material["alphaCutoff"].has<double>()) {
        alpha_cutoff = VULL_ASSUME(material["alphaCutoff"].get<double>());
    }
    if (material["doubleSided"].has<bool>()) {
        double_sided = VULL_ASSUME(material["doubleSided"].get<bool>());
    }

    if (alpha_mode != "OPAQUE") {
        vull::warn("[gltf] Ignoring unsupported alpha mode {} on material '{}'", alpha_mode, name);
    }
    if (alpha_cutoff != 0.5) {
        vull::warn("[gltf] Ignoring non-default alpha cutoff of {} on material '{}'", alpha_cutoff, name);
    }
    if (double_sided) {
        vull::warn("[gltf] Ignoring unsupported double sided property on material '{}'", name);
    }

    // TODO: In addition to the material properties, if a primitive specifies a vertex color using the attribute
    //       semantic property COLOR_0, then this value acts as an additional linear multiplier to base color.
    String albedo_path = "/default_albedo";
    auto pbr_info = material["pbrMetallicRoughness"];
    if (pbr_info) {
        Optional<uint64_t> texture_index;
        if (pbr_info["baseColorTexture"]["index"].has<int64_t>()) {
            texture_index = static_cast<uint64_t>(VULL_ASSUME(pbr_info["baseColorTexture"]["index"].get<int64_t>()));
        }
        Optional<Vec4f> colour_factor;
        if (pbr_info["baseColorFactor"].has<json::Array>()) {
            const auto &factor = VULL_ASSUME(pbr_info["baseColorFactor"].get<json::Array>());
            colour_factor = Vec4f(VULL_TRY(factor[0].get<double>()), VULL_TRY(factor[1].get<double>()),
                                  VULL_TRY(factor[2].get<double>()), VULL_TRY(factor[3].get<double>()));
        }
        VULL_TRY(process_texture(TextureType::Albedo, albedo_path, vull::format("/materials/{}/albedo", name),
                                 texture_index, colour_factor));
    }

    String normal_path = "/default_normal";
    const auto normal_info = material["normalTexture"];
    if (normal_info) {
        int64_t tex_coord = 0;
        double scale = 1.0;
        if (normal_info["texCoord"].has<int64_t>()) {
            tex_coord = VULL_ASSUME(normal_info["texCoord"].get<int64_t>());
        }
        if (normal_info["scale"].has<double>()) {
            scale = VULL_ASSUME(normal_info["scale"].get<double>());
        }

        if (tex_coord != 0) {
            vull::warn("[gltf] Ignoring unsupported texCoord attribute index {} on material '{}'", tex_coord, name);
        }
        if (scale != 1.0) {
            vull::warn("[gltf] Ignoring non-one normal map scale of {} on material '{}'", scale, name);
        }

        const auto texture_index = static_cast<uint64_t>(VULL_TRY(normal_info["index"].get<int64_t>()));
        VULL_TRY(process_texture(TextureType::Normal, normal_path, vull::format("/materials/{}/normal", name),
                                 texture_index, {}));
    }

    ScopedLock lock(m_material_map_mutex);
    m_albedo_paths.set(index, albedo_path);
    m_normal_paths.set(index, normal_path);
    return {};
}

size_t convert_index_type(int64_t index_type) {
    // This produces better code than a switch.
    const auto mapped = static_cast<size_t>(index_type) - 5121;
    return mapped != 0 ? mapped : 1;
}

Result<void, GltfParser::Error, StreamError, json::TreeError>
Converter::process_primitive(const json::Object &primitive, String &&name) {
    if (primitive["mode"].has<int64_t>()) {
        const auto mode = VULL_ASSUME(primitive["mode"].get<int64_t>());
        if (mode != 4) {
            // Not triangles.
            return GltfParser::Error::UnsupportedPrimitiveMode;
        }
    }

    // Get accessors for all the attributes we care about.
    // TODO: Spec says we should support two UV sets, vertex colours, etc.
    const auto position_accessor_index = VULL_TRY(primitive["attributes"]["POSITION"].get<int64_t>());
    const auto normal_accessor_index = VULL_TRY(primitive["attributes"]["NORMAL"].get<int64_t>());
    const auto uv_accessor_index = VULL_TRY(primitive["attributes"]["TEXCOORD_0"].get<int64_t>());
    const auto index_accessor_index = VULL_TRY(primitive["indices"].get<int64_t>());
    const auto &position_accessor = VULL_TRY(m_document["accessors"][position_accessor_index].get<json::Object>());
    const auto &normal_accessor = VULL_TRY(m_document["accessors"][normal_accessor_index].get<json::Object>());
    const auto &uv_accessor = VULL_TRY(m_document["accessors"][uv_accessor_index].get<json::Object>());
    const auto &index_accessor = VULL_TRY(m_document["accessors"][index_accessor_index].get<json::Object>());

    // Validate the accessors.
    VULL_TRY(validate_accessor(position_accessor));
    VULL_TRY(validate_accessor(normal_accessor));
    VULL_TRY(validate_accessor(uv_accessor));
    VULL_TRY(validate_accessor(index_accessor));

    const auto index_count = static_cast<uint64_t>(VULL_TRY(index_accessor["count"].get<int64_t>()));
    const auto index_size = convert_index_type(VULL_TRY(index_accessor["componentType"].get<int64_t>()));
    auto index_offset = VULL_TRY(get_blob_offset(index_accessor));

    auto indices = FixedBuffer<uint32_t>::create_zeroed(index_count);
    for (auto &index : indices) {
        auto index_bytes = m_binary_blob.subspan(index_offset, index_size);
        switch (index_size) {
        case sizeof(uint32_t):
            index |= static_cast<uint32_t>(index_bytes[3]) << 24u;
            index |= static_cast<uint32_t>(index_bytes[2]) << 16u;
            [[fallthrough]];
        case sizeof(uint16_t):
            index |= static_cast<uint32_t>(index_bytes[1]) << 8u;
            [[fallthrough]];
        case sizeof(uint8_t):
            index |= static_cast<uint32_t>(index_bytes[0]) << 0u;
            break;
        default:
            vull::unreachable();
        }
        index_offset += index_size;
    }

    const auto vertex_count = static_cast<uint64_t>(VULL_TRY(position_accessor["count"].get<int64_t>()));
    auto position_offset = VULL_TRY(get_blob_offset(position_accessor));
    auto normal_offset = VULL_TRY(get_blob_offset(normal_accessor));
    auto uv_offset = VULL_TRY(get_blob_offset(uv_accessor));

    auto vertices = FixedBuffer<Vertex>::create_uninitialised(vertex_count);
    for (auto &vertex : vertices) {
        memcpy(&vertex.position, m_binary_blob.byte_offset(position_offset), sizeof(Vec3f));
        memcpy(&vertex.normal, m_binary_blob.byte_offset(normal_offset), sizeof(Vec3f));
        memcpy(&vertex.uv, m_binary_blob.byte_offset(uv_offset), sizeof(Vec2f));
        position_offset += sizeof(Vec3f);
        normal_offset += sizeof(Vec3f);
        uv_offset += sizeof(Vec2f);
    }

    // TODO: Don't do this if --fast passed.
    meshopt_optimizeVertexCache(indices.data(), indices.data(), indices.size(), vertices.size());
    meshopt_optimizeVertexFetch(vertices.data(), indices.data(), indices.size(), vertices.data(), vertices.size(),
                                sizeof(Vertex));

    auto vertex_data_entry = m_pack_writer.start_entry(vull::format("/meshes/{}/vertex", name), vpak::EntryType::Blob);
    VULL_TRY(vertex_data_entry.write(vertices.span()));
    vertex_data_entry.finish();

    auto index_data_entry = m_pack_writer.start_entry(vull::format("/meshes/{}/index", name), vpak::EntryType::Blob);
    VULL_TRY(index_data_entry.write(indices.span()));
    index_data_entry.finish();

    Vec3f aabb_min(FLT_MAX);
    Vec3f aabb_max(FLT_MIN);
    Vec3f sphere_center;
    for (const auto &vertex : vertices) {
        aabb_min = vull::min(aabb_min, vertex.position);
        aabb_max = vull::max(aabb_max, vertex.position);
        sphere_center += vertex.position;
    }
    sphere_center /= static_cast<float>(vertices.size());

    float sphere_radius = 0;
    for (const auto &vertex : vertices) {
        sphere_radius = vull::max(sphere_radius, vull::distance(sphere_center, vertex.position));
    }

    BoundingBox bounding_box((aabb_min + aabb_max) * 0.5f, (aabb_max - aabb_min) * 0.5f);
    BoundingSphere bounding_sphere(sphere_center, sphere_radius);
    ScopedLock lock(m_mesh_bounds_mutex);
    m_mesh_bounds.set(vull::move(name), {bounding_box, bounding_sphere});
    return {};
}

Result<void, GltfParser::Error, json::TreeError> array_to_vec(const json::Array &array, auto &vec) {
    if (array.size() != vull::remove_ref<decltype(vec)>::length) {
        return GltfParser::Error::BadVectorArrayLength;
    }
    for (uint64_t i = 0; i < array.size(); i++) {
        auto value = VULL_TRY(array[i].get<double>());
        vec[static_cast<unsigned>(i)] = static_cast<float>(value);
    }
    return {};
}

Result<Material, json::TreeError> Converter::make_material(const json::Object &primitive) {
    if (!primitive["material"]) {
        return Material("/default_albedo", "/default_normal");
    }
    auto index = static_cast<uint64_t>(VULL_TRY(primitive["material"].get<int64_t>()));

    String albedo_path = "/default_albedo";
    if (auto path = m_albedo_paths.get(index)) {
        albedo_path = String(*path);
    }

    String normal_path = "/default_normal";
    if (auto path = m_normal_paths.get(index)) {
        normal_path = String(*path);
    }
    return Material(vull::move(albedo_path), vull::move(normal_path));
}

Result<void, GltfParser::Error, StreamError, json::TreeError> Converter::visit_node(World &world, EntityId parent_id,
                                                                                    uint64_t index) {
    const auto &node = VULL_TRY(m_document["nodes"][index].get<json::Object>());
    if (node["matrix"]) {
        return GltfParser::Error::UnsupportedNodeMatrix;
    }

    Vec3f position;
    if (auto translation_array = node["translation"].get<json::Array>().to_optional()) {
        VULL_TRY(array_to_vec(*translation_array, position));
    }

    Quatf rotation;
    if (auto rotation_array = node["rotation"].get<json::Array>().to_optional()) {
        VULL_TRY(array_to_vec(*rotation_array, rotation));
    }

    Vec3f scale(1.0f);
    if (auto scale_array = node["scale"].get<json::Array>().to_optional()) {
        VULL_TRY(array_to_vec(*scale_array, scale));
    }

    auto entity = world.create_entity();
    entity.add<Transform>(parent_id, position, rotation, scale);

    // TODO: Make this much simpler by not using name strings as keys?
    if (auto mesh_index = node["mesh"].get<int64_t>().to_optional()) {
        const auto &mesh = VULL_TRY(m_document["meshes"][*mesh_index].get<json::Object>());
        const auto &mesh_name = VULL_TRY(mesh["name"].get<String>());
        const auto &primitive_array = VULL_TRY(mesh["primitives"].get<json::Array>());

        auto build_entity = [&](Entity &sub_entity, uint64_t primitive_index) -> Result<void, json::TreeError> {
            const auto &primitive = VULL_TRY(primitive_array[primitive_index].get<json::Object>());
            sub_entity.add<Material>(VULL_TRY(make_material(primitive)));
            sub_entity.add<Mesh>(vull::format("/meshes/{}.{}/vertex", mesh_name, primitive_index),
                                 vull::format("/meshes/{}.{}/index", mesh_name, primitive_index));
            if (auto bounds = m_mesh_bounds.get(vull::format("{}.{}", mesh_name, primitive_index))) {
                sub_entity.add<BoundingBox>(bounds->box);
                sub_entity.add<BoundingSphere>(bounds->sphere);
            }
            return {};
        };

        if (primitive_array.size() == 1) {
            VULL_TRY(build_entity(entity, 0));
        } else {
            for (uint64_t i = 0; i < primitive_array.size(); i++) {
                auto sub_entity = world.create_entity();
                sub_entity.add<Transform>(entity);
                VULL_TRY(build_entity(sub_entity, i));
            }
        }
    }

    if (auto children_array = node["children"].get<json::Array>().to_optional()) {
        for (uint64_t i = 0; i < children_array->size(); i++) {
            auto child_index = static_cast<uint64_t>(VULL_TRY((*children_array)[i].get<int64_t>()));
            VULL_TRY(visit_node(world, entity, child_index));
        }
    }
    return {};
}

Result<void, GltfParser::Error, StreamError, json::TreeError> Converter::process_scene(const json::Object &scene,
                                                                                       StringView name) {
    World world;
    world.register_component<Transform>();
    world.register_component<Mesh>();
    world.register_component<Material>();
    world.register_component<BoundingBox>();
    world.register_component<BoundingSphere>();

    const auto &root_node_array = VULL_TRY(scene["nodes"].get<json::Array>());
    for (uint64_t i = 0; i < root_node_array.size(); i++) {
        const auto index = static_cast<uint64_t>(VULL_TRY(root_node_array[i].get<int64_t>()));
        VULL_TRY(visit_node(world, ~EntityId(0), index));
    }

    // Serialise to vpak.
    const auto entry_name = vull::format("/scenes/{}", name);
    VULL_TRY(world.serialise(m_pack_writer, entry_name));
    return {};
}

Result<void, GltfParser::Error, StreamError, json::TreeError> Converter::convert() {
    // TODO: These defaults textures should be created once in the engine.

    // Emit default albedo texture.
    auto albedo_entry = m_pack_writer.start_entry("/default_albedo", vpak::EntryType::Image);
    VULL_TRY(albedo_entry.write_byte(uint8_t(vpak::ImageFormat::RgbaUnorm)));
    VULL_TRY(albedo_entry.write_byte(uint8_t(vpak::ImageFilter::Nearest)));
    VULL_TRY(albedo_entry.write_byte(uint8_t(vpak::ImageFilter::Nearest)));
    VULL_TRY(albedo_entry.write_byte(uint8_t(vpak::ImageWrapMode::Repeat)));
    VULL_TRY(albedo_entry.write_byte(uint8_t(vpak::ImageWrapMode::Repeat)));
    VULL_TRY(albedo_entry.write_varint(16u));
    VULL_TRY(albedo_entry.write_varint(16u));
    VULL_TRY(albedo_entry.write_varint(1u));
    constexpr Array colours{
        Vec<uint8_t, 4>(0xff, 0x69, 0xb4, 0xff),
        Vec<uint8_t, 4>(0x94, 0x00, 0xd3, 0xff),
    };
    for (uint32_t y = 0; y < 16; y++) {
        for (uint32_t x = 0; x < 16; x++) {
            uint32_t colour_index = (x + y) % colours.size();
            VULL_TRY(albedo_entry.write({&colours[colour_index], 4}));
        }
    }
    albedo_entry.finish();

    // Emit default normal map texture.
    auto normal_entry = m_pack_writer.start_entry("/default_normal", vpak::EntryType::Image);
    VULL_TRY(normal_entry.write_byte(uint8_t(vpak::ImageFormat::RgUnorm)));
    VULL_TRY(albedo_entry.write_byte(uint8_t(vpak::ImageFilter::Linear)));
    VULL_TRY(albedo_entry.write_byte(uint8_t(vpak::ImageFilter::Linear)));
    VULL_TRY(albedo_entry.write_byte(uint8_t(vpak::ImageWrapMode::Repeat)));
    VULL_TRY(albedo_entry.write_byte(uint8_t(vpak::ImageWrapMode::Repeat)));
    VULL_TRY(normal_entry.write(Array<uint8_t, 5>{1u, 1u, 1u, 127u, 127u}.span()));
    normal_entry.finish();

    const auto &material_array = VULL_TRY(m_document["materials"].get<json::Array>());
    Latch material_latch(material_array.size());
    for (uint32_t i = 0; i < material_array.size(); i++) {
        const auto &material = VULL_TRY(material_array[i].get<json::Object>());
        vull::schedule([this, &material_latch, &material, index = i] {
            // TODO(tasklet): Need a future system to propagate errors.
            VULL_EXPECT(process_material(material, index));
            material_latch.count_down();
        });
    }

    const auto &mesh_array = VULL_TRY(m_document["meshes"].get<json::Array>());
    uint32_t total_primitive_count = 0;
    for (uint32_t i = 0; i < mesh_array.size(); i++) {
        const auto &primitive_array = VULL_TRY(mesh_array[i]["primitives"].get<json::Array>());
        total_primitive_count += primitive_array.size();
    }

    Latch mesh_latch(total_primitive_count);
    for (uint64_t i = 0; i < mesh_array.size(); i++) {
        // TODO: Spec doesn't require meshes to have a name (do what process_material does).
        const auto &mesh = VULL_TRY(mesh_array[i].get<json::Object>());
        const auto &mesh_name = VULL_TRY(mesh["name"].get<String>());
        const auto &primitive_array = VULL_TRY(mesh["primitives"].get<json::Array>());
        for (uint64_t j = 0; j < primitive_array.size(); j++) {
            const auto &primitive = VULL_TRY(primitive_array[j].get<json::Object>());
            vull::schedule([this, &mesh_latch, &primitive, name = vull::format("{}.{}", mesh_name, j)]() mutable {
                // TODO(tasklet): Need a future system to propagate errors.
                VULL_EXPECT(process_primitive(primitive, vull::move(name)));
                mesh_latch.count_down();
            });
        }
    }

    material_latch.wait();
    mesh_latch.wait();
    return {};
}

} // namespace

Result<void, GltfParser::Error, StreamError> GltfParser::parse_glb() {
    if (const auto magic = VULL_TRY(m_stream.read_le<uint32_t>()); magic != 0x46546c67u) {
        vull::error("[gltf] Invalid magic number: {h}", magic);
        return Error::InvalidMagic;
    }

    if (const auto version = VULL_TRY(m_stream.read_le<uint32_t>()); version != 2u) {
        vull::error("[gltf] Unsupported version: {}", version);
        return Error::UnsupportedVersion;
    }

    // Ignore header size.
    VULL_TRY(m_stream.read_le<uint32_t>());

    // glTF 2 must have a single JSON chunk at the start.
    const auto json_length = VULL_TRY(m_stream.read_le<uint32_t>());
    if (VULL_TRY(m_stream.read_le<uint32_t>()) != 0x4e4f534au) {
        vull::error("[gltf] Missing or invalid JSON chunk");
        return Error::BadJsonChunk;
    }
    m_json = String(json_length);
    VULL_TRY(m_stream.read({m_json.data(), json_length}));

    // Followed by a binary chunk.
    const auto binary_length = VULL_TRY(m_stream.read_le<uint32_t>());
    if (VULL_TRY(m_stream.read_le<uint32_t>()) != 0x004e4942u) {
        vull::error("[gltf] Missing or invalid binary chunk");
        return Error::BadBinaryChunk;
    }
    m_binary_blob = ByteBuffer::create_uninitialised(binary_length);
    VULL_TRY(m_stream.read(m_binary_blob.span()));
    return {};
}

Result<void, GltfParser::Error, StreamError, json::ParseError, json::TreeError>
GltfParser::convert(vpak::Writer &pack_writer, bool max_resolution, bool reproducible) {
    auto document = VULL_TRY(json::parse(m_json));
    if (auto generator = document["asset"]["generator"].get<String>()) {
        vull::info("[gltf] Generator: {}", generator.value());
    }

    if (auto required_extensions = document["extensionsRequired"].get<json::Array>().to_optional()) {
        for (uint32_t i = 0; i < required_extensions->size(); i++) {
            auto name = VULL_TRY((*required_extensions)[i].get<String>());
            vull::warn("[gltf] Required extension {} not supported", name);
        }
    }

    Converter converter(m_binary_blob.span(), pack_writer, document, max_resolution);

    // Use only one thread if reproducible, otherwise let scheduler decide.
    {
        Scheduler scheduler(reproducible ? 1 : 0);
        scheduler.start([&] {
            // TODO(tasklet): Need a future system to propagate errors.
            VULL_EXPECT(converter.convert());
            scheduler.stop();
        });
    }

    if (!document["scenes"]) {
        return {};
    }

    const auto &scene_array = VULL_TRY(document["scenes"].get<json::Array>());
    for (uint64_t i = 0; i < scene_array.size(); i++) {
        const auto &scene = VULL_TRY(scene_array[i].get<json::Object>());
        const auto &name = VULL_TRY(scene["name"].get<String>());

        vull::info("[gltf] Creating scene '{}'", name);
        VULL_TRY(converter.process_scene(scene, name));
    }
    return {};
}

} // namespace vull
