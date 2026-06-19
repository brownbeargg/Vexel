#pragma once

#include "Platform/Vulkan/Context/VulkanContext.hpp"
#include "Vexel/Renderer/RendererAPI.hpp"

namespace Vex
{
    class VulkanRendererAPI : public RendererAPI
    {
      public:
        static void Init();
        static void Shutdown();
    };
} // namespace Vex
