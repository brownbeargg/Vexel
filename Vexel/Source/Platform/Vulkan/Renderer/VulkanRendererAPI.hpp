#pragma once

#include "Platform/Vulkan/Context/VulkanContext.hpp"
#include "Platform/Vulkan/Context/VulkanLogicalDevice.hpp"
#include "Vexel/Renderer/RendererAPI.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanRendererAPI : public RendererAPI
    {
      public:
        static void Init();
        static void Shutdown();

      private:
        static inline Ref<VulkanContext> m_Context = nullptr;

        static inline VulkanLogicalDevice m_LogicalDevice = nullptr;
    };
} // namespace Vex
