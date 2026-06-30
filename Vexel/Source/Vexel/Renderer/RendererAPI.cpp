#include "RendererAPI.hpp"

#include "Platform/Vulkan/Renderer/VulkanRendererAPI.hpp"

namespace Vex
{
    void RendererAPI::Init()
    {
        switch (s_GraphicsAPI)
        {
        case GraphicsAPIs::Vulkan:
            return VulkanRendererAPI::Init();
        }

        VEX_RELEASE_ASSERT(false, "Invalid graphics API");
    }

    void RendererAPI::Shutdown()
    {
        switch (s_GraphicsAPI)
        {
        case GraphicsAPIs::Vulkan:
            return VulkanRendererAPI::Shutdown();
        }

        VEX_RELEASE_ASSERT(false, "Invalid graphics API");
    }

    void RendererAPI::BeginFrame(const glm::vec3& clearColor)
    {
        switch (s_GraphicsAPI)
        {
        case GraphicsAPIs::Vulkan:
            VulkanRendererAPI::BeginFrame(clearColor);
        }
    }

    void RendererAPI::EndFrame()
    {
        switch (s_GraphicsAPI)
        {
        case GraphicsAPIs::Vulkan:
            VulkanRendererAPI::EndFrame();
        }
    }
} // namespace Vex
