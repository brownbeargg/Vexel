#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Vex
{
    class PerspectiveCamera
    {
      public:
        PerspectiveCamera(float fov, float ratio, float nearClip, float farClip);

        glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }

        const glm::vec3& GetPosition() const { return m_Position; }
        void SetPosition(const glm::vec3& position) { m_Position = position; }

        const glm::vec3& GetFront() const { return m_Front; }
        void SetFront(const glm::vec3& front) { m_Front = glm::normalize(front); }

        const glm::vec3& GetUp() const { return m_Up; }
        void SetUp(const glm::vec3& up) { m_Up = glm::normalize(up); }

        const glm::mat4& GetProjectionMatrix() const { return m_Projection; }
        void SetProjectionMatrix(const glm::mat4& matrix) { m_Projection = matrix; }

        glm::mat4 GetViewProjectionMatrix() const { return m_Projection * GetViewMatrix(); }

      private:
        glm::mat4 m_Projection{1.0f};

        glm::vec3 m_Position{0.0f};

        glm::vec3 m_Front{0.0f, 0.0f, 1.0f};
        glm::vec3 m_Up{0.0f, 1.0f, 0.0f};

        float m_FOV;
        float m_Ratio;

        float m_NearClip;
        float m_FarClip;
    };
} // namespace Vex
