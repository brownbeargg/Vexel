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

    VexisApplication::VexisApplication() : Application()
    {
        m_EventBus.Observe<WindowClosedEvent>(VEX_BIND_METHOD(OnWindowClose));

        for (u32 i{}; i < 1; ++i)
        {
            m_Windows.push_back(Window::Create());
            m_Windows.back()->SetEventCallbackFn(VEX_BIND_METHOD(ForwardEvent));
        }
    }

    VexisApplication::~VexisApplication() {}

    void VexisApplication::OnUpdate() {}

    void VexisApplication::PollEvents()
    {
        for (Ref<Window> pWindow : GetActiveWindows())
            if (pWindow->IsOpen())
                pWindow->PollEvents();
    }

    void VexisApplication::OnWindowClose(WindowClosedEvent& e)
    {
        for (u32 i{}; i < Window::GetNumberOfWindows(); ++i)
        {
            while (i < m_Windows.size() && !m_Windows[i]->IsOpen())
                ++i;

            if (m_Windows[i] == e.GetWindow())
                m_Windows[i]->Close();
        }

        if (Window::GetNumberOfWindows() == 0)
            Close();
    }
} // namespace Vex
