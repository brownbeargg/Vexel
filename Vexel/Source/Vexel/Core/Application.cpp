#include "Application.hpp"

#include "Vexel/Core/Layer.hpp"
#include "Vexel/Misc/DeltaTime.hpp"
#include "Vexel/STL.hpp"

namespace Vex
{
    void Application::Run()
    {
        DeltaTime dt = 0;

        while (m_Running)
        {
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate(dt);
        }
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
