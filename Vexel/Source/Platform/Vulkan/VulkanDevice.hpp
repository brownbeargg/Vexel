#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanDevice
    {
      public:
        static void Init();
        static void Shutdown();

        static void CreateInstance();
    };
} // namespace Vex
