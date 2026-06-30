#pragma once

#include "Vexel/Core/Window.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    static constexpr u32 s_MaxFramesInFlight = 2;

    class VulkanSwapChain final
    {
      public:
        VulkanSwapChain(vk::raii::Instance& instance, vk::raii::PhysicalDevice& physicalDevice,
            vk::raii::Device& logicalDevice, Observer<Window> pWindow);

        void Invalidate();

        vk::raii::SurfaceKHR& GetSurface() { return m_Surface; }

        vk::ImageView GetImageView(u32 imageIndex) { return m_ImageViews[imageIndex]; }
        vk::Image GetImage(u32 imageIndex) { return m_Images[imageIndex]; }
        vk::Extent2D GetExtent() { return m_Extent; }

        static constexpr u32 MaxFramesInFlight() { return s_MaxFramesInFlight; }

        static u32 GetImageCount() { return s_ImageCount; }

        std::pair<vk::Result, uint32_t> AcquireNextImage(vk::Semaphore semaphore)
        {
            return m_SwapChain.acquireNextImage(u64_max, semaphore, nullptr);
        }

        vk::raii::SwapchainKHR& GetVulkanObject() { return m_SwapChain; }

        vk::SurfaceFormatKHR& GetFormat() { return m_SurfaceFormat; }

      private:
        void CreateSurface();
        void CreateSwapChain();
        void CreateImageViews();

        void ChooseImageCount(vk::SurfaceCapabilitiesKHR& surfaceCapabilities);
        void ChooseExtent(vk::SurfaceCapabilitiesKHR& surfaceCapabilities);
        void ChooseFormat();
        void ChoosePresentMode();

      private:
        vk::raii::Instance& m_Instance;
        vk::raii::PhysicalDevice& m_PhysicalDevice;
        vk::raii::Device& m_LogicalDevice;

        Observer<Window> m_pWindow;

        vk::raii::SurfaceKHR m_Surface = nullptr;
        vk::SurfaceFormatKHR m_SurfaceFormat;

        vk::PresentModeKHR m_PresentMode;
        vk::Extent2D m_Extent;

        vk::raii::SwapchainKHR m_SwapChain = nullptr;

        std::vector<vk::Image> m_Images;
        std::vector<vk::raii::ImageView> m_ImageViews;

        static inline u32 s_ImageCount = 0;
    };
} // namespace Vex
