#pragma once

#include "Platform/Vulkan/Context/VulkanInstance.hpp"

#include "Vexel/Core/Window.hpp"
#include "Vexel/Utils.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanSurface final
    {
      public:
        VulkanSurface(nullptr_t) {}

        vk::raii::SurfaceKHR& Get() { return m_Surface; }

      private:
        vk::raii::SurfaceKHR m_Surface = nullptr;
    };
} // namespace Vex
