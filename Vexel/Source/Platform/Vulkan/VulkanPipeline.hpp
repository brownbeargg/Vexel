#pragma once

#include "Vexel/Graphics/Pipeline.hpp"

#include <Vexel/Utils.hpp>

namespace Vex
{
    class VulkanPipeline : public Pipeline
    {
      public:
        VulkanPipeline(std::string_view vertFilepath, std::string_view fragFilepath);

        static std::vector<char> ReadSpv(std::string_view& filepath);
    };
} // namespace Vex
