#pragma once

#include "Vexel/Core/Window.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    namespace
    {
        static constexpr u32 s_MaxFramesInFlight = 2;
    }

    class VulkanSwapChain final
    {
      public:
        VulkanSwapChain(vk::raii::Instance& instance, vk::raii::PhysicalDevice& physicalDevice,
            vk::raii::Device& logicalDevice, Observer<Window> pWindow);

        void Invalidate();

        vk::raii::SurfaceKHR& GetSurface() { return m_Surface; }

      private:
        void CreateSurface();
        void CreateSwapChain();
        void CreateImageViews();

      private:
        vk::raii::Instance& m_Instance;
        vk::raii::PhysicalDevice& m_PhysicalDevice;
        vk::raii::Device& m_LogicalDevice;

        Observer<Window> m_pWindow;

        u32 m_FrameIndex = 0;

        vk::raii::SurfaceKHR m_Surface = nullptr;
        vk::SurfaceFormatKHR m_SurfaceFormat;

        vk::PresentModeKHR m_PresentMode;
        vk::Extent2D m_SwapChainExtent;

        vk::raii::SwapchainKHR m_SwapChain = nullptr;

        std::vector<vk::Image> m_Images;
        std::vector<vk::raii::ImageView> m_ImageViews;
    };
} // namespace Vex
