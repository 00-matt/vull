#pragma once

#include <vull/support/Span.hh>
#include <vull/support/Vector.hh>
#include <vull/vulkan/Vulkan.hh>

#include <stdint.h>

namespace vull::vk {

class Buffer;
class CommandPool;
class Context;
class QueryPool;

class CommandBuffer {
    friend CommandPool;

    struct DescriptorBufferBinding {
        vkb::PipelineBindPoint bind_point;
        uint32_t set;
        uint32_t buffer_index;
        vkb::DeviceSize offset;
    };

private:
    const Context &m_context;
    const vkb::CommandBuffer m_cmd_buf;
    vkb::Semaphore m_completion_semaphore;
    uint64_t m_completion_value{0};
    Vector<Buffer> m_associated_buffers;
    Vector<vkb::DescriptorBufferBindingInfoEXT> m_descriptor_buffers;
    Vector<DescriptorBufferBinding> m_descriptor_buffer_bindings;
    vkb::PipelineLayout m_compute_layout;
    vkb::PipelineLayout m_graphics_layout;

    void reset();
    void emit_descriptor_binds();

public:
    CommandBuffer(const Context &context, vkb::CommandBuffer cmd_buf);
    CommandBuffer(const CommandBuffer &) = delete;
    CommandBuffer(CommandBuffer &&);
    ~CommandBuffer();

    CommandBuffer &operator=(const CommandBuffer &) = delete;
    CommandBuffer &operator=(CommandBuffer &&) = delete;

    void begin_rendering(const vkb::RenderingInfo &rendering_info) const;
    void end_rendering() const;

    void bind_associated_buffer(Buffer &&buffer);
    void bind_descriptor_buffer(vkb::PipelineBindPoint bind_point, const Buffer &buffer, uint32_t set,
                                vkb::DeviceSize offset);
    void bind_index_buffer(vkb::Buffer buffer, vkb::IndexType index_type) const;
    void bind_layout(vkb::PipelineBindPoint bind_point, vkb::PipelineLayout layout);
    void bind_pipeline(vkb::PipelineBindPoint bind_point, vkb::Pipeline pipeline) const;
    void bind_vertex_buffer(vkb::Buffer buffer) const;

    void copy_buffer(vkb::Buffer src, vkb::Buffer dst, Span<vkb::BufferCopy> regions) const;
    void copy_buffer_to_image(vkb::Buffer src, vkb::Image dst, vkb::ImageLayout dst_layout,
                              Span<vkb::BufferImageCopy> regions) const;
    void push_constants(vkb::ShaderStage stage, uint32_t size, const void *data) const;

    void dispatch(uint32_t x, uint32_t y = 1, uint32_t z = 1);
    void draw(uint32_t vertex_count, uint32_t instance_count);
    void draw_indexed(uint32_t index_count, uint32_t instance_count);

    void image_barrier(const vkb::ImageMemoryBarrier2 &barrier) const;
    void pipeline_barrier(const vkb::DependencyInfo &dependency_info) const;
    void reset_query_pool(const QueryPool &query_pool) const;
    void write_timestamp(vkb::PipelineStage2 stage, const QueryPool &query_pool, uint32_t query) const;

    vkb::CommandBuffer operator*() const { return m_cmd_buf; }
    vkb::Semaphore completion_semaphore() const { return m_completion_semaphore; }
    uint64_t completion_value() const { return m_completion_value; }
};

} // namespace vull::vk
