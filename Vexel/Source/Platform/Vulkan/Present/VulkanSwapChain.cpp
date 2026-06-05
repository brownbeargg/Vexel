#include "VulkanSwapChain.hpp"

namespace Vex
{
    /// @todo be able to have multiple Surfaces
    VulkanSwapChain::VulkanSwapChain(
        VulkanPhysicalDevice& pd, VulkanLogicalDevice& device, Observer<Window> pWindow, VulkanSurface& surface)
        : m_PhysicalDevice(&pd)
    {
        ChooseSwapSurfaceFormat(surface);
        ChooseSwapPresentMode(surface);

        vk::SurfaceCapabilitiesKHR surfaceCapabilities = pd.GetSurfaceCapablities(surface);
        ChooseSwapExtent(pWindow, surfaceCapabilities);

        vk::SwapchainCreateInfoKHR swapChainCreateInfo;
        swapChainCreateInfo.setSurface(surface.Get())
            .setMinImageCount(ChooseMinImageCount(surfaceCapabilities))
            .setImageFormat(m_SurfaceFormat.format)
            .setImageColorSpace(m_SurfaceFormat.colorSpace)
            .setImageExtent(m_Extent)
            .setImageArrayLayers(1)
            .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
            .setImageSharingMode(vk::SharingMode::eExclusive)
            .setPreTransform(surfaceCapabilities.currentTransform)
            .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
            .setPresentMode(m_PresentMode)
            .setClipped(true);

        CreateImageViews(device);
    }

    void VulkanSwapChain::ChooseSwapSurfaceFormat(VulkanSurface& surface)
    {
        std::vector<vk::SurfaceFormatKHR> availableFormats =
            m_PhysicalDevice->AvailableSurfaceFormats(surface);

        VEX_RELEASE_ASSERT(!availableFormats.empty(), "Could NOT find an available format for the swapchain");

        const auto formatIt = std::ranges::find_if(availableFormats,
            [](const vk::SurfaceFormatKHR& format) { return format.format == vk::Format::eB8G8R8A8Srgb; });

        m_SurfaceFormat = formatIt != availableFormats.end() ? availableFormats[0] : *formatIt;
    }

    void VulkanSwapChain::ChooseSwapPresentMode(VulkanSurface& surface)
    {
        std::vector<vk::PresentModeKHR> availablePresentModes =
            m_PhysicalDevice->AvailbleSurfacePresentModes(surface);

        auto m_PresentMode =
            std::ranges::any_of(availablePresentModes, [](const vk::PresentModeKHR& presentMode)
        { return presentMode == vk::PresentModeKHR::eMailbox; })
            ? vk::PresentModeKHR::eMailbox
            : vk::PresentModeKHR::eFifo;
    }

    void VulkanSwapChain::ChooseSwapExtent(
        Observer<Window> pWindow, const vk::SurfaceCapabilitiesKHR& surfaceCapabilities)
    {
        if (surfaceCapabilities.currentExtent.width != std::numeric_limits<u32>::max())
        {
            m_Extent = surfaceCapabilities.currentExtent;
            return;
        }

        int width, height;
        pWindow->GetFrameBufferSize(&width, &height);

        m_Extent = vk::Extent2D{std::clamp<u32>(width, surfaceCapabilities.minImageExtent.width,
                                    surfaceCapabilities.maxImageExtent.width),
            std::clamp<u32>(height, surfaceCapabilities.minImageExtent.height,
                surfaceCapabilities.maxImageExtent.height)};
    }

    u32 VulkanSwapChain::ChooseMinImageCount(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities)
    {
        u32 minImageCount = std::max(3u, surfaceCapabilities.minImageCount);

        if (minImageCount > surfaceCapabilities.maxImageCount)
            minImageCount = surfaceCapabilities.maxImageCount;

        return minImageCount;
    }

    void VulkanSwapChain::CreateImageViews(VulkanLogicalDevice& device)
    {
        VEX_RELEASE_ASSERT(m_ImageViews.empty(), "Image views already exist");

        vk::ImageViewCreateInfo imageViewCreateInfo;
        imageViewCreateInfo.setViewType(vk::ImageViewType::e2D)
            .setFormat(m_SurfaceFormat.format)
            .setSubresourceRange({vk::ImageAspectFlagBits::eColor, 1, 1, 0, 1});

        for (vk::Image& image : m_Images)
        {
            imageViewCreateInfo.setImage(image);
            m_ImageViews.emplace_back(device.Get(), imageViewCreateInfo);
        }
    }
} // namespace Vex
