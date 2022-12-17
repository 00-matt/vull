#include <vull/vulkan/ImageView.hh>

#include <vull/support/Utility.hh>
#include <vull/vulkan/Context.hh>
#include <vull/vulkan/Vulkan.hh>

namespace vull::vk {

ImageView::ImageView(ImageView &&other) {
    m_context = vull::exchange(other.m_context, nullptr);
    m_image = vull::exchange(other.m_image, nullptr);
    m_view = vull::exchange(other.m_view, nullptr);
    m_range = vull::exchange(other.m_range, {});
}

ImageView::~ImageView() {
    if (m_context != nullptr) {
        m_context->vkDestroyImageView(m_view);
    }
}

ImageView &ImageView::operator=(ImageView &&other) {
    ImageView moved(vull::move(other));
    vull::swap(m_context, moved.m_context);
    vull::swap(m_image, moved.m_image);
    vull::swap(m_view, moved.m_view);
    vull::swap(m_range, moved.m_range);
    return *this;
}

} // namespace vull::vk
