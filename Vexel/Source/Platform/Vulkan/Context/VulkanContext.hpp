#pragma once

#include "Platform/Vulkan/Context/VulkanDevice.hpp"
#include "Platform/Vulkan/Context/VulkanInstance.hpp"

#include "Vexel/Utils.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanContext
    {
      public:
        VulkanContext(nullptr_t) {}

        static Ref<VulkanContext> Create() { return Ref(new VulkanContext); }

        vk::raii::Context& Context() { return m_Context; }

      private:
        VulkanContext();

        std::vector<const char*> GetRequiredInstanceExtensions();
        std::vector<const char*> GetRequiredLayers();

      private:
        vk::raii::Context m_Context;

        VulkanInstance m_Instance = nullptr;
        VulkanDevice m_Device = nullptr;
    };
} // namespace Vex
