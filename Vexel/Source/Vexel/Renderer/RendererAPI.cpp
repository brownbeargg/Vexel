#include "RendererAPI.hpp"

#include "Platform/Vulkan/VulkanRendererAPI.hpp"

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

    void RendererAPI::Shutdown() {}
} // namespace Vex
