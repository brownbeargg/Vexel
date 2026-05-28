#include "VulkanPipeline.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    VulkanPipeline::VulkanPipeline(std::string_view vertFilepath, std::string_view fragFilepath)
    {
        VEX_CORE_INFO("Vertex shader size : {}", ReadSpv(vertFilepath).size());
        VEX_CORE_INFO("Fragment shader size : {}", ReadSpv(fragFilepath).size());
    }

    /// @todo add this to vexelutils filesystem
    std::vector<char> VulkanPipeline::ReadSpv(std::string_view& filepath)
    {
        /// @todo make this not be engine specific
        std::filesystem::path fullPath = FileSystem::Resolve(RootDirectory::Build, filepath);

        std::ifstream file(fullPath, std::ios::ate | std::ios::binary);
        VEX_RELEASE_ASSERT(file.is_open(), "Failed to open file: {}", fullPath.string());

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }
} // namespace Vex
