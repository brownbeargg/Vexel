#include "Pipeline.hpp"

#include "Platform/Vulkan/VulkanPipeline.hpp"

namespace Vex
{
    Ref<Pipeline> Pipeline::Create(std::string_view vertFilepath, std::string_view fragFilepath)
    {
        switch (RendererAPI::API())
        {
        case GraphicsAPIs::Vulkan:
            return Ref<VulkanPipeline>::Create(vertFilepath, fragFilepath);
        }
    }
} // namespace Vex
