#include "RendererAPI.hpp"

#include "Platform/Vulkan/Renderer/VulkanRendererAPI.hpp"
#include "Vexel/ImGui/ImGuiContext.hpp"

#include <imgui.h>

namespace Vex
{
    void RendererAPI::DrawIndexed(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer)
    {
        switch (s_GraphicsAPI)
        {
        case GraphicsAPIs::Vulkan:
            return VulkanRendererAPI::DrawIndexed(vertexBuffer, indexBuffer);
        }

        VEX_RELEASE_ASSERT(false, "Invalid graphics API");
    }

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
            return VulkanRendererAPI::BeginFrame(clearColor);
        }

        VEX_RELEASE_ASSERT(false, "Invalid graphics API");
    }

    void RendererAPI::EndFrame()
    {
        switch (s_GraphicsAPI)
        {
        case GraphicsAPIs::Vulkan:
            return VulkanRendererAPI::EndFrame();
        }

        VEX_RELEASE_ASSERT(false, "Invalid graphics API");
    }
} // namespace Vex
