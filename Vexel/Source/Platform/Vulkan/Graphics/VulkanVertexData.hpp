// WARN: whenever VertexData.hpp changes, this whole file should change

#pragma once

#include "Vexel/Graphics/VertexData.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanVertexData
    {
        /* From Vulkan
         *
         *typedef struct VkVertexInputBindingDescription {
         *    uint32_t             binding;
         *    uint32_t             stride;
         *    VkVertexInputRate    inputRate;
         *} VkVertexInputBindingDescription;
         */

        vk::VertexInputBindingDescription GetVertexBindingDescription()
        {
            vk::VertexInputBindingDescription bindingDescription;

            // TODO: make this be a param
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(VertexData);

            // TODO: make this be a param
            bindingDescription.inputRate = vk::VertexInputRate::eVertex;

            return bindingDescription;
        }

        std::array<vk::VertexInputAttributeDescription, 2> GetVertexAttributeDescriptions()
        {
            /* From Vulkan
             *
             *typedef struct VkVertexInputAttributeDescription {
             *      uint32_t    location;
             *      uint32_t    binding;
             *      VkFormat    format;
             *      uint32_t    offset;
             *  } VkVertexInputAttributeDescription;
             */

            std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions;

            // Position

            attributeDescriptions[0].location = 0;

            // TODO: make this a param
            attributeDescriptions[0].binding = 0;

            attributeDescriptions[0].format = vk::Format::eR32G32B32A32Sfloat;
            attributeDescriptions[0].offset = 0;

            // Color

            attributeDescriptions[0].location = 1;

            // TODO: make this a param
            attributeDescriptions[0].binding = 0;

            attributeDescriptions[0].format = vk::Format::eR32G32B32A32Sfloat;
            attributeDescriptions[0].offset = 0;

            return std::move(attributeDescriptions);
        }
    };
} // namespace Vex
