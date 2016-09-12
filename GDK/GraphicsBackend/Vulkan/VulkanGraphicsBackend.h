//=============================================================================
// VulkanGraphicsBackend.h - Vulkan implementation of Graphics Backend
// Reza Nourai, 2016
//=============================================================================
#pragma once

#include "GraphicsBackend/GraphicsBackend.h"
#include "vulkan/vulkan.h"

class VulkanGraphicsBackend : public GraphicsBackend
{
public:
    virtual ~VulkanGraphicsBackend();

    virtual bool InitializeSystem() override;

private:
    VkInstance instance_ = nullptr;
    VkPhysicalDevice physical_device_ = nullptr;
    VkDevice device_ = nullptr;
};
