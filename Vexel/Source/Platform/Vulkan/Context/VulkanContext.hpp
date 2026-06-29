#pragma once

#include "Platform/Vulkan/Context/VulkanSwapChain.hpp"
#include "Vexel/Core/Window.hpp"
#include "Vexel/Renderer/RendererContext.hpp"
#include "Vexel/Utils.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    namespace
    {
        struct VulkanContextTypes
        {
            vk::raii::Context Context;
            vk::raii::Instance Instance = nullptr;
            vk::raii::DebugUtilsMessengerEXT DebugMessenger = nullptr;
            vk::raii::PhysicalDevice PhysicalDevice = nullptr;
        };
    } // namespace

    class VulkanContext final : public RendererContext
    {
      public:
        VulkanContext(Observer<Window> pWindow) : m_pWindow(pWindow) {}

        void Init() override;

        static void CreateContext();

        static vk::raii::PhysicalDevice& QueryPhysicalDevice() { return s_Context.PhysicalDevice; }
        static vk::raii::Instance& QueryInstance() { return s_Context.Instance; }
        vk::raii::Device& QueryLogicalDevice() { return m_LogicalDevice; }

        vk::raii::Queue& QueryGraphicsQueue() { return m_GraphicsQueue; }

        static VulkanContextTypes& QueryContextTypes() { return s_Context; }

      private:
        static void CreateInstance();
        static void SetupDebugMessenger();
        static void SelectPhysicalDevice();

        void CreateLogicalDevice();
        vk::DeviceQueueCreateInfo CreateQueue();

      private:
        static inline VulkanContextTypes s_Context;

        vk::raii::Device m_LogicalDevice = nullptr;

        u32 m_GraphicsQueueIndex = u32_max;
        vk::raii::Queue m_GraphicsQueue = nullptr;

        u32 m_TransferQueueIndex = u32_max;
        vk::raii::Queue m_TransferQueue = nullptr;

        Ref<VulkanSwapChain> m_SwapChain;

        Observer<Window> m_pWindow;
    };
} // namespace Vex
