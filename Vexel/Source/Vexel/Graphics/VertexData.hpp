/*
 * WARN: this file should remain mostly the same because other files rely on it
 * FILES LIST:
 *  -VulkanVertexData.hpp
 */

#pragma once

#include <glm/glm.hpp>

namespace Vex
{
    struct VertexData
    {
        glm::vec3 Position;
        glm::vec3 Color;

        VertexData() = default;
        VertexData(const glm::vec3& position, const glm::vec3& color) : Position(position), Color(color) {}
    };
} // namespace Vex
