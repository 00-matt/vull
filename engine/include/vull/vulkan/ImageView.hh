#pragma once

#include <vull/vulkan/Vulkan.hh>

namespace vull::vk {

class Context;
class Image;

class ImageView {
    friend Context;
    friend Image;

private:
    const Context *m_context{nullptr};
    vkb::Image m_image{nullptr};
    vkb::ImageView m_view{nullptr};
    vkb::ImageSubresourceRange m_range{};

    ImageView(const Context &context, vkb::Image image, vkb::ImageView view, const vkb::ImageSubresourceRange &range)
        : m_context(&context), m_image(image), m_view(view), m_range(range) {}

public:
    ImageView() = default;
    ImageView(const ImageView &) = delete;
    ImageView(ImageView &&);
    ~ImageView();

    ImageView &operator=(const ImageView &) = delete;
    ImageView &operator=(ImageView &&);

    vkb::ImageView operator*() const { return m_view; }
    vkb::Image image() const { return m_image; }
    const Context *context() const { return m_context; }
    const vkb::ImageSubresourceRange &range() const { return m_range; }
};

} // namespace vull::vk
