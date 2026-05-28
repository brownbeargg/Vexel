#pragma once

#include "Platform/Vulkan/VulkanDevice.hpp"
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
        static void CreateInstance();

        static const char** GetRequiredInstanceExtensions(uint32_t& extensionCount);

      private:
        inline static vk::raii::Context s_Context;
        inline static vk::raii::Instance s_Instance = nullptr;

        inline static VulkanDevice s_Device;
    };
} // namespace Vex
