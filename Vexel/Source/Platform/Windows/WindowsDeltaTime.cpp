#include "Vexel/Core/DeltaTime.hpp"

#include <GLFW/glfw3.h>

namespace Vex
{
    /// @todo implement this function
    void DeltaTime::Calculate(float& last)
    {
        float current = glfwGetTime();
        m_Time = current - last;
        last = current;
    }
} // namespace Vex
