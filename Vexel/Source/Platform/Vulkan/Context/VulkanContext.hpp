#pragma once

#include "Platform/Vulkan/Context/VulkanSwapChain.hpp"
#include "Vexel/Core/Window.hpp"
#include "Vexel/Renderer/RendererContext.hpp"
#include "Vexel/Utils.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    struct VulkanContextTypes
    {
        vk::raii::Context Context;
        vk::raii::Instance Instance = nullptr;
        vk::raii::DebugUtilsMessengerEXT DebugMessenger = nullptr;
        vk::raii::PhysicalDevice PhysicalDevice = nullptr;
    };

    class VulkanContext final : public RendererContext
    {
      public:
        VulkanContext(Observer<Window> pWindow) : m_pWindow(pWindow) {}
        ~VulkanContext() { s_LogicalDevice.waitIdle(); }

        void Init() override;

        static void CreateContext();
        static void DestroyContext();

        static vk::raii::PhysicalDevice& QueryPhysicalDevice() { return s_Context.PhysicalDevice; }
        static vk::raii::Instance& QueryInstance() { return s_Context.Instance; }
        static vk::raii::Device& QueryLogicalDevice() { return s_LogicalDevice; }

        static vk::raii::Queue& QueryGraphicsQueue() { return s_GraphicsQueue; }
        static vk::raii::CommandPool& QueryGraphicsCommandPool() { return s_GraphicsCommandPool; }
        static vk::raii::CommandBuffer& QueryGraphicsCommandBuffer()
        {
            return s_GraphicsCommandBuffers[s_CurrentFrameIndex];
        }

        static vk::raii::Queue& QueryTransferQueue() { return s_TransferQueue; }
        static vk::raii::CommandPool& QueryTransferCommandPool() { return s_TransferCommandPool; }
        static vk::raii::CommandBuffer& QueryTransferCommandBuffer() { return s_TransferCommandBuffer; }

        static VulkanContextTypes& QueryContextTypes() { return s_Context; }

        void MakeContextCurrent() override { s_CurrentContext = m_pWindow; }
        static Observer<Window> GetCurrentContext() { return s_CurrentContext; }

        static Observer<VulkanContext> GetCurrentVulkanContext()
        {
            return (VulkanContext*)(s_CurrentContext->GetRendererContext().Get());
        }

        Ref<VulkanSwapChain> GetSwapChain() { return m_SwapChain; }

        static u32 GetFrameIndex() { return s_CurrentFrameIndex; }
        static void IncFrameIndex()
        {
            s_CurrentFrameIndex = (s_CurrentFrameIndex + 1) % VulkanSwapChain::MaxFramesInFlight();
        };

      private:
        static void CreateInstance();
        static void SetupDebugMessenger();
        static void SelectPhysicalDevice();

        static void CreateLogicalDevice(Ref<VulkanSwapChain> swapChain);
        static std::vector<vk::DeviceQueueCreateInfo> CreateQueue(Ref<VulkanSwapChain> swapChain);

        static void CreateCommandPools();
        static void CreateCommandBuffers();

        static void CreateSyncObjects();

        static void CreateDescriptorPool();
        static void CreateDescriptorSets();

      private:
        static inline VulkanContextTypes s_Context;

        static inline vk::raii::Device s_LogicalDevice = nullptr;

        static inline u32 s_GraphicsQueueIndex = u32_max;
        static inline vk::raii::Queue s_GraphicsQueue = nullptr;
        static inline vk::raii::CommandPool s_GraphicsCommandPool = nullptr;
        static inline std::vector<vk::raii::CommandBuffer> s_GraphicsCommandBuffers;

        static inline u32 s_TransferQueueIndex = u32_max;
        static inline vk::raii::Queue s_TransferQueue = nullptr;
        static inline vk::raii::CommandPool s_TransferCommandPool = nullptr;
        static inline vk::raii::CommandBuffer s_TransferCommandBuffer = nullptr;

        static inline Observer<Window> s_CurrentContext;

        Ref<VulkanSwapChain> m_SwapChain;

        Observer<Window> m_pWindow;

        static inline u32 s_CurrentFrameIndex = 0;
    };
} // namespace Vex
