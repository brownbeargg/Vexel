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
} // namespace Vex
