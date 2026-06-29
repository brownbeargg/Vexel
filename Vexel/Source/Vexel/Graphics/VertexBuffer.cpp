#include "VertexBuffer.hpp"

#include "Platform/Vulkan/Graphics/VulkanVertexBuffer.hpp"

namespace Vex
{
    Ref<VertexBuffer> VertexBuffer::Create()
    {
        switch (RendererAPI::API())
        {
        case GraphicsAPIs::Vulkan:
            Ref<VulkanVertexBuffer>::Create();
        }

        VEX_RELEASE_ASSERT(false, "Unknown renderer API");
        return nullptr;
    }
} // namespace Vex
