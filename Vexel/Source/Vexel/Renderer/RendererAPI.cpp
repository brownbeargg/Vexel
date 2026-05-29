#include "RendererAPI.hpp"

#include "Platform/Vulkan/Renderer/VulkanRendererAPI.hpp"

namespace Vex
{
    void RendererAPI::Init()
    {
        switch (s_GraphicsAPI)
        {
        case GraphicsAPIs::Vulkan:
            VulkanRendererAPI::Init();
            break;
        }
    }

    void RendererAPI::Shutdown()
    {
        switch (s_GraphicsAPI)
        {
        case GraphicsAPIs::Vulkan:
            VulkanRendererAPI::Shutdown();
            break;
        }
    }
} // namespace Vex
