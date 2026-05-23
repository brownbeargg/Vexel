#include "Application.hpp"

#include "Vexel/Core/Layer.hpp"
#include "Vexel/Core/Window.hpp"
#include "Vexel/Events/ApplicationEvent.hpp"

#include "Vexel/Utils.hpp"

namespace Vex
{
    Application::Application()
    {
        Log::Init();
        VEX_RELEASE_ASSERT(Window::CreateContext(), "Failed to create window context");
    }

    Application::~Application()
    {
        Window::DestroyContext();
    }

    void Application::Run()
    {
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
} // namespace Vex
