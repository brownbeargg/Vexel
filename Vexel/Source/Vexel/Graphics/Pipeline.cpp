#include "Pipeline.hpp"

namespace Vex
{
    Pipeline::Pipeline(std::string_view vertFilepath, std::string_view fragFilepath)
    {
        VEX_CORE_INFO("Vertex shader: {}", ReadBinary(vertFilepath).data());
        VEX_CORE_INFO("Fragment shader: {}", ReadBinary(fragFilepath).data());
    }

    std::vector<char> Pipeline::ReadBinary(std::string_view& filepath)
    {
        std::filesystem::path fullPath = FileSystem::Resolve(RootDirectory::Engine, filepath);

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
