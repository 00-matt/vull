#pragma once

#include <vull/ecs/EntityId.hh>
#include <vull/ecs/World.hh>
#include <vull/maths/Mat.hh>
#include <vull/support/HashMap.hh>
#include <vull/support/Optional.hh>
#include <vull/support/String.hh>
#include <vull/support/StringView.hh>
#include <vull/support/Vector.hh>
#include <vull/vulkan/Image.hh>
#include <vull/vulkan/Vulkan.hh>

#include <stdint.h>

namespace vull::vk {

class Context;

} // namespace vull::vk

namespace vull::vpak {

class ReadStream;
class Reader;

} // namespace vull::vpak

namespace vull {

class Scene {
    vk::Context &m_context;
    World m_world;
    HashMap<String, uint32_t> m_texture_indices;
    Vector<vk::Image> m_texture_images;
    Vector<vkb::Sampler> m_texture_samplers;
    vkb::Sampler m_linear_sampler{nullptr};
    vkb::Sampler m_nearest_sampler{nullptr};

    vk::Image load_image(vpak::ReadStream &);

public:
    explicit Scene(vk::Context &context) : m_context(context) {}
    Scene(const Scene &) = delete;
    Scene(Scene &&) = delete;
    ~Scene();

    Scene &operator=(const Scene &) = delete;
    Scene &operator=(Scene &&) = delete;

    Mat4f get_transform_matrix(EntityId entity);
    void load(vpak::Reader &pack_reader, StringView scene_name);

    World &world() { return m_world; }
    Optional<const uint32_t &> texture_index(const String &name) const { return m_texture_indices.get(name); }
    uint32_t texture_count() const { return m_texture_images.size(); }
    const Vector<vk::Image> &texture_images() const { return m_texture_images; }
    const Vector<vkb::Sampler> &texture_samplers() const { return m_texture_samplers; }
};

} // namespace vull
