#include "VulkanShader.hpp"

#include "Platform/Vulkan/Context/VulkanContext.hpp"

namespace Vex
{
    // TODO: Move to VexelUtils
    static std::vector<char> ReadFile(RootDirectory root, const std::filesystem::path& path)
    {
        std::ifstream file(FileSystem::Resolve(root, path), std::ios::ate | std::ios::binary);
        VEX_RELEASE_ASSERT(
            file.is_open(), "Failed to open file: {}", FileSystem::Resolve(root, path).string());

        size_t fileSize = static_cast<size_t>(file.tellg());

        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return std::move(buffer);
    }

    static std::vector<u32> ReadSpirv(RootDirectory root, const std::filesystem::path& path)
    {
        std::vector<char> bytes = ReadFile(root, path);

        VEX_RELEASE_ASSERT(bytes.size() % 4 == 0, "SPIR-V size is not multiple of 4");

        std::vector<u32> words(bytes.size() / 4);
        std::memcpy(words.data(), bytes.data(), bytes.size());
        return std::move(words);
    }

    VulkanShader::VulkanShader(RootDirectory root, const std::filesystem::path& vertexPath,
        const std::filesystem::path& fragmentPath)
    {
        vk::ShaderCreateFlagsEXT flags = vk::ShaderCreateFlagBitsEXT::eLinkStage;
        vk::ShaderStageFlags nextStage = vk::ShaderStageFlagBits::eFragment;
        vk::ShaderCodeTypeEXT codeType = vk::ShaderCodeTypeEXT::eSpirv;

        std::vector<u32> vertSrc = ReadSpirv(root, vertexPath);
        const char* pName = "main";

        vk::ShaderCreateInfoEXT vertexInfo = {};
        vertexInfo.flags = flags;
        vertexInfo.stage = vk::ShaderStageFlagBits::eVertex;
        vertexInfo.nextStage = nextStage;
        vertexInfo.codeType = codeType;
        vertexInfo.codeSize = vertSrc.size() * sizeof(u32);
        vertexInfo.pCode = vertSrc.data();
        vertexInfo.pName = pName;

        std::vector<u32> fragSrc = ReadSpirv(root, fragmentPath);

        vk::ShaderCreateInfoEXT fragmentInfo = {};
        fragmentInfo.flags = flags;
        fragmentInfo.stage = vk::ShaderStageFlagBits::eFragment;
        fragmentInfo.codeType = codeType;
        fragmentInfo.codeSize = fragSrc.size() * sizeof(u32);
        fragmentInfo.pCode = fragSrc.data();
        fragmentInfo.pName = pName;

        std::array<vk::ShaderCreateInfoEXT, 2> shaderInfos = {std::move(vertexInfo), std::move(fragmentInfo)};

        vk::raii::Device& logicalDevice = VulkanContext::QueryLogicalDevice();

        VEX_RELEASE_ASSERT(logicalDevice.getDispatcher(), "Failed to find Vulkan dynamic dispatcher");
        VEX_RELEASE_ASSERT(
            logicalDevice.getDispatcher()->vkCreateShadersEXT, "Failed to find shader create function");

        std::vector<vk::raii::ShaderEXT> result = logicalDevice.createShadersEXT(shaderInfos);

        m_VertexShader = std::move(result[0]);
        m_FragmentShader = std::move(result[1]);
    }
} // namespace Vex
