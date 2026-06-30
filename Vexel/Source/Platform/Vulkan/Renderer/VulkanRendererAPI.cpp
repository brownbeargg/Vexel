#include "VulkanRendererAPI.hpp"

#include "Platform/Vulkan/Renderer/VulkanRenderCommand.hpp"

#include "Vexel/Utils.hpp"

#include "Vexel/Graphics/VertexBuffer.hpp"
#include "Vexel/Graphics/IndexBuffer.hpp"

namespace Vex
{
    void VulkanRendererAPI::DrawIndexed(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer)
    {
        vertexBuffer->Bind();
        indexBuffer->Bind();
        VulkanRenderCommand::DrawIndexed(vertexBuffer, indexBuffer);
    }

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
