#pragma once

#include "Vexel/Renderer/RendererAPI.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanRendererAPI : public RendererAPI
    {
      public:
        static void Init();
        static void Shutdown();

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
            VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

      private:
        static void CreateInstance();
        static void SetupDebugMessenger();
        static void PickPhysicalDevice();
        static void CreateLogicalDevice();

        static std::vector<const char*> GetRequiredInstanceExtensions();

        static std::vector<const char*> SetupRequiredLayers();

      private:
        inline static vk::raii::Context s_Context;
        inline static vk::raii::Instance s_Instance = nullptr;

        inline static vk::raii::DebugUtilsMessengerEXT s_DebugMessenger = nullptr;

        inline static vk::raii::PhysicalDevice s_PhysicalDevice = nullptr;

        inline static vk::raii::Device s_Device = nullptr;
        inline static std::vector<const char*> s_RequiredDeviceExtensions = {vk::KHRSwapchainExtensionName};

        inline static vk::raii::Queue s_GraphicsQueue = nullptr;

        vk::raii::SurfaceKHR surface = nullptr;
    };
} // namespace Vex
