#include "VulkanImguiContext.hpp"

#include "Platform/Vulkan/Renderer/VulkanRenderCommand.hpp"
#include "Vexel/Core/Application.hpp"

#include "Platform/Vulkan/Context/VulkanContext.hpp"
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <imgui.h>
#include <GLFW/glfw3.h>

namespace Vex
{
    void VulkanImguiContext::CreateContext()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.Alpha = 1.0f;

        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        style.Colors[ImGuiCol_ChildBg].w = 1.0f;
        style.Colors[ImGuiCol_PopupBg].w = 1.0f;
        style.Colors[ImGuiCol_FrameBg].w = 1.0f;
        style.Colors[ImGuiCol_TitleBg].w = 1.0f;
        style.Colors[ImGuiCol_TitleBgActive].w = 1.0f;

        GLFWwindow* window =
            (GLFWwindow*)(VulkanContext::GetCurrentVulkanContext()->GetWindow()->GetNativeWindow());

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo info = {};

        VulkanContextTypes& context = VulkanContext::ContextTypes();

        info.Instance = *context.Instance;
        info.PhysicalDevice = *context.PhysicalDevice;
        info.Device = *VulkanContext::LogicalDevice();
        info.QueueFamily = VulkanContext::GetGraphicsQueueIndex();
        info.Queue = *VulkanContext::GraphicsQueue();
        info.PipelineCache = VK_NULL_HANDLE;
        info.MinImageCount = 2;
        info.DescriptorPoolSize = 16;
        info.ImageCount = VulkanSwapChain::GetImageCount();

        info.UseDynamicRendering = true;
        info.PipelineInfoMain.PipelineRenderingCreateInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
        info.PipelineInfoMain.PipelineRenderingCreateInfo.colorAttachmentCount = 1;

        vk::Format* imGuiFormat =
            &VulkanContext::GetCurrentVulkanContext()->GetSwapChain()->GetFormat().format;

        info.PipelineInfoMain.PipelineRenderingCreateInfo.pColorAttachmentFormats = (VkFormat*)&*imGuiFormat;

        ImGui_ImplVulkan_Init(&info);
    }

    void VulkanImguiContext::DestroyContext()
    {
        VulkanContext::LogicalDevice().waitIdle();

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void VulkanImguiContext::BeginFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void VulkanImguiContext::EndFrame()
    {
        ImGui::Render();

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        vk::raii::CommandBuffer& cmdBuf = VulkanContext::GraphicsCommandBuffer();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), *cmdBuf);
    }
} // namespace Vex
