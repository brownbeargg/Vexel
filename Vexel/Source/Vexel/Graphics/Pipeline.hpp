#pragma once

#include "Vexel/Utils.hpp"

namespace Vex
{
    /// @todo make this pipeline class complete
    class Pipeline
    {
      public:
        Pipeline() = default;
        Pipeline(std::string_view vertFilepath, std::string_view fragFilepath);

        static Pipeline Create(std::string_view vertFilepath, std::string_view fragFilepath)
        {
            return Pipeline(vertFilepath, fragFilepath);
        }

      private:
        /// @todo place this function in VexelUtils Vex::FileSystem
        static std::vector<char> ReadBinary(std::string_view& filepath);
    };
} // namespace Vex
