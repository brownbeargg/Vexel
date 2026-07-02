#include "VulkanRenderCommand.hpp"

#include "Platform/Vulkan/Context/VulkanContext.hpp"

#include "Vexel/Graphics/IndexBuffer.hpp"

namespace Vex
{
    namespace
    {
        void transImgLayout(uint32_t imageIndex, vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
            vk::AccessFlags2 srcAccessMask, vk::AccessFlags2 dstAccessMask,
            vk::PipelineStageFlags2 srcStageMask, vk::PipelineStageFlags2 dstStageMask);
    }

    void VulkanRenderCommand::DrawIndexed(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer)
    {
        VulkanContext::QueryGraphicsCommandBuffer().drawIndexed(indexBuffer->GetIndexCount(), 1, 0, 0, 0);
    }

    void VulkanRenderCommand::Init()
    {
        VulkanContext::CreateContext();
    }

    void VulkanRenderCommand::Shutdown()
    {
        VulkanContext::QueryLogicalDevice().waitIdle();

        s_PresentCompleteSemaphores.clear();
        s_RenderFinishedSemaphores.clear();
        s_InFlightFences.clear();

        VulkanContext::DestroyContext();
    }

    void VulkanRenderCommand::BeginFrame(const glm::vec3& clearColor)
    {
        VulkanContext::IncFrameIndex();
        const u32 frameIndex = VulkanContext::GetFrameIndex();

        vk::Device logicalDevice = VulkanContext::QueryLogicalDevice();

        Ref<VulkanSwapChain> swapChain =
            VulkanContext::GetCurrentContext()->GetRendererContext()->ToVulkanContext()->GetSwapChain();

        // Wait for fence
        vk::Result fenceResult =
            logicalDevice.waitForFences(*s_InFlightFences[frameIndex], vk::True, u64_max);

        VEX_RELEASE_ASSERT(fenceResult == vk::Result::eSuccess, "Failed to wait for vulkan fence");

        // Get next image from swap chain
        auto [result, imageIndex] =
            VulkanContext::GetCurrentVulkanContext()->GetSwapChain()->AcquireNextImage(
                *s_PresentCompleteSemaphores[frameIndex]);

        VEX_RELEASE_ASSERT(result == vk::Result::eSuccess, "Failed to acquire image from swap chain");

        s_ImageIndex = imageIndex;

        vk::CommandBuffer cmdBuf = VulkanContext::QueryGraphicsCommandBuffer();

        logicalDevice.resetFences(*s_InFlightFences[frameIndex]);
        cmdBuf.reset();

        vk::CommandBufferBeginInfo beginInfo = {};
        cmdBuf.begin(beginInfo);

        transImgLayout(s_ImageIndex, vk::ImageLayout::eUndefined, vk::ImageLayout::eColorAttachmentOptimal,
            vk::AccessFlagBits2::eNone, vk::AccessFlagBits2::eColorAttachmentWrite,
            vk::PipelineStageFlagBits2::eTopOfPipe, vk::PipelineStageFlagBits2::eColorAttachmentOutput);

        vk::RenderingAttachmentInfo attachmentInfo = {};
        attachmentInfo.imageView = swapChain->GetImageView(s_ImageIndex);
        attachmentInfo.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
        attachmentInfo.loadOp = vk::AttachmentLoadOp::eClear;
        attachmentInfo.storeOp = vk::AttachmentStoreOp::eStore;
        attachmentInfo.clearValue = vk::ClearColorValue(clearColor.r, clearColor.g, clearColor.b, 1.0f);

        vk::RenderingInfo renderingInfo = {};
        renderingInfo.renderArea = vk::Rect2D{{0, 0}, swapChain->GetExtent()};
        renderingInfo.layerCount = 1;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachments = &attachmentInfo;

        cmdBuf.beginRendering(renderingInfo);

        cmdBuf.setViewport(0,
            vk::Viewport{0.0f, 0.0f, static_cast<f32>(swapChain->GetExtent().width),
                static_cast<f32>(swapChain->GetExtent().height), 0.0f, 1.0f});

        cmdBuf.setScissor(0, vk::Rect2D{vk::Offset2D{0, 0}, swapChain->GetExtent()});
    }

    void VulkanRenderCommand::EndFrame()
    {
        vk::CommandBuffer cmdBuf = VulkanContext::QueryGraphicsCommandBuffer();

        cmdBuf.endRendering();

        transImgLayout(s_ImageIndex, vk::ImageLayout::eColorAttachmentOptimal,
            vk::ImageLayout::ePresentSrcKHR, vk::AccessFlagBits2::eColorAttachmentWrite,
            vk::AccessFlagBits2::eNone, vk::PipelineStageFlagBits2::eColorAttachmentOutput,
            vk::PipelineStageFlagBits2::eBottomOfPipe);

        const u32 frameIndex = VulkanContext::GetFrameIndex();
        vk::raii::Queue& graphicsQueue = VulkanContext::QueryGraphicsQueue();

        cmdBuf.end();

        vk::PipelineStageFlags waitDstStageFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;

        vk::SubmitInfo submitInfo = {};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &*s_PresentCompleteSemaphores[frameIndex];
        submitInfo.pWaitDstStageMask = &waitDstStageFlags;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cmdBuf;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &*s_RenderFinishedSemaphores[s_ImageIndex];

        graphicsQueue.submit(submitInfo, *s_InFlightFences[frameIndex]);

        vk::PresentInfoKHR presentInfo = {};
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &*s_RenderFinishedSemaphores[s_ImageIndex];
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains =
            &*VulkanContext::GetCurrentVulkanContext()->GetSwapChain()->GetVulkanObject();
        presentInfo.pImageIndices = &s_ImageIndex;

        vk::Result result = graphicsQueue.presentKHR(presentInfo);
        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eSuboptimalKHR:
            VEX_CORE_WARN("Got suboptimal present result");
        default:
            break;
        }
    }

    namespace
    {
        void transImgLayout(uint32_t imageIndex, vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
            vk::AccessFlags2 srcAccessMask, vk::AccessFlags2 dstAccessMask,
            vk::PipelineStageFlags2 srcStageMask, vk::PipelineStageFlags2 dstStageMask)
        {
            vk::ImageMemoryBarrier2 barrier = {};
            barrier.srcStageMask = srcStageMask;
            barrier.srcAccessMask = srcAccessMask;
            barrier.dstStageMask = dstStageMask;
            barrier.dstAccessMask = dstAccessMask;
            barrier.oldLayout = oldLayout;
            barrier.newLayout = newLayout;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = VulkanContext::GetCurrentVulkanContext()->GetSwapChain()->GetImage(imageIndex);

            barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;

            vk::DependencyInfo dependencyInfo = {};
            dependencyInfo.dependencyFlags = {};
            dependencyInfo.imageMemoryBarrierCount = 1;
            dependencyInfo.pImageMemoryBarriers = &barrier;

            VulkanContext::QueryGraphicsCommandBuffer().pipelineBarrier2(dependencyInfo);
        }
    } // namespace
} // namespace Vex
