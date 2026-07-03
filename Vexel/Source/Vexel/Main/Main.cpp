#include "Main.hpp"

#include "Vexel/Core/Application.hpp"
#include "Vexel/Renderer/RendererAPI.hpp"

namespace Vex
{
    void Main::Run(int argc, char* argv[])
    {
        CreateMainContext();

        Application::s_Instance = Application::Create();
        Application::s_Instance->Run();
        delete Application::s_Instance;

        DestroyMainContext();
    }

    void Main::CreateMainContext()
    {
        Log::Init();

        VEX_CORE_TRACE("Context creation");

        FileSystem::Mount(RootDirectory::Binary, FileSystem::GetExecutableDir());

        FileSystem::Mount(RootDirectory::Build, FileSystem::GetBuildDir());

        FileSystem::Mount(
            RootDirectory::Engine, FileSystem::GetProjectRoot(FileSystem::GetExecutableDir()) / "Vexel");

        FileSystem::Mount(RootDirectory::Assets, FileSystem::Resolve(RootDirectory::Build, "Assets"));

        VEX_RELEASE_ASSERT(Window::CreateContext(), "Failed to create window context");

        RendererAPI::Init();
    }

    void Main::DestroyMainContext()
    {
        VEX_CORE_TRACE("Context destruction");

        Window::DestroyContext();
        RendererAPI::Shutdown();
    }
} // namespace Vex

int main(int argc, char* argv[])
{
    try
    {
        Vex::Main::Run(argc, argv);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
