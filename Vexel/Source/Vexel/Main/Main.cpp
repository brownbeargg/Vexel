#include "Main.hpp"

#include "Vexel/Core/Application.hpp"
#include "Vexel/Renderer/RendererAPI.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    void Main::Run(int argc, char* argv[])
    {
        InitMainContext();

        Application::s_Instance = Application::Create();
        Application::s_Instance->Run();
        delete Application::s_Instance;

        DestroyMainContext();
    }

    void Main::InitMainContext()
    {
        Log::Init();

        VEX_CORE_TRACE("Application initialization");

        FileSystem::Mount(RootDirectory::Binary, FileSystem::GetExecutableDir());

        FileSystem::Mount(RootDirectory::Build, FileSystem::GetBuildDir());

        FileSystem::Mount(
            RootDirectory::Engine, FileSystem::GetProjectRoot(FileSystem::GetExecutableDir()) / "Vexel");

        VEX_RELEASE_ASSERT(Window::CreateContext(), "Failed to create window context");

        RendererAPI::Init();
    }

    void Main::DestroyMainContext()
    {
        VEX_CORE_TRACE("Application shutdown");

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
        std::cout << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
