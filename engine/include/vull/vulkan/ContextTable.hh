// File generated by tools/gen_vk.py
#pragma once

#include <vull/vulkan/Vulkan.hh>

namespace vull::vk {

class ContextTable {
protected:
    Instance m_instance; // NOLINT
    PhysicalDevice m_physical_device; // NOLINT
    Device m_device; // NOLINT

    void load_loader(PFN_vkGetInstanceProcAddr get_instance_proc_addr);
    void load_instance(PFN_vkGetInstanceProcAddr get_instance_proc_addr);
    void load_device();

private:
    PFN_vkAcquireNextImage2KHR m_vkAcquireNextImage2KHR;
    PFN_vkAcquireNextImageKHR m_vkAcquireNextImageKHR;
    PFN_vkAllocateCommandBuffers m_vkAllocateCommandBuffers;
    PFN_vkAllocateDescriptorSets m_vkAllocateDescriptorSets;
    PFN_vkAllocateMemory m_vkAllocateMemory;
    PFN_vkBeginCommandBuffer m_vkBeginCommandBuffer;
    PFN_vkBindBufferMemory m_vkBindBufferMemory;
    PFN_vkBindBufferMemory2 m_vkBindBufferMemory2;
    PFN_vkBindImageMemory m_vkBindImageMemory;
    PFN_vkBindImageMemory2 m_vkBindImageMemory2;
    PFN_vkCmdBeginQuery m_vkCmdBeginQuery;
    PFN_vkCmdBeginRenderPass m_vkCmdBeginRenderPass;
    PFN_vkCmdBeginRenderPass2 m_vkCmdBeginRenderPass2;
    PFN_vkCmdBeginRenderingKHR m_vkCmdBeginRenderingKHR;
    PFN_vkCmdBindDescriptorSets m_vkCmdBindDescriptorSets;
    PFN_vkCmdBindIndexBuffer m_vkCmdBindIndexBuffer;
    PFN_vkCmdBindPipeline m_vkCmdBindPipeline;
    PFN_vkCmdBindVertexBuffers m_vkCmdBindVertexBuffers;
    PFN_vkCmdBlitImage m_vkCmdBlitImage;
    PFN_vkCmdClearAttachments m_vkCmdClearAttachments;
    PFN_vkCmdClearColorImage m_vkCmdClearColorImage;
    PFN_vkCmdClearDepthStencilImage m_vkCmdClearDepthStencilImage;
    PFN_vkCmdCopyBuffer m_vkCmdCopyBuffer;
    PFN_vkCmdCopyBufferToImage m_vkCmdCopyBufferToImage;
    PFN_vkCmdCopyImage m_vkCmdCopyImage;
    PFN_vkCmdCopyImageToBuffer m_vkCmdCopyImageToBuffer;
    PFN_vkCmdCopyQueryPoolResults m_vkCmdCopyQueryPoolResults;
    PFN_vkCmdDispatch m_vkCmdDispatch;
    PFN_vkCmdDispatchBase m_vkCmdDispatchBase;
    PFN_vkCmdDispatchIndirect m_vkCmdDispatchIndirect;
    PFN_vkCmdDraw m_vkCmdDraw;
    PFN_vkCmdDrawIndexed m_vkCmdDrawIndexed;
    PFN_vkCmdDrawIndexedIndirect m_vkCmdDrawIndexedIndirect;
    PFN_vkCmdDrawIndexedIndirectCount m_vkCmdDrawIndexedIndirectCount;
    PFN_vkCmdDrawIndirect m_vkCmdDrawIndirect;
    PFN_vkCmdDrawIndirectCount m_vkCmdDrawIndirectCount;
    PFN_vkCmdEndQuery m_vkCmdEndQuery;
    PFN_vkCmdEndRenderPass m_vkCmdEndRenderPass;
    PFN_vkCmdEndRenderPass2 m_vkCmdEndRenderPass2;
    PFN_vkCmdEndRenderingKHR m_vkCmdEndRenderingKHR;
    PFN_vkCmdExecuteCommands m_vkCmdExecuteCommands;
    PFN_vkCmdFillBuffer m_vkCmdFillBuffer;
    PFN_vkCmdNextSubpass m_vkCmdNextSubpass;
    PFN_vkCmdNextSubpass2 m_vkCmdNextSubpass2;
    PFN_vkCmdPipelineBarrier m_vkCmdPipelineBarrier;
    PFN_vkCmdPushConstants m_vkCmdPushConstants;
    PFN_vkCmdResetEvent m_vkCmdResetEvent;
    PFN_vkCmdResetQueryPool m_vkCmdResetQueryPool;
    PFN_vkCmdResolveImage m_vkCmdResolveImage;
    PFN_vkCmdSetBlendConstants m_vkCmdSetBlendConstants;
    PFN_vkCmdSetDepthBias m_vkCmdSetDepthBias;
    PFN_vkCmdSetDepthBounds m_vkCmdSetDepthBounds;
    PFN_vkCmdSetDeviceMask m_vkCmdSetDeviceMask;
    PFN_vkCmdSetEvent m_vkCmdSetEvent;
    PFN_vkCmdSetLineWidth m_vkCmdSetLineWidth;
    PFN_vkCmdSetScissor m_vkCmdSetScissor;
    PFN_vkCmdSetStencilCompareMask m_vkCmdSetStencilCompareMask;
    PFN_vkCmdSetStencilReference m_vkCmdSetStencilReference;
    PFN_vkCmdSetStencilWriteMask m_vkCmdSetStencilWriteMask;
    PFN_vkCmdSetViewport m_vkCmdSetViewport;
    PFN_vkCmdUpdateBuffer m_vkCmdUpdateBuffer;
    PFN_vkCmdWaitEvents m_vkCmdWaitEvents;
    PFN_vkCmdWriteTimestamp m_vkCmdWriteTimestamp;
    PFN_vkCreateBuffer m_vkCreateBuffer;
    PFN_vkCreateBufferView m_vkCreateBufferView;
    PFN_vkCreateCommandPool m_vkCreateCommandPool;
    PFN_vkCreateComputePipelines m_vkCreateComputePipelines;
    PFN_vkCreateDescriptorPool m_vkCreateDescriptorPool;
    PFN_vkCreateDescriptorSetLayout m_vkCreateDescriptorSetLayout;
    PFN_vkCreateDescriptorUpdateTemplate m_vkCreateDescriptorUpdateTemplate;
    PFN_vkCreateDevice m_vkCreateDevice;
    PFN_vkCreateEvent m_vkCreateEvent;
    PFN_vkCreateFence m_vkCreateFence;
    PFN_vkCreateFramebuffer m_vkCreateFramebuffer;
    PFN_vkCreateGraphicsPipelines m_vkCreateGraphicsPipelines;
    PFN_vkCreateImage m_vkCreateImage;
    PFN_vkCreateImageView m_vkCreateImageView;
    PFN_vkCreateInstance m_vkCreateInstance;
    PFN_vkCreatePipelineCache m_vkCreatePipelineCache;
    PFN_vkCreatePipelineLayout m_vkCreatePipelineLayout;
    PFN_vkCreateQueryPool m_vkCreateQueryPool;
    PFN_vkCreateRenderPass m_vkCreateRenderPass;
    PFN_vkCreateRenderPass2 m_vkCreateRenderPass2;
    PFN_vkCreateSampler m_vkCreateSampler;
    PFN_vkCreateSamplerYcbcrConversion m_vkCreateSamplerYcbcrConversion;
    PFN_vkCreateSemaphore m_vkCreateSemaphore;
    PFN_vkCreateShaderModule m_vkCreateShaderModule;
    PFN_vkCreateSwapchainKHR m_vkCreateSwapchainKHR;
    PFN_vkCreateXcbSurfaceKHR m_vkCreateXcbSurfaceKHR;
    PFN_vkDestroyBuffer m_vkDestroyBuffer;
    PFN_vkDestroyBufferView m_vkDestroyBufferView;
    PFN_vkDestroyCommandPool m_vkDestroyCommandPool;
    PFN_vkDestroyDescriptorPool m_vkDestroyDescriptorPool;
    PFN_vkDestroyDescriptorSetLayout m_vkDestroyDescriptorSetLayout;
    PFN_vkDestroyDescriptorUpdateTemplate m_vkDestroyDescriptorUpdateTemplate;
    PFN_vkDestroyDevice m_vkDestroyDevice;
    PFN_vkDestroyEvent m_vkDestroyEvent;
    PFN_vkDestroyFence m_vkDestroyFence;
    PFN_vkDestroyFramebuffer m_vkDestroyFramebuffer;
    PFN_vkDestroyImage m_vkDestroyImage;
    PFN_vkDestroyImageView m_vkDestroyImageView;
    PFN_vkDestroyInstance m_vkDestroyInstance;
    PFN_vkDestroyPipeline m_vkDestroyPipeline;
    PFN_vkDestroyPipelineCache m_vkDestroyPipelineCache;
    PFN_vkDestroyPipelineLayout m_vkDestroyPipelineLayout;
    PFN_vkDestroyQueryPool m_vkDestroyQueryPool;
    PFN_vkDestroyRenderPass m_vkDestroyRenderPass;
    PFN_vkDestroySampler m_vkDestroySampler;
    PFN_vkDestroySamplerYcbcrConversion m_vkDestroySamplerYcbcrConversion;
    PFN_vkDestroySemaphore m_vkDestroySemaphore;
    PFN_vkDestroyShaderModule m_vkDestroyShaderModule;
    PFN_vkDestroySurfaceKHR m_vkDestroySurfaceKHR;
    PFN_vkDestroySwapchainKHR m_vkDestroySwapchainKHR;
    PFN_vkDeviceWaitIdle m_vkDeviceWaitIdle;
    PFN_vkEndCommandBuffer m_vkEndCommandBuffer;
    PFN_vkEnumerateDeviceExtensionProperties m_vkEnumerateDeviceExtensionProperties;
    PFN_vkEnumerateDeviceLayerProperties m_vkEnumerateDeviceLayerProperties;
    PFN_vkEnumerateInstanceExtensionProperties m_vkEnumerateInstanceExtensionProperties;
    PFN_vkEnumerateInstanceLayerProperties m_vkEnumerateInstanceLayerProperties;
    PFN_vkEnumerateInstanceVersion m_vkEnumerateInstanceVersion;
    PFN_vkEnumeratePhysicalDeviceGroups m_vkEnumeratePhysicalDeviceGroups;
    PFN_vkEnumeratePhysicalDevices m_vkEnumeratePhysicalDevices;
    PFN_vkFlushMappedMemoryRanges m_vkFlushMappedMemoryRanges;
    PFN_vkFreeCommandBuffers m_vkFreeCommandBuffers;
    PFN_vkFreeDescriptorSets m_vkFreeDescriptorSets;
    PFN_vkFreeMemory m_vkFreeMemory;
    PFN_vkGetBufferDeviceAddress m_vkGetBufferDeviceAddress;
    PFN_vkGetBufferMemoryRequirements m_vkGetBufferMemoryRequirements;
    PFN_vkGetBufferMemoryRequirements2 m_vkGetBufferMemoryRequirements2;
    PFN_vkGetBufferOpaqueCaptureAddress m_vkGetBufferOpaqueCaptureAddress;
    PFN_vkGetDescriptorSetLayoutSupport m_vkGetDescriptorSetLayoutSupport;
    PFN_vkGetDeviceGroupPeerMemoryFeatures m_vkGetDeviceGroupPeerMemoryFeatures;
    PFN_vkGetDeviceGroupPresentCapabilitiesKHR m_vkGetDeviceGroupPresentCapabilitiesKHR;
    PFN_vkGetDeviceGroupSurfacePresentModesKHR m_vkGetDeviceGroupSurfacePresentModesKHR;
    PFN_vkGetDeviceMemoryCommitment m_vkGetDeviceMemoryCommitment;
    PFN_vkGetDeviceMemoryOpaqueCaptureAddress m_vkGetDeviceMemoryOpaqueCaptureAddress;
    PFN_vkGetDeviceProcAddr m_vkGetDeviceProcAddr;
    PFN_vkGetDeviceQueue m_vkGetDeviceQueue;
    PFN_vkGetDeviceQueue2 m_vkGetDeviceQueue2;
    PFN_vkGetEventStatus m_vkGetEventStatus;
    PFN_vkGetFenceStatus m_vkGetFenceStatus;
    PFN_vkGetImageMemoryRequirements m_vkGetImageMemoryRequirements;
    PFN_vkGetImageMemoryRequirements2 m_vkGetImageMemoryRequirements2;
    PFN_vkGetImageSparseMemoryRequirements m_vkGetImageSparseMemoryRequirements;
    PFN_vkGetImageSparseMemoryRequirements2 m_vkGetImageSparseMemoryRequirements2;
    PFN_vkGetImageSubresourceLayout m_vkGetImageSubresourceLayout;
    PFN_vkGetPhysicalDeviceExternalBufferProperties m_vkGetPhysicalDeviceExternalBufferProperties;
    PFN_vkGetPhysicalDeviceExternalFenceProperties m_vkGetPhysicalDeviceExternalFenceProperties;
    PFN_vkGetPhysicalDeviceExternalSemaphoreProperties m_vkGetPhysicalDeviceExternalSemaphoreProperties;
    PFN_vkGetPhysicalDeviceFeatures m_vkGetPhysicalDeviceFeatures;
    PFN_vkGetPhysicalDeviceFeatures2 m_vkGetPhysicalDeviceFeatures2;
    PFN_vkGetPhysicalDeviceFormatProperties m_vkGetPhysicalDeviceFormatProperties;
    PFN_vkGetPhysicalDeviceFormatProperties2 m_vkGetPhysicalDeviceFormatProperties2;
    PFN_vkGetPhysicalDeviceImageFormatProperties m_vkGetPhysicalDeviceImageFormatProperties;
    PFN_vkGetPhysicalDeviceImageFormatProperties2 m_vkGetPhysicalDeviceImageFormatProperties2;
    PFN_vkGetPhysicalDeviceMemoryProperties m_vkGetPhysicalDeviceMemoryProperties;
    PFN_vkGetPhysicalDeviceMemoryProperties2 m_vkGetPhysicalDeviceMemoryProperties2;
    PFN_vkGetPhysicalDevicePresentRectanglesKHR m_vkGetPhysicalDevicePresentRectanglesKHR;
    PFN_vkGetPhysicalDeviceProperties m_vkGetPhysicalDeviceProperties;
    PFN_vkGetPhysicalDeviceProperties2 m_vkGetPhysicalDeviceProperties2;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties m_vkGetPhysicalDeviceQueueFamilyProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties2 m_vkGetPhysicalDeviceQueueFamilyProperties2;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties m_vkGetPhysicalDeviceSparseImageFormatProperties;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 m_vkGetPhysicalDeviceSparseImageFormatProperties2;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR m_vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR m_vkGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR m_vkGetPhysicalDeviceSurfacePresentModesKHR;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR m_vkGetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR m_vkGetPhysicalDeviceXcbPresentationSupportKHR;
    PFN_vkGetPipelineCacheData m_vkGetPipelineCacheData;
    PFN_vkGetQueryPoolResults m_vkGetQueryPoolResults;
    PFN_vkGetRenderAreaGranularity m_vkGetRenderAreaGranularity;
    PFN_vkGetSemaphoreCounterValue m_vkGetSemaphoreCounterValue;
    PFN_vkGetSwapchainImagesKHR m_vkGetSwapchainImagesKHR;
    PFN_vkInvalidateMappedMemoryRanges m_vkInvalidateMappedMemoryRanges;
    PFN_vkMapMemory m_vkMapMemory;
    PFN_vkMergePipelineCaches m_vkMergePipelineCaches;
    PFN_vkQueueBindSparse m_vkQueueBindSparse;
    PFN_vkQueuePresentKHR m_vkQueuePresentKHR;
    PFN_vkQueueSubmit m_vkQueueSubmit;
    PFN_vkQueueWaitIdle m_vkQueueWaitIdle;
    PFN_vkResetCommandBuffer m_vkResetCommandBuffer;
    PFN_vkResetCommandPool m_vkResetCommandPool;
    PFN_vkResetDescriptorPool m_vkResetDescriptorPool;
    PFN_vkResetEvent m_vkResetEvent;
    PFN_vkResetFences m_vkResetFences;
    PFN_vkResetQueryPool m_vkResetQueryPool;
    PFN_vkSetEvent m_vkSetEvent;
    PFN_vkSignalSemaphore m_vkSignalSemaphore;
    PFN_vkTrimCommandPool m_vkTrimCommandPool;
    PFN_vkUnmapMemory m_vkUnmapMemory;
    PFN_vkUpdateDescriptorSetWithTemplate m_vkUpdateDescriptorSetWithTemplate;
    PFN_vkUpdateDescriptorSets m_vkUpdateDescriptorSets;
    PFN_vkWaitForFences m_vkWaitForFences;
    PFN_vkWaitSemaphores m_vkWaitSemaphores;

public:
    Result vkAcquireNextImage2KHR(const AcquireNextImageInfoKHR *pAcquireInfo, uint32_t *pImageIndex) const; // NOLINT
    Result vkAcquireNextImageKHR(SwapchainKHR swapchain, uint64_t timeout, Semaphore semaphore, Fence fence, uint32_t *pImageIndex) const; // NOLINT
    Result vkAllocateCommandBuffers(const CommandBufferAllocateInfo *pAllocateInfo, CommandBuffer *pCommandBuffers) const; // NOLINT
    Result vkAllocateDescriptorSets(const DescriptorSetAllocateInfo *pAllocateInfo, DescriptorSet *pDescriptorSets) const; // NOLINT
    Result vkAllocateMemory(const MemoryAllocateInfo *pAllocateInfo, DeviceMemory *pMemory) const; // NOLINT
    Result vkBeginCommandBuffer(CommandBuffer commandBuffer, const CommandBufferBeginInfo *pBeginInfo) const; // NOLINT
    Result vkBindBufferMemory(Buffer buffer, DeviceMemory memory, DeviceSize memoryOffset) const; // NOLINT
    Result vkBindBufferMemory2(uint32_t bindInfoCount, const BindBufferMemoryInfo *pBindInfos) const; // NOLINT
    Result vkBindImageMemory(Image image, DeviceMemory memory, DeviceSize memoryOffset) const; // NOLINT
    Result vkBindImageMemory2(uint32_t bindInfoCount, const BindImageMemoryInfo *pBindInfos) const; // NOLINT
    void vkCmdBeginQuery(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query, QueryControlFlags flags) const; // NOLINT
    void vkCmdBeginRenderPass(CommandBuffer commandBuffer, const RenderPassBeginInfo *pRenderPassBegin, SubpassContents contents) const; // NOLINT
    void vkCmdBeginRenderPass2(CommandBuffer commandBuffer, const RenderPassBeginInfo *pRenderPassBegin, const SubpassBeginInfo *pSubpassBeginInfo) const; // NOLINT
    void vkCmdBeginRenderingKHR(CommandBuffer commandBuffer, const RenderingInfoKHR *pRenderingInfo) const; // NOLINT
    void vkCmdBindDescriptorSets(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, PipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const DescriptorSet *pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t *pDynamicOffsets) const; // NOLINT
    void vkCmdBindIndexBuffer(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, IndexType indexType) const; // NOLINT
    void vkCmdBindPipeline(CommandBuffer commandBuffer, PipelineBindPoint pipelineBindPoint, Pipeline pipeline) const; // NOLINT
    void vkCmdBindVertexBuffers(CommandBuffer commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const Buffer *pBuffers, const DeviceSize *pOffsets) const; // NOLINT
    void vkCmdBlitImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageBlit *pRegions, Filter filter) const; // NOLINT
    void vkCmdClearAttachments(CommandBuffer commandBuffer, uint32_t attachmentCount, const ClearAttachment *pAttachments, uint32_t rectCount, const ClearRect *pRects) const; // NOLINT
    void vkCmdClearColorImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearColorValue *pColor, uint32_t rangeCount, const ImageSubresourceRange *pRanges) const; // NOLINT
    void vkCmdClearDepthStencilImage(CommandBuffer commandBuffer, Image image, ImageLayout imageLayout, const ClearDepthStencilValue *pDepthStencil, uint32_t rangeCount, const ImageSubresourceRange *pRanges) const; // NOLINT
    void vkCmdCopyBuffer(CommandBuffer commandBuffer, Buffer srcBuffer, Buffer dstBuffer, uint32_t regionCount, const BufferCopy *pRegions) const; // NOLINT
    void vkCmdCopyBufferToImage(CommandBuffer commandBuffer, Buffer srcBuffer, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const BufferImageCopy *pRegions) const; // NOLINT
    void vkCmdCopyImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageCopy *pRegions) const; // NOLINT
    void vkCmdCopyImageToBuffer(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Buffer dstBuffer, uint32_t regionCount, const BufferImageCopy *pRegions) const; // NOLINT
    void vkCmdCopyQueryPoolResults(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize stride, QueryResultFlags flags) const; // NOLINT
    void vkCmdDispatch(CommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const; // NOLINT
    void vkCmdDispatchBase(CommandBuffer commandBuffer, uint32_t baseGroupX, uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const; // NOLINT
    void vkCmdDispatchIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset) const; // NOLINT
    void vkCmdDraw(CommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const; // NOLINT
    void vkCmdDrawIndexed(CommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const; // NOLINT
    void vkCmdDrawIndexedIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride) const; // NOLINT
    void vkCmdDrawIndexedIndirectCount(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, Buffer countBuffer, DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const; // NOLINT
    void vkCmdDrawIndirect(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, uint32_t drawCount, uint32_t stride) const; // NOLINT
    void vkCmdDrawIndirectCount(CommandBuffer commandBuffer, Buffer buffer, DeviceSize offset, Buffer countBuffer, DeviceSize countBufferOffset, uint32_t maxDrawCount, uint32_t stride) const; // NOLINT
    void vkCmdEndQuery(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t query) const; // NOLINT
    void vkCmdEndRenderPass(CommandBuffer commandBuffer) const; // NOLINT
    void vkCmdEndRenderPass2(CommandBuffer commandBuffer, const SubpassEndInfo *pSubpassEndInfo) const; // NOLINT
    void vkCmdEndRenderingKHR(CommandBuffer commandBuffer) const; // NOLINT
    void vkCmdExecuteCommands(CommandBuffer commandBuffer, uint32_t commandBufferCount, const CommandBuffer *pCommandBuffers) const; // NOLINT
    void vkCmdFillBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize size, uint32_t data) const; // NOLINT
    void vkCmdNextSubpass(CommandBuffer commandBuffer, SubpassContents contents) const; // NOLINT
    void vkCmdNextSubpass2(CommandBuffer commandBuffer, const SubpassBeginInfo *pSubpassBeginInfo, const SubpassEndInfo *pSubpassEndInfo) const; // NOLINT
    void vkCmdPipelineBarrier(CommandBuffer commandBuffer, PipelineStage srcStageMask, PipelineStage dstStageMask, DependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const MemoryBarrier *pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier *pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier *pImageMemoryBarriers) const; // NOLINT
    void vkCmdPushConstants(CommandBuffer commandBuffer, PipelineLayout layout, ShaderStage stageFlags, uint32_t offset, uint32_t size, const void *pValues) const; // NOLINT
    void vkCmdResetEvent(CommandBuffer commandBuffer, Event event, PipelineStage stageMask) const; // NOLINT
    void vkCmdResetQueryPool(CommandBuffer commandBuffer, QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const; // NOLINT
    void vkCmdResolveImage(CommandBuffer commandBuffer, Image srcImage, ImageLayout srcImageLayout, Image dstImage, ImageLayout dstImageLayout, uint32_t regionCount, const ImageResolve *pRegions) const; // NOLINT
    void vkCmdSetBlendConstants(CommandBuffer commandBuffer, const float blendConstants [4]) const; // NOLINT
    void vkCmdSetDepthBias(CommandBuffer commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) const; // NOLINT
    void vkCmdSetDepthBounds(CommandBuffer commandBuffer, float minDepthBounds, float maxDepthBounds) const; // NOLINT
    void vkCmdSetDeviceMask(CommandBuffer commandBuffer, uint32_t deviceMask) const; // NOLINT
    void vkCmdSetEvent(CommandBuffer commandBuffer, Event event, PipelineStage stageMask) const; // NOLINT
    void vkCmdSetLineWidth(CommandBuffer commandBuffer, float lineWidth) const; // NOLINT
    void vkCmdSetScissor(CommandBuffer commandBuffer, uint32_t firstScissor, uint32_t scissorCount, const Rect2D *pScissors) const; // NOLINT
    void vkCmdSetStencilCompareMask(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t compareMask) const; // NOLINT
    void vkCmdSetStencilReference(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t reference) const; // NOLINT
    void vkCmdSetStencilWriteMask(CommandBuffer commandBuffer, StencilFaceFlags faceMask, uint32_t writeMask) const; // NOLINT
    void vkCmdSetViewport(CommandBuffer commandBuffer, uint32_t firstViewport, uint32_t viewportCount, const Viewport *pViewports) const; // NOLINT
    void vkCmdUpdateBuffer(CommandBuffer commandBuffer, Buffer dstBuffer, DeviceSize dstOffset, DeviceSize dataSize, const void *pData) const; // NOLINT
    void vkCmdWaitEvents(CommandBuffer commandBuffer, uint32_t eventCount, const Event *pEvents, PipelineStage srcStageMask, PipelineStage dstStageMask, uint32_t memoryBarrierCount, const MemoryBarrier *pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const BufferMemoryBarrier *pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const ImageMemoryBarrier *pImageMemoryBarriers) const; // NOLINT
    void vkCmdWriteTimestamp(CommandBuffer commandBuffer, PipelineStage pipelineStage, QueryPool queryPool, uint32_t query) const; // NOLINT
    Result vkCreateBuffer(const BufferCreateInfo *pCreateInfo, Buffer *pBuffer) const; // NOLINT
    Result vkCreateBufferView(const BufferViewCreateInfo *pCreateInfo, BufferView *pView) const; // NOLINT
    Result vkCreateCommandPool(const CommandPoolCreateInfo *pCreateInfo, CommandPool *pCommandPool) const; // NOLINT
    Result vkCreateComputePipelines(PipelineCache pipelineCache, uint32_t createInfoCount, const ComputePipelineCreateInfo *pCreateInfos, Pipeline *pPipelines) const; // NOLINT
    Result vkCreateDescriptorPool(const DescriptorPoolCreateInfo *pCreateInfo, DescriptorPool *pDescriptorPool) const; // NOLINT
    Result vkCreateDescriptorSetLayout(const DescriptorSetLayoutCreateInfo *pCreateInfo, DescriptorSetLayout *pSetLayout) const; // NOLINT
    Result vkCreateDescriptorUpdateTemplate(const DescriptorUpdateTemplateCreateInfo *pCreateInfo, DescriptorUpdateTemplate *pDescriptorUpdateTemplate) const; // NOLINT
    Result vkCreateDevice(const DeviceCreateInfo *pCreateInfo, Device *pDevice) const; // NOLINT
    Result vkCreateEvent(const EventCreateInfo *pCreateInfo, Event *pEvent) const; // NOLINT
    Result vkCreateFence(const FenceCreateInfo *pCreateInfo, Fence *pFence) const; // NOLINT
    Result vkCreateFramebuffer(const FramebufferCreateInfo *pCreateInfo, Framebuffer *pFramebuffer) const; // NOLINT
    Result vkCreateGraphicsPipelines(PipelineCache pipelineCache, uint32_t createInfoCount, const GraphicsPipelineCreateInfo *pCreateInfos, Pipeline *pPipelines) const; // NOLINT
    Result vkCreateImage(const ImageCreateInfo *pCreateInfo, Image *pImage) const; // NOLINT
    Result vkCreateImageView(const ImageViewCreateInfo *pCreateInfo, ImageView *pView) const; // NOLINT
    Result vkCreateInstance(const InstanceCreateInfo *pCreateInfo, Instance *pInstance) const; // NOLINT
    Result vkCreatePipelineCache(const PipelineCacheCreateInfo *pCreateInfo, PipelineCache *pPipelineCache) const; // NOLINT
    Result vkCreatePipelineLayout(const PipelineLayoutCreateInfo *pCreateInfo, PipelineLayout *pPipelineLayout) const; // NOLINT
    Result vkCreateQueryPool(const QueryPoolCreateInfo *pCreateInfo, QueryPool *pQueryPool) const; // NOLINT
    Result vkCreateRenderPass(const RenderPassCreateInfo *pCreateInfo, RenderPass *pRenderPass) const; // NOLINT
    Result vkCreateRenderPass2(const RenderPassCreateInfo2 *pCreateInfo, RenderPass *pRenderPass) const; // NOLINT
    Result vkCreateSampler(const SamplerCreateInfo *pCreateInfo, Sampler *pSampler) const; // NOLINT
    Result vkCreateSamplerYcbcrConversion(const SamplerYcbcrConversionCreateInfo *pCreateInfo, SamplerYcbcrConversion *pYcbcrConversion) const; // NOLINT
    Result vkCreateSemaphore(const SemaphoreCreateInfo *pCreateInfo, Semaphore *pSemaphore) const; // NOLINT
    Result vkCreateShaderModule(const ShaderModuleCreateInfo *pCreateInfo, ShaderModule *pShaderModule) const; // NOLINT
    Result vkCreateSwapchainKHR(const SwapchainCreateInfoKHR *pCreateInfo, SwapchainKHR *pSwapchain) const; // NOLINT
    Result vkCreateXcbSurfaceKHR(const XcbSurfaceCreateInfoKHR *pCreateInfo, SurfaceKHR *pSurface) const; // NOLINT
    void vkDestroyBuffer(Buffer buffer) const; // NOLINT
    void vkDestroyBufferView(BufferView bufferView) const; // NOLINT
    void vkDestroyCommandPool(CommandPool commandPool) const; // NOLINT
    void vkDestroyDescriptorPool(DescriptorPool descriptorPool) const; // NOLINT
    void vkDestroyDescriptorSetLayout(DescriptorSetLayout descriptorSetLayout) const; // NOLINT
    void vkDestroyDescriptorUpdateTemplate(DescriptorUpdateTemplate descriptorUpdateTemplate) const; // NOLINT
    void vkDestroyDevice() const; // NOLINT
    void vkDestroyEvent(Event event) const; // NOLINT
    void vkDestroyFence(Fence fence) const; // NOLINT
    void vkDestroyFramebuffer(Framebuffer framebuffer) const; // NOLINT
    void vkDestroyImage(Image image) const; // NOLINT
    void vkDestroyImageView(ImageView imageView) const; // NOLINT
    void vkDestroyInstance() const; // NOLINT
    void vkDestroyPipeline(Pipeline pipeline) const; // NOLINT
    void vkDestroyPipelineCache(PipelineCache pipelineCache) const; // NOLINT
    void vkDestroyPipelineLayout(PipelineLayout pipelineLayout) const; // NOLINT
    void vkDestroyQueryPool(QueryPool queryPool) const; // NOLINT
    void vkDestroyRenderPass(RenderPass renderPass) const; // NOLINT
    void vkDestroySampler(Sampler sampler) const; // NOLINT
    void vkDestroySamplerYcbcrConversion(SamplerYcbcrConversion ycbcrConversion) const; // NOLINT
    void vkDestroySemaphore(Semaphore semaphore) const; // NOLINT
    void vkDestroyShaderModule(ShaderModule shaderModule) const; // NOLINT
    void vkDestroySurfaceKHR(SurfaceKHR surface) const; // NOLINT
    void vkDestroySwapchainKHR(SwapchainKHR swapchain) const; // NOLINT
    Result vkDeviceWaitIdle() const; // NOLINT
    Result vkEndCommandBuffer(CommandBuffer commandBuffer) const; // NOLINT
    Result vkEnumerateDeviceExtensionProperties(const char *pLayerName, uint32_t *pPropertyCount, ExtensionProperties *pProperties) const; // NOLINT
    Result vkEnumerateDeviceLayerProperties(uint32_t *pPropertyCount, LayerProperties *pProperties) const; // NOLINT
    Result vkEnumerateInstanceExtensionProperties(const char *pLayerName, uint32_t *pPropertyCount, ExtensionProperties *pProperties) const; // NOLINT
    Result vkEnumerateInstanceLayerProperties(uint32_t *pPropertyCount, LayerProperties *pProperties) const; // NOLINT
    Result vkEnumerateInstanceVersion(uint32_t *pApiVersion) const; // NOLINT
    Result vkEnumeratePhysicalDeviceGroups(uint32_t *pPhysicalDeviceGroupCount, PhysicalDeviceGroupProperties *pPhysicalDeviceGroupProperties) const; // NOLINT
    Result vkEnumeratePhysicalDevices(uint32_t *pPhysicalDeviceCount, PhysicalDevice *pPhysicalDevices) const; // NOLINT
    Result vkFlushMappedMemoryRanges(uint32_t memoryRangeCount, const MappedMemoryRange *pMemoryRanges) const; // NOLINT
    void vkFreeCommandBuffers(CommandPool commandPool, uint32_t commandBufferCount, const CommandBuffer *pCommandBuffers) const; // NOLINT
    Result vkFreeDescriptorSets(DescriptorPool descriptorPool, uint32_t descriptorSetCount, const DescriptorSet *pDescriptorSets) const; // NOLINT
    void vkFreeMemory(DeviceMemory memory) const; // NOLINT
    DeviceAddress vkGetBufferDeviceAddress(const BufferDeviceAddressInfo *pInfo) const; // NOLINT
    void vkGetBufferMemoryRequirements(Buffer buffer, MemoryRequirements *pMemoryRequirements) const; // NOLINT
    void vkGetBufferMemoryRequirements2(const BufferMemoryRequirementsInfo2 *pInfo, MemoryRequirements2 *pMemoryRequirements) const; // NOLINT
    uint64_t vkGetBufferOpaqueCaptureAddress(const BufferDeviceAddressInfo *pInfo) const; // NOLINT
    void vkGetDescriptorSetLayoutSupport(const DescriptorSetLayoutCreateInfo *pCreateInfo, DescriptorSetLayoutSupport *pSupport) const; // NOLINT
    void vkGetDeviceGroupPeerMemoryFeatures(uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, PeerMemoryFeature *pPeerMemoryFeatures) const; // NOLINT
    Result vkGetDeviceGroupPresentCapabilitiesKHR(DeviceGroupPresentCapabilitiesKHR *pDeviceGroupPresentCapabilities) const; // NOLINT
    Result vkGetDeviceGroupSurfacePresentModesKHR(SurfaceKHR surface, DeviceGroupPresentModeFlagsKHR *pModes) const; // NOLINT
    void vkGetDeviceMemoryCommitment(DeviceMemory memory, DeviceSize *pCommittedMemoryInBytes) const; // NOLINT
    uint64_t vkGetDeviceMemoryOpaqueCaptureAddress(const DeviceMemoryOpaqueCaptureAddressInfo *pInfo) const; // NOLINT
    PFN_vkVoidFunction vkGetDeviceProcAddr(const char *pName) const; // NOLINT
    void vkGetDeviceQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, Queue *pQueue) const; // NOLINT
    void vkGetDeviceQueue2(const DeviceQueueInfo2 *pQueueInfo, Queue *pQueue) const; // NOLINT
    Result vkGetEventStatus(Event event) const; // NOLINT
    Result vkGetFenceStatus(Fence fence) const; // NOLINT
    void vkGetImageMemoryRequirements(Image image, MemoryRequirements *pMemoryRequirements) const; // NOLINT
    void vkGetImageMemoryRequirements2(const ImageMemoryRequirementsInfo2 *pInfo, MemoryRequirements2 *pMemoryRequirements) const; // NOLINT
    void vkGetImageSparseMemoryRequirements(Image image, uint32_t *pSparseMemoryRequirementCount, SparseImageMemoryRequirements *pSparseMemoryRequirements) const; // NOLINT
    void vkGetImageSparseMemoryRequirements2(const ImageSparseMemoryRequirementsInfo2 *pInfo, uint32_t *pSparseMemoryRequirementCount, SparseImageMemoryRequirements2 *pSparseMemoryRequirements) const; // NOLINT
    void vkGetImageSubresourceLayout(Image image, const ImageSubresource *pSubresource, SubresourceLayout *pLayout) const; // NOLINT
    void vkGetPhysicalDeviceExternalBufferProperties(const PhysicalDeviceExternalBufferInfo *pExternalBufferInfo, ExternalBufferProperties *pExternalBufferProperties) const; // NOLINT
    void vkGetPhysicalDeviceExternalFenceProperties(const PhysicalDeviceExternalFenceInfo *pExternalFenceInfo, ExternalFenceProperties *pExternalFenceProperties) const; // NOLINT
    void vkGetPhysicalDeviceExternalSemaphoreProperties(const PhysicalDeviceExternalSemaphoreInfo *pExternalSemaphoreInfo, ExternalSemaphoreProperties *pExternalSemaphoreProperties) const; // NOLINT
    void vkGetPhysicalDeviceFeatures(PhysicalDeviceFeatures *pFeatures) const; // NOLINT
    void vkGetPhysicalDeviceFeatures2(PhysicalDeviceFeatures2 *pFeatures) const; // NOLINT
    void vkGetPhysicalDeviceFormatProperties(Format format, FormatProperties *pFormatProperties) const; // NOLINT
    void vkGetPhysicalDeviceFormatProperties2(Format format, FormatProperties2 *pFormatProperties) const; // NOLINT
    Result vkGetPhysicalDeviceImageFormatProperties(Format format, ImageType type, ImageTiling tiling, ImageUsage usage, ImageCreateFlags flags, ImageFormatProperties *pImageFormatProperties) const; // NOLINT
    Result vkGetPhysicalDeviceImageFormatProperties2(const PhysicalDeviceImageFormatInfo2 *pImageFormatInfo, ImageFormatProperties2 *pImageFormatProperties) const; // NOLINT
    void vkGetPhysicalDeviceMemoryProperties(PhysicalDeviceMemoryProperties *pMemoryProperties) const; // NOLINT
    void vkGetPhysicalDeviceMemoryProperties2(PhysicalDeviceMemoryProperties2 *pMemoryProperties) const; // NOLINT
    Result vkGetPhysicalDevicePresentRectanglesKHR(SurfaceKHR surface, uint32_t *pRectCount, Rect2D *pRects) const; // NOLINT
    void vkGetPhysicalDeviceProperties(PhysicalDeviceProperties *pProperties) const; // NOLINT
    void vkGetPhysicalDeviceProperties2(PhysicalDeviceProperties2 *pProperties) const; // NOLINT
    void vkGetPhysicalDeviceQueueFamilyProperties(uint32_t *pQueueFamilyPropertyCount, QueueFamilyProperties *pQueueFamilyProperties) const; // NOLINT
    void vkGetPhysicalDeviceQueueFamilyProperties2(uint32_t *pQueueFamilyPropertyCount, QueueFamilyProperties2 *pQueueFamilyProperties) const; // NOLINT
    void vkGetPhysicalDeviceSparseImageFormatProperties(Format format, ImageType type, SampleCount samples, ImageUsage usage, ImageTiling tiling, uint32_t *pPropertyCount, SparseImageFormatProperties *pProperties) const; // NOLINT
    void vkGetPhysicalDeviceSparseImageFormatProperties2(const PhysicalDeviceSparseImageFormatInfo2 *pFormatInfo, uint32_t *pPropertyCount, SparseImageFormatProperties2 *pProperties) const; // NOLINT
    Result vkGetPhysicalDeviceSurfaceCapabilitiesKHR(SurfaceKHR surface, SurfaceCapabilitiesKHR *pSurfaceCapabilities) const; // NOLINT
    Result vkGetPhysicalDeviceSurfaceFormatsKHR(SurfaceKHR surface, uint32_t *pSurfaceFormatCount, SurfaceFormatKHR *pSurfaceFormats) const; // NOLINT
    Result vkGetPhysicalDeviceSurfacePresentModesKHR(SurfaceKHR surface, uint32_t *pPresentModeCount, PresentModeKHR *pPresentModes) const; // NOLINT
    Result vkGetPhysicalDeviceSurfaceSupportKHR(uint32_t queueFamilyIndex, SurfaceKHR surface, Bool *pSupported) const; // NOLINT
    Bool vkGetPhysicalDeviceXcbPresentationSupportKHR(uint32_t queueFamilyIndex, xcb_connection_t *connection, xcb_visualid_t visual_id) const; // NOLINT
    Result vkGetPipelineCacheData(PipelineCache pipelineCache, size_t *pDataSize, void *pData) const; // NOLINT
    Result vkGetQueryPoolResults(QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void *pData, DeviceSize stride, QueryResultFlags flags) const; // NOLINT
    void vkGetRenderAreaGranularity(RenderPass renderPass, Extent2D *pGranularity) const; // NOLINT
    Result vkGetSemaphoreCounterValue(Semaphore semaphore, uint64_t *pValue) const; // NOLINT
    Result vkGetSwapchainImagesKHR(SwapchainKHR swapchain, uint32_t *pSwapchainImageCount, Image *pSwapchainImages) const; // NOLINT
    Result vkInvalidateMappedMemoryRanges(uint32_t memoryRangeCount, const MappedMemoryRange *pMemoryRanges) const; // NOLINT
    Result vkMapMemory(DeviceMemory memory, DeviceSize offset, DeviceSize size, MemoryMapFlags flags, void ** ppData) const; // NOLINT
    Result vkMergePipelineCaches(PipelineCache dstCache, uint32_t srcCacheCount, const PipelineCache *pSrcCaches) const; // NOLINT
    Result vkQueueBindSparse(Queue queue, uint32_t bindInfoCount, const BindSparseInfo *pBindInfo, Fence fence) const; // NOLINT
    Result vkQueuePresentKHR(Queue queue, const PresentInfoKHR *pPresentInfo) const; // NOLINT
    Result vkQueueSubmit(Queue queue, uint32_t submitCount, const SubmitInfo *pSubmits, Fence fence) const; // NOLINT
    Result vkQueueWaitIdle(Queue queue) const; // NOLINT
    Result vkResetCommandBuffer(CommandBuffer commandBuffer, CommandBufferResetFlags flags) const; // NOLINT
    Result vkResetCommandPool(CommandPool commandPool, CommandPoolResetFlags flags) const; // NOLINT
    Result vkResetDescriptorPool(DescriptorPool descriptorPool, DescriptorPoolResetFlags flags) const; // NOLINT
    Result vkResetEvent(Event event) const; // NOLINT
    Result vkResetFences(uint32_t fenceCount, const Fence *pFences) const; // NOLINT
    void vkResetQueryPool(QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount) const; // NOLINT
    Result vkSetEvent(Event event) const; // NOLINT
    Result vkSignalSemaphore(const SemaphoreSignalInfo *pSignalInfo) const; // NOLINT
    void vkTrimCommandPool(CommandPool commandPool, CommandPoolTrimFlags flags) const; // NOLINT
    void vkUnmapMemory(DeviceMemory memory) const; // NOLINT
    void vkUpdateDescriptorSetWithTemplate(DescriptorSet descriptorSet, DescriptorUpdateTemplate descriptorUpdateTemplate, const void *pData) const; // NOLINT
    void vkUpdateDescriptorSets(uint32_t descriptorWriteCount, const WriteDescriptorSet *pDescriptorWrites, uint32_t descriptorCopyCount, const CopyDescriptorSet *pDescriptorCopies) const; // NOLINT
    Result vkWaitForFences(uint32_t fenceCount, const Fence *pFences, Bool waitAll, uint64_t timeout) const; // NOLINT
    Result vkWaitSemaphores(const SemaphoreWaitInfo *pWaitInfo, uint64_t timeout) const; // NOLINT
};

} // namespace vull::vk
