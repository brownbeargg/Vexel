#include "Main.hpp"

#include "Vexel/Core/Application.hpp"

namespace Vex
{
    void Main::Run(int argc, char* argv[])
    {
        Application::s_Instance = Application::Create();

        Application::s_Instance->Run();

        delete Application::s_Instance;
    }
} // namespace Vex

int main(int argc, char* argv[])
{
    try
    {
        Vex::Main::Run(argc, argv);
    }
    catch (std::exception e)
    {
        std::cout << e.what();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
