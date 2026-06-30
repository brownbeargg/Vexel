#pragma once

#include "Platform/Vulkan/Context/VulkanContext.hpp"
#include "Vexel/Renderer/RendererAPI.hpp"

#include <glm/glm.hpp>

namespace Vex
{
    class VulkanRendererAPI final
    {
      public:
        static void Init();
        static void Shutdown();

        static void BeginFrame(const glm::vec3& clearColor);
        static void EndFrame();
    };
} // namespace Vex
