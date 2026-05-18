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
        PushLayer(new EditorLayer("HELLO WORLD"));
    }

    VexisApplication::~VexisApplication() {}

    void VexisApplication::OnUpdate()
    {
        m_Window->OnUpdate();
    }
} // namespace Vex

int main(int argc, char* argv[])
{
    Vex::Main::Main(argc, argv);
}
