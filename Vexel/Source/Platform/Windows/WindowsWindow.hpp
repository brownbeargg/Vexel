#pragma once

#include "Platform/Vulkan/Present/VulkanSurface.hpp"
#include "Vexel/Core/Window.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Vex
{
    class WindowsWindow : public Window
    {
      public:
        WindowsWindow(const WindowProps& props);
        ~WindowsWindow();

        void PollEvents() override;

        bool ShouldClose() const override;
        void Close() override;

        u32 GetWidth() const override { return m_Data.Width; }
        u32 GetHeight() const override { return m_Data.Height; }

        void GetFrameBufferSize(int* width, int* height) const override
        {
            glfwGetFramebufferSize(m_Window, width, height);
        }

        void SetVSync(bool enabled) override;
        bool IsVSync() const override { return m_Data.VSync; }

        VkResult CreateSurfaceVulkan(vk::raii::Instance& instance) override;

        void* GetNativeWindow() const override { return m_Window; }
        VulkanSurface& GetSurfaceVulkan() override { return m_Surface; }

        void SetEventCallbackFn(const EventCallbackFn& fn) override { m_Data.EventCallback = fn; }

      private:
        void SetEventCallbacks();

      private:
        struct WindowData
        {
            Observer<Window> Self = {};

            uint32_t Width, Height;
            int XPos, YPos;

            bool Focus;
            bool VSync;
            bool HasTitleBar;
            bool Resizable;

            std::string Title;

            EventCallbackFn EventCallback = nullptr;

            WindowData(const WindowProps& props)
                : Title(props.Title), Width(props.Width), Height(props.Height), XPos(props.XPos),
                  YPos(props.YPos), Focus(props.Focus), HasTitleBar(props.HasTitleBar), VSync(props.VSync)
            {
            }
        };

        WindowData m_Data;

        GLFWwindow* m_Window = nullptr;
        VulkanSurface m_Surface = nullptr;
    };
} // namespace Vex
