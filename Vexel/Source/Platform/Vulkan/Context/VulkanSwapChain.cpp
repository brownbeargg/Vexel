#include "VulkanSwapChain.hpp"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "Platform/Vulkan/Context/VulkanContext.hpp"

namespace Vex
{
    VulkanSwapChain::VulkanSwapChain(vk::raii::Instance& instance, vk::raii::PhysicalDevice& physicalDevice,
        vk::raii::Device& logicalDevice, Observer<Window> pWindow)
        : m_Instance(instance), m_PhysicalDevice(physicalDevice), m_LogicalDevice(logicalDevice),
          m_pWindow(pWindow)
    {
        VEX_RELEASE_ASSERT(glfwVulkanSupported(), "GLFW error: Vulkan is not supported");
        CreateSurface();
    }

    void VulkanSwapChain::Invalidate()
    {
        m_ImageViews.clear();
        m_SwapChain = nullptr;

        try
        {
            CreateSwapChain();
            CreateImageViews();
        }
        catch (const vk::SurfaceLostKHRError& e)
        {
        }
    }

    void VulkanSwapChain::CreateSurface()
    {
        VkSurfaceKHR rawSurface;
        VEX_VERIFY(glfwCreateWindowSurface(*m_Instance,
                       static_cast<GLFWwindow*>(m_pWindow->GetNativeWindow()), nullptr, &rawSurface),
            "Failed to create window surface");

        m_Surface = vk::raii::SurfaceKHR{m_Instance, rawSurface};
    }

    void VulkanSwapChain::CreateSwapChain()
    {
        vk::SurfaceCapabilitiesKHR surfaceCapabilities =
            m_PhysicalDevice.getSurfaceCapabilitiesKHR(m_Surface);

        if (!s_ImageCount)
            ChooseImageCount(surfaceCapabilities);

        ChooseExtent(surfaceCapabilities);
        ChooseFormat();
        ChoosePresentMode();

        // Create swap chain
        vk::SwapchainCreateInfoKHR createInfo = {};
        createInfo.surface = *m_Surface;
        createInfo.minImageCount = s_ImageCount;
        createInfo.imageFormat = m_SurfaceFormat.format;
        createInfo.imageColorSpace = m_SurfaceFormat.colorSpace;
        createInfo.imageExtent = m_Extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        createInfo.preTransform = surfaceCapabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = m_PresentMode;
        createInfo.clipped = true;

        m_SwapChain = vk::raii::SwapchainKHR{m_LogicalDevice, createInfo};
        m_Images = m_SwapChain.getImages();
    }

    void VulkanSwapChain::CreateImageViews()
    {
        VEX_RELEASE_ASSERT(m_ImageViews.empty(), "Swap chain image views already contains elements");

        vk::ImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
        imageViewCreateInfo.format = m_SurfaceFormat.format;
        imageViewCreateInfo.subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1};

        imageViewCreateInfo.components = {vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity,
            vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity};

        for (vk::Image& image : m_Images)
        {
            imageViewCreateInfo.image = image;
            m_ImageViews.emplace_back(m_LogicalDevice, imageViewCreateInfo);
        }
    }

    void VulkanSwapChain::ChooseImageCount(vk::SurfaceCapabilitiesKHR& surfaceCapabilities)
    {
        u32 minImageCount = std::max(3u, surfaceCapabilities.minImageCount);

        if ((0 < surfaceCapabilities.maxImageCount) && (surfaceCapabilities.maxImageCount < minImageCount))
            minImageCount = surfaceCapabilities.maxImageCount;

        s_ImageCount = minImageCount;
    }

    void VulkanSwapChain::ChooseExtent(vk::SurfaceCapabilitiesKHR& surfaceCapabilities)
    {
        if (surfaceCapabilities.currentExtent.width != std::numeric_limits<u32>::max())
            m_Extent = surfaceCapabilities.currentExtent;
        else
        {
            int width, height;
            glfwGetFramebufferSize(static_cast<GLFWwindow*>(m_pWindow->GetNativeWindow()), &width, &height);

            m_Extent = vk::Extent2D{std::clamp<u32>(width, surfaceCapabilities.minImageExtent.width,
                                        surfaceCapabilities.maxImageExtent.width),
                std::clamp<u32>(height, surfaceCapabilities.minImageExtent.height,
                    surfaceCapabilities.maxImageExtent.height)};
        }
    }

    void VulkanSwapChain::ChooseFormat()
    {
        std::vector<vk::SurfaceFormatKHR> availableFormats = m_PhysicalDevice.getSurfaceFormatsKHR(m_Surface);

        auto formatIt = std::ranges::find_if(availableFormats, [](const auto& format)
        {
            return format.format == vk::Format::eB8G8R8A8Unorm &&
                format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
        });

        m_SurfaceFormat = formatIt != availableFormats.end() ? *formatIt : availableFormats[0];
    }

    void VulkanSwapChain::ChoosePresentMode()
    {
        std::vector<vk::PresentModeKHR> availablePresentModes =
            m_PhysicalDevice.getSurfacePresentModesKHR(m_Surface);

        VEX_RELEASE_ASSERT(std::ranges::any_of(availablePresentModes,
                               [](auto presentMode) { return presentMode == vk::PresentModeKHR::eFifo; }),
            "Required present mode not available");

        m_PresentMode = std::ranges::any_of(availablePresentModes, [](const vk::PresentModeKHR value)
        { return vk::PresentModeKHR::eMailbox == value; })
            ? vk::PresentModeKHR::eMailbox
            : vk::PresentModeKHR::eFifo;
    }
} // namespace Vex
