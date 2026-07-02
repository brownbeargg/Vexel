#include "ImGuiContext.hpp"

#include "Platform/Vulkan/Imgui/VulkanImguiContext.hpp"
#include "Vexel/Renderer/RendererAPI.hpp"

namespace Vex
{
    void ImGuiContext::CreateContext()
    {
        switch (RendererAPI::API())
        {
        case GraphicsAPIs::Vulkan:
            return VulkanImguiContext::CreateContext();
        }
    }

    void ImGuiContext::DestroyContext()
    {
        switch (RendererAPI::API())
        {
        case GraphicsAPIs::Vulkan:
            return VulkanImguiContext::DestroyContext();
        }
    }

    void ImGuiContext::BeginFrame()
    {
        switch (RendererAPI::API())
        {
        case GraphicsAPIs::Vulkan:
            return VulkanImguiContext::BeginFrame();
        }
    }

    void ImGuiContext::EndFrame()
    {
        switch (RendererAPI::API())
        {
        case GraphicsAPIs::Vulkan:
            return VulkanImguiContext::EndFrame();
        }
    }
} // namespace Vex
