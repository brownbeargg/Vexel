#include "Application.hpp"

#include "Vexel/Core/Layer.hpp"
#include "Vexel/Core/Window.hpp"
#include "Vexel/Events/ApplicationEvent.hpp"

#include "Vexel/Renderer/RendererAPI.hpp"

#include "Vexel/Utils.hpp"

namespace Vex
{
    Application::Application() {}

    Application::~Application() {}

    void Application::Run()
    {
        VEX_CORE_TRACE("Application loop");

        while (m_Running)
        {
            m_Time.Calculate(m_LastTime);
            m_TimeAccumulator += m_Time.Sec();

            while (ShouldFixedUpdate())
                for (Layer* layer : m_LayerStack)
                    layer->OnFixedUpdate(m_FixedTimeStep.Sec());

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(m_Time);

            OnUpdate();

            PollEvents();
            m_EventBus.Dispatch();

            for (Ref<Window> window : GetActiveWindows())
                RendererAPI::SwapBuffers(window.Get());
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
