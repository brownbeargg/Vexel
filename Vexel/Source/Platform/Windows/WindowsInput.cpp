#include "Vexel/Core/Input.hpp"

#include "Vexel/Core/Application.hpp"
#include <GLFW/glfw3.h>

namespace Vex
{
    // TODO: Make window listened to not hardcoded
    bool Input::IsKeyPressed(Key::Key key)
    {
        int state =
            glfwGetKey((GLFWwindow*)Application::Instance()->GetActiveWindows()[0]->GetNativeWindow(), key);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    // TODO: Make window listened to not hardcoded
    bool Input::IsMouseButtonPressed(Mouse::Mouse button)
    {
        int state = glfwGetMouseButton(
            (GLFWwindow*)Application::Instance()->GetActiveWindows()[0]->GetNativeWindow(), (int)button);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    // TODO: Make window listened to not hardcoded
    glm::vec2 Input::GetMousePos()
    {
        double xPos, yPos;
        glfwGetCursorPos(
            (GLFWwindow*)Application::Instance()->GetActiveWindows()[0]->GetNativeWindow(), &xPos, &yPos);
        return glm::vec2(xPos, yPos);
    }
} // namespace Vex
