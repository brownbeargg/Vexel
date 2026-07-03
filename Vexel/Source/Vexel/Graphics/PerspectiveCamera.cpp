#include "PerspectiveCamera.hpp"

namespace Vex
{
    PerspectiveCamera::PerspectiveCamera(float fov, float ratio, float nearClip, float farClip)
        : m_FOV(fov), m_Ratio(ratio), m_NearClip(nearClip), m_FarClip(farClip)
    {
        m_Projection = glm::perspective(glm::radians(m_FOV), m_Ratio, m_NearClip, m_FarClip);
    }
} // namespace Vex
