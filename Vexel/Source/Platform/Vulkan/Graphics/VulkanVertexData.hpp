// WARN: whenever VertexData.hpp changes, this whole file should change

#pragma once

#include "Vexel/Graphics/VertexData.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanVertexData
    {
      public:
        static vk::VertexInputBindingDescription GetVertexBindingDescription()
        {
            vk::VertexInputBindingDescription bindingDescription;

            // TODO: make this be a param
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(VertexData);

            // TODO: make this be a param
            bindingDescription.inputRate = vk::VertexInputRate::eVertex;

            return bindingDescription;
        }

        static std::array<vk::VertexInputAttributeDescription, 2> GetVertexAttributeDescriptions()
        {
            std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions;

            // Position

            attributeDescriptions[0].location = 0;

            // TODO: make this a param
            attributeDescriptions[0].binding = 0;

            attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
            attributeDescriptions[0].offset = 0;

            // Color

            attributeDescriptions[1].location = 1;

            // TODO: make this a param
            attributeDescriptions[1].binding = 0;

            attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
            attributeDescriptions[1].offset = offsetof(VertexData, Color);

            return std::move(attributeDescriptions);
        }
    };
} // namespace Vex
