#include "UniformBuffer.hpp"

#include "Vexel/Renderer/RendererAPI.hpp"
#include "Platform/Vulkan/Graphics/VulkanUniformBuffer.hpp"

namespace Vex
{
    Ref<UniformBuffer> UniformBuffer::Create()
    {
        switch (RendererAPI::API())
        {
        case GraphicsAPIs::Vulkan:
            return Ref<VulkanUniformBuffer>::Create();
        }

        VEX_RELEASE_ASSERT(false, "Invalid renderer API");
    }
} // namespace Vex
