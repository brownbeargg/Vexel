#include "VulkanRendererAPI.hpp"

#include "Platform/Vulkan/Renderer/VulkanRenderCommand.hpp"

#include "Vexel/Utils.hpp"

namespace Vex
{
    void VulkanRendererAPI::Init()
    {
        VulkanRenderCommand::Init();
    }

    void VulkanRendererAPI::Shutdown()
    {
        VulkanRenderCommand::Shutdown();
    }

    void VulkanRendererAPI::BeginFrame(const glm::vec3& clearColor)
    {
        VulkanRenderCommand::BeginFrame(clearColor);
    }

    void VulkanRendererAPI::EndFrame()
    {
        VulkanRenderCommand::EndFrame();
    }
} // namespace Vex
