#include "WindowsWindow.hpp"

#include <GLFW/glfw3.h>

namespace Vex
{
    static bool s_IsInitialized = false;

    WindowsWindow::WindowsWindow(const WindowProps& props) : m_Props(props)
    {
        VEX_RELEASE_ASSERT(s_IsInitialized, "Window context is not initialized");

        glfwWindowHint(GLFW_DECORATED, props.HasTitleBar);

        glfwSwapInterval(props.VSync);
        m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(m_Window, &m_Props);

        /// @todo add event callbacks
    }

    WindowsWindow::~WindowsWindow()
    {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        m_Props.VSync = enabled;
        glfwSwapInterval(enabled);
    }

    Ref<Window> Window::Create(const WindowProps& props)
    {
        return Ref<Window>(new WindowsWindow(props));
    }

    bool Window::CreateContext()
    {
        s_IsInitialized = glfwInit();
        return s_IsInitialized;
    }

    void Window::DestroyContext()
    {
        glfwTerminate();
    }
} // namespace Vex
