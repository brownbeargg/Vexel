#include "UniformBuffer.hpp"

#include "Vexel/Renderer/RendererAPI.hpp"
#include "Platform/Vulkan/Graphics/VulkanUniformBuffer.hpp"

namespace Vex
{
    Ref<UniformBuffer> UniformBuffer::Create(Ref<Shader> shader)
    {
        switch (RendererAPI::API())
        {
        case GraphicsAPIs::Vulkan:
            return Ref<VulkanUniformBuffer>::Create(shader);
        }

        VEX_RELEASE_ASSERT(false, "Invalid renderer API");
    }
} // namespace Vex
