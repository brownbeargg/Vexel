#include "Vexis/VexisApplication.hpp"

#include "Vexis/EditorLayer.hpp"

namespace Vex
{
    Application* Application::Create()
    {
        return new VexisApplication;
    }

    VexisApplication::VexisApplication()
    {
        PushLayer(new EditorLayer("HELLO WORLD"));
    }
} // namespace Vex

int main(int argc, char* argv[])
{
    Vex::Main::Main(argc, argv);
}
