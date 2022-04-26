#pragma once

#include <vull/support/Span.hh>
#include <vull/vulkan/Vulkan.hh>

#include <stdint.h>

namespace vull {

class CommandPool;
class VkContext;

class CommandBuffer {
    friend CommandPool;

private:
    const VkContext &m_context;
    const vk::CommandBuffer m_cmd_buf;
    vk::Semaphore m_completion_semaphore;
    uint64_t m_completion_value{0};

    void reset();

public:
    CommandBuffer(const VkContext &context, vk::CommandBuffer cmd_buf);
    CommandBuffer(const CommandBuffer &) = delete;
    CommandBuffer(CommandBuffer &&);
    ~CommandBuffer();

    CommandBuffer &operator=(const CommandBuffer &) = delete;
    CommandBuffer &operator=(CommandBuffer &&) = delete;

    void begin_rendering(const vk::RenderingInfo &rendering_info) const;
    void end_rendering() const;

    void bind_descriptor_sets(vk::PipelineBindPoint bind_point, vk::PipelineLayout layout,
                              Span<vk::DescriptorSet> descriptor_sets) const;
    void bind_index_buffer(vk::Buffer buffer, vk::IndexType index_type) const;
    void bind_pipeline(vk::PipelineBindPoint bind_point, vk::Pipeline pipeline) const;
    void bind_vertex_buffer(vk::Buffer buffer) const;

    void copy_buffer(vk::Buffer src, vk::Buffer dst, Span<vk::BufferCopy> regions) const;
    void copy_buffer_to_image(vk::Buffer src, vk::Image dst, vk::ImageLayout dst_layout,
                              Span<vk::BufferImageCopy> regions) const;
    void push_constants(vk::PipelineLayout layout, vk::ShaderStage stage, uint32_t size, const void *data) const;

    void dispatch(uint32_t x, uint32_t y, uint32_t z) const;
    void draw(uint32_t vertex_count, uint32_t instance_count) const;
    void draw_indexed(uint32_t index_count, uint32_t instance_count) const;

    void pipeline_barrier(vk::PipelineStage src_stage, vk::PipelineStage dst_stage,
                          Span<vk::BufferMemoryBarrier> buffer_barriers,
                          Span<vk::ImageMemoryBarrier> image_barriers) const;
    void reset_query_pool(vk::QueryPool query_pool, uint32_t query_count) const;
    void write_timestamp(vk::PipelineStage stage, vk::QueryPool query_pool, uint32_t query) const;

    vk::CommandBuffer operator*() const { return m_cmd_buf; }
    vk::Semaphore completion_semaphore() const { return m_completion_semaphore; }
    uint64_t completion_value() const { return m_completion_value; }
};

} // namespace vull
