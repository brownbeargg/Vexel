#include "VulkanRendererAPI.hpp"

#include "Vexel/Utils.hpp"

#include <GLFW/glfw3.h>

namespace Vex
{
    void VulkanRendererAPI::Init()
    {
        m_Context = VulkanContext::Create();
    }

    void VulkanRendererAPI::Shutdown() {}
} // namespace Vex
