#pragma once

#include "Vexel/Utils.hpp"
#include "Platform/Vulkan/Context/VulkanPhysicalDevice.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanSwapChain
    {
      public:
        VulkanSwapChain(nullptr_t) {}

        static Ref<VulkanSwapChain> Create(
            VulkanPhysicalDevice& pd, Observer<Window> pWindow, VulkanSurface& surface)
        {
            return Ref<VulkanSwapChain>(new VulkanSwapChain(pd, pWindow, surface));
        }

      private:
        VulkanSwapChain(VulkanPhysicalDevice& pd, Observer<Window> pWindow, VulkanSurface& surface);

        void ChooseSwapSurfaceFormat(VulkanSurface& surface);
        void ChooseSwapPresentMode(VulkanSurface& surface);
        void ChooseSwapExtent(Observer<Window> pWindow, const vk::SurfaceCapabilitiesKHR& capabilites);
        u32 ChooseMinImageCount(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities);

        void RetrieveImages() { m_SwapChainImages = m_SwapChain.getImages(); }

      private:
        vk::raii::SwapchainKHR m_SwapChain = nullptr;
        std::vector<vk::Image> m_SwapChainImages;

        vk::SurfaceFormatKHR m_SurfaceFormat;
        vk::PresentModeKHR m_PresentMode;
        vk::Extent2D m_Extent;

        Observer<VulkanPhysicalDevice> m_PhysicalDevice;
    };
} // namespace Vex
