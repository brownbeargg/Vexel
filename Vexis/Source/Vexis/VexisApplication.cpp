#include "Vexis/VexisApplication.hpp"

#include "Vexis/EditorLayer.hpp"

namespace Vex
{
    Application* Application::Create()
    {
        return new VexisApplication;
    }

    VexisApplication::VexisApplication() : Application()
    {
        m_EventBus.Observe<WindowClosedEvent>(VEX_BIND_METHOD(OnWindowClose));

        PushLayer(new EditorLayer());
    }

    VexisApplication::~VexisApplication() {}

    void VexisApplication::OnUpdate() {}

    void VexisApplication::PollEvents()
    {
        m_Window->PollEvents();
    }
} // namespace Vex
