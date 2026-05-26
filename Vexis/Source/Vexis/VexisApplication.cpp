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
        m_Window = Window::Create();
        m_Window->SetEventCallbackFn(VEX_BIND_METHOD(ForwardEvent));
        m_EventBus.Observe<WindowClosedEvent>(VEX_BIND_METHOD(OnWindowClose));

        PushLayer(new EditorLayer());
    }

    VexisApplication::~VexisApplication() {}

    void VexisApplication::OnUpdate()
    {
        m_Window->OnUpdate();
    }

    void VexisApplication::PollEvents()
    {
        m_Window->PollEvents();
    }
} // namespace Vex

int main(int argc, char* argv[])
{
    try
    {
        Vex::Main::Main(argc, argv);
    }
    catch (std::exception e)
    {
        std::cout << e.what();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
