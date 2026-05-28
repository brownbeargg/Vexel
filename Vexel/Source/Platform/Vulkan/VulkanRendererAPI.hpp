#pragma once

#include "Platform/Vulkan/VulkanDevice.hpp"
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

        static std::vector<const char*> GetRequiredInstanceExtensions();

        static std::vector<const char*> SetupRequiredLayers();

      private:
        inline static vk::raii::Context s_Context;
        inline static vk::raii::Instance s_Instance = nullptr;

        inline static vk::raii::DebugUtilsMessengerEXT s_DebugMessenger = nullptr;
    };
} // namespace Vex
