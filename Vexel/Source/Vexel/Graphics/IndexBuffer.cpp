#include "IndexBuffer.hpp"

#include "Platform/Vulkan/Graphics/VulkanIndexBuffer.hpp"
#include "Vexel/Renderer/RendererAPI.hpp"

namespace Vex
{
    Ref<IndexBuffer> IndexBuffer::Create(const std::vector<u32>& indices)
    {
        switch (RendererAPI::API())
        {
        case GraphicsAPIs::Vulkan:
            return Ref<VulkanIndexBuffer>::Create(indices);
        }
    }
} // namespace Vex
