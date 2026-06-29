/*
 * WARN: this file should remain mostly the same because other files rely on it
 * FILES LIST:
 *  -VulkanVertexData.hpp
 */

#pragma once

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>

namespace Vex
{
    struct VertexData
    {
        glm::vec3 Position;
        glm::vec3 Color;
    };
} // namespace Vex
