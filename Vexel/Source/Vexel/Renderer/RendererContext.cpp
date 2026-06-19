#include "RendererContext.hpp"

#include "Vexel/Renderer/RendererAPI.hpp"

#include "Platform/Vulkan/Context/VulkanContext.hpp"

namespace Vex
{
    Ref<RendererContext> RendererContext::Create(Observer<Window> pWindow)
    {
        {
            switch (RendererAPI::API())
            {
            case GraphicsAPIs::Vulkan:
                return Ref<VulkanContext>::Create(pWindow);
            }
        }
    }
} // namespace Vex
