#pragma once

#include "Platform/Vulkan/Context/VulkanDevice.hpp"
#include "Platform/Vulkan/Context/VulkanInstance.hpp"

#include "Vexel/Utils.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanContext final
    {
      public:
        VulkanContext(nullptr_t) {}

        static Ref<VulkanContext> Create()
        {
            /// @todo make this support multiple windows
            return Ref(new VulkanContext(Window::GetWindowInstances().at(0)));
        }

        vk::raii::Context& Context() { return m_Context; }

        VulkanInstance& GetInstance();

      private:
        VulkanContext(Observer<Window>);

        std::vector<const char*> GetRequiredInstanceExtensions();
        std::vector<const char*> GetRequiredLayers();

      private:
        vk::raii::Context m_Context;

        VulkanInstance m_Instance = nullptr;
        VulkanDevice m_Device = nullptr;
    };
} // namespace Vex
