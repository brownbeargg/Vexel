#include "VulkanRenderCommand.hpp"

#include "Platform/Vulkan/Context/VulkanContext.hpp"

namespace Vex
{
    void VulkanRenderCommand::Init()
    {
        VulkanContext::CreateContext();
    }

    void VulkanRenderCommand::Shutdown() {}
} // namespace Vex
