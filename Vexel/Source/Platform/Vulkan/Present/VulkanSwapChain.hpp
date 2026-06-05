#pragma once

#include "Platform/Vulkan/Context/VulkanPhysicalDevice.hpp"

#include "Vexel/Utils.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanLogicalDevice;

    class VulkanSwapChain
    {
      public:
        VulkanSwapChain(nullptr_t) {}

        static Ref<VulkanSwapChain> Create(VulkanPhysicalDevice& pd, VulkanLogicalDevice& device,
            Observer<Window> pWindow, VulkanSurface& surface)
        {
            return Ref<VulkanSwapChain>(new VulkanSwapChain(pd, device, pWindow, surface));
        }

        void RetrieveImages() { m_Images = m_SwapChain.getImages(); }

      private:
        VulkanSwapChain(VulkanPhysicalDevice& pd, VulkanLogicalDevice& device, Observer<Window> pWindow,
            VulkanSurface& surface);

        void ChooseSwapSurfaceFormat(VulkanSurface& surface);
        void ChooseSwapPresentMode(VulkanSurface& surface);
        void ChooseSwapExtent(Observer<Window> pWindow, const vk::SurfaceCapabilitiesKHR& capabilites);
        u32 ChooseMinImageCount(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities);

        void CreateImageViews(VulkanLogicalDevice& device);

      private:
        vk::raii::SwapchainKHR m_SwapChain = nullptr;

        std::vector<vk::Image> m_Images;
        std::vector<vk::raii::ImageView> m_ImageViews;

        vk::SurfaceFormatKHR m_SurfaceFormat;
        vk::PresentModeKHR m_PresentMode;
        vk::Extent2D m_Extent;

        Observer<VulkanPhysicalDevice> m_PhysicalDevice;
    };
} // namespace Vex
