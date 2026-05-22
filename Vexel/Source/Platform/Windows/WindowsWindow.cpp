#include "WindowsWindow.hpp"

#include <GLFW/glfw3.h>

#include "Vexel/Events/ApplicationEvent.hpp"
#include "Vexel/Events/KeyEvent.hpp"
#include "Vexel/Events/MouseEvent.hpp"

namespace Vex
{
    static bool s_IsInitialized = false;

    WindowsWindow::WindowsWindow(const WindowProps& props) : m_Data(props)
    {
        VEX_RELEASE_ASSERT(s_IsInitialized, "Window context is not initialized");

        glfwWindowHint(GLFW_DECORATED, props.HasTitleBar);

        glfwSwapInterval(props.VSync);
        m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(m_Window, &m_Data);

        SetEventCallbacks();

        ++s_NumberOfWindows;
    }

    WindowsWindow::~WindowsWindow()
    {
        if (m_Window)
            Close();
    }

    void WindowsWindow::OnUpdate()
    {
        glfwSwapBuffers(m_Window);
    }

    void WindowsWindow::PollEvents()
    {
        glfwPollEvents();
    }

    bool WindowsWindow::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void WindowsWindow::Close()
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;

        --s_NumberOfWindows;
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        m_Data.VSync = enabled;
        glfwSwapInterval(enabled);
    }

    void WindowsWindow::SetEventCallbacks()
    {
        glfwSetErrorCallback([](int error, const char* description)
        { VEX_CORE_ERROR("GLFW ERROR ({0}), {1}", error, description); });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
                data->EventCallback(Scope<KeyPressedEvent>::Create(key, 0));
                break;
            case GLFW_REPEAT:
                data->EventCallback(Scope<KeyPressedEvent>::Create(key, 1));
                break;
            case GLFW_RELEASE:
                data->EventCallback(Scope<KeyReleasedEvent>::Create(key));
                break;
            }
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
                data->EventCallback(Scope<MouseButtonPressedEvent>::Create(button));
                break;
            case GLFW_RELEASE:
                data->EventCallback(Scope<MouseButtonReleasedEvent>::Create(button));
                break;
            }
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            data->EventCallback(Scope<MouseMovedEvent>::Create(xPos, yPos));
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            data->EventCallback(Scope<MouseScrolledEvent>::Create(xOffset, yOffset));
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            /// @todo add multiple options so multiple windows are possible
            data->EventCallback(Scope<WindowClosedEvent>::Create(data->Self));
        });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            data->EventCallback(Scope<WindowResizedEvent>::Create(data->Self, width, height));
        });

        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
        {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data->Focus = focused;

            if (focused)
                data->EventCallback(Scope<WindowFocusedEvent>::Create(data->Self));
            else
                data->EventCallback(Scope<WindowLostFocusEvent>::Create(data->Self));
        });

        glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xPos, int yPos)
        {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            data->EventCallback(Scope<WindowMovedEvent>::Create(Weak<Window>(data->Self), xPos, yPos));
        });
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
