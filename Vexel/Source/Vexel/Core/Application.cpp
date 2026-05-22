#include "Application.hpp"

#include "Vexel/Core/Layer.hpp"
#include "Vexel/Core/Window.hpp"
#include "Vexel/Events/ApplicationEvent.hpp"
#include "Vexel/Misc/DeltaTime.hpp"

namespace Vex
{
    Application::Application()
    {
        VEX_RELEASE_ASSERT(Window::CreateContext(), "Failed to create window context");
    }

    Application::~Application()
    {
        Window::DestroyContext();
    }

    void Application::Run()
    {
        DeltaTime dt = 0;

        while (m_Running)
        {
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(dt);

            OnUpdate();

            PollEvents();
            m_EventBus.Dispatch();
        }
    }

    void Application::ForwardEvent(Scope<Event> e)
    {
        m_EventBus.Queue(std::move(e));
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
