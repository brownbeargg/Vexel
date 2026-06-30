#include "VertexBuffer.hpp"

#include "Platform/Vulkan/Graphics/VulkanVertexBuffer.hpp"
#include "Vexel/Renderer/RendererAPI.hpp"

namespace Vex
{
    Ref<VertexBuffer> VertexBuffer::Create(const std::vector<VertexData>& vertices)
    {
        switch (RendererAPI::API())
        {
        case GraphicsAPIs::Vulkan:
            return Ref<VulkanVertexBuffer>::Create(vertices);
        }

        VEX_RELEASE_ASSERT(false, "Unknown renderer API");
        return nullptr;
    }
} // namespace Vex
