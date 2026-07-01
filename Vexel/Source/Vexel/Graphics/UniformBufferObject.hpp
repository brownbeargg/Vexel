#pragma once

#include <glm/glm.hpp>

namespace Vex
{
    struct UniformBufferObject
    {
        alignas(16) glm::mat4 Model{1.0f};
        alignas(16) glm::mat4 View{1.0f};
        alignas(16) glm::mat4 Projection{1.0f};
    };
} // namespace Vex
