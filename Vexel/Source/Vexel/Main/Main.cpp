#include "Main.hpp"

#include "Vexel/Core/Application.hpp"

namespace Vex
{
    void Main::Main(int argc, char* argv[])
    {
        Application* app = Application::Create();

        app->Run();

        delete app;
    }
} // namespace Vex
