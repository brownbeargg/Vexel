#include "Application.hpp"

#include "Vexel/Core/Layer.hpp"
#include "Vexel/Core/Window.hpp"
#include "Vexel/Events/ApplicationEvent.hpp"

#include "Vexel/ImGui/ImGuiContext.hpp"
#include "Vexel/Renderer/RendererAPI.hpp"

#include "Vexel/Utils.hpp"

namespace Vex
{
    Application::Application()
    {
        VEX_CORE_TRACE("Application initialization");

        m_Window = Window::Create();
        m_Window->SetEventCallbackFn(VEX_BIND_METHOD(ForwardEvent));
        m_EventBus.Observe<WindowClosedEvent>(VEX_BIND_METHOD(OnWindowClose));

        ImGuiContext::CreateContext();
    }

    Application::~Application()
    {
        VEX_CORE_TRACE("Application shutdown");

        ImGuiContext::DestroyContext();
    }

    void Application::Run()
    {
        VEX_CORE_TRACE("Application loop");
        VEX_RELEASE_ASSERT(
            Window::GetNumberOfWindows() > 0, "Must have a window initialized before application loop");

        while (m_Running)
        {
            m_Time.Calculate(m_LastTime);
            m_TimeAccumulator += m_Time.Sec();

            m_Window->PollEvents();
            m_EventBus.Dispatch();

            while (ShouldFixedUpdate())
                for (Layer* layer : m_LayerStack)
                    layer->OnFixedUpdate(m_FixedTimeStep.Sec());

            if ((m_ContinueUnfocused || m_Window->IsFocused()) && !m_Window->IsMinimized())
            {
                RendererAPI::BeginFrame(m_ClearColor);

                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(m_Time);

                ImGuiContext::BeginFrame();
                for (Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
                ImGuiContext::EndFrame();

                RendererAPI::EndFrame();
            }
        }
    }

    bool Application::ShouldFixedUpdate()
    {
        if (m_TimeAccumulator >= m_FixedTimeStep.Sec())
        {
            m_TimeAccumulator -= m_FixedTimeStep.Sec();
            return true;
        }

        return false;
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::PopLayer(Layer* layer)
    {
        m_LayerStack.PopLayer(layer);
        layer->OnDetach();
    }

    void Application::PopOverlay(Layer* overlay)
    {
        m_LayerStack.PopLayer(overlay);
        overlay->OnDetach();
    }

    Application* Application::s_Instance;
} // namespace Vex
