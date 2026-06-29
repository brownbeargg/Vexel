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
        static void DestroyContext();

        static vk::raii::PhysicalDevice& QueryPhysicalDevice() { return s_Context.PhysicalDevice; }
        static vk::raii::Instance& QueryInstance() { return s_Context.Instance; }
        static vk::raii::Device& QueryLogicalDevice() { return s_LogicalDevice; }

        static vk::raii::Queue& QueryGraphicsQueue() { return s_GraphicsQueue; }
        static vk::raii::Queue& QueryTransferQueue() { return s_TransferQueue; }

        static VulkanContextTypes& QueryContextTypes() { return s_Context; }

      private:
        static void CreateInstance();
        static void SetupDebugMessenger();
        static void SelectPhysicalDevice();

        static void CreateLogicalDevice(Ref<VulkanSwapChain> swapChain);
        static vk::DeviceQueueCreateInfo CreateQueue(Ref<VulkanSwapChain> swapChain);

      private:
        static inline VulkanContextTypes s_Context;

        static inline vk::raii::Device s_LogicalDevice = nullptr;

        static inline u32 s_GraphicsQueueIndex = u32_max;
        static inline vk::raii::Queue s_GraphicsQueue = nullptr;

        static inline u32 s_TransferQueueIndex = u32_max;
        static inline vk::raii::Queue s_TransferQueue = nullptr;

        Ref<VulkanSwapChain> m_SwapChain;

        Observer<Window> m_pWindow;
    };
} // namespace Vex
