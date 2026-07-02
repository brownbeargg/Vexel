#include "WindowsWindow.hpp"

#include "Vexel/Events/ApplicationEvent.hpp"
#include "Vexel/Events/KeyEvent.hpp"
#include "Vexel/Events/MouseEvent.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    static bool s_IsContextInitialized = false;

    WindowsWindow::WindowsWindow(const WindowProps& props) : m_Data(props)
    {
        VEX_RELEASE_ASSERT(s_IsContextInitialized, "Window context is not initialized");

        m_Data.Self = this;

        m_RendererContext = RendererContext::Create(this);

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        glfwWindowHint(GLFW_DECORATED, props.HasTitleBar);
        glfwWindowHint(GLFW_RESIZABLE, props.Resizable);

        m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(m_Window, &m_Data);

        m_RendererContext->Init();
        m_RendererContext->MakeContextCurrent();

        SetEventCallbacks();
    }

    WindowsWindow::~WindowsWindow()
    {
        if (m_Data.Open)
            Close();
    }

    void WindowsWindow::PollEvents()
    {
        glfwPollEvents();
    }

    bool WindowsWindow::ShouldClose() const
    {
        if (m_Data.Open)
            return glfwWindowShouldClose(m_Window);

        return false;
    }

    void WindowsWindow::Close()
    {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
        m_Data.Open = false;

        --s_NumberOfWindows;
        s_WindowInstances.erase(
            std::find(s_WindowInstances.begin(), s_WindowInstances.end(), Observer<Window>::Borrow(this)));
    }

    /// @todo create this function for vulkan
    void WindowsWindow::SetVSync(bool enabled)
    {
        m_Data.VSync = enabled;
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

            /// @todo set swapchain extent width and height when resized
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

            data->EventCallback(Scope<WindowMovedEvent>::Create(data->Self, xPos, yPos));
        });

        glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int iconified)
        {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            data->EventCallback(Scope<WindowMinimizedEvent>::Create(data->Self, iconified));
        });
    }

    Ref<Window> Window::Create(const WindowProps& props)
    {
        auto res = Ref<WindowsWindow>::Create(props);

        ++s_NumberOfWindows;
        s_WindowInstances.push_back(res);

        return std::move(res);
    }

    bool Window::CreateContext()
    {
        s_IsContextInitialized = glfwInit();
        return s_IsContextInitialized;
    }

    void Window::DestroyContext()
    {
        glfwTerminate();
    }
} // namespace Vex
