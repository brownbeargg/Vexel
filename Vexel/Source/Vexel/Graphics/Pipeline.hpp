#pragma once

#include "Vexel/Renderer/RendererAPI.hpp"
#include "Vexel/Utils.hpp"

namespace Vex
{
    /// @todo make this pipeline class complete
    class Pipeline
    {
      public:
        Pipeline() = default;

        static Ref<Pipeline> Create(std::string_view vertFilepath, std::string_view fragFilepath);
    };
} // namespace Vex
