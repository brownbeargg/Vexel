#include "Vexis/VexisApplication.hpp"

#include "Vexis/EditorLayer.hpp"

namespace Vex
{
    Application* Application::Create()
    {
        VexisApplication* app = new VexisApplication;
        app->PushLayer(new EditorLayer);
        return std::move(app);
    }

    VexisApplication::VexisApplication() : Application() {}

    VexisApplication::~VexisApplication() {}

    void VexisApplication::OnWindowClose(WindowClosedEvent& e)
    {
        m_Window->Close();
        Close();
    }
} // namespace Vex
