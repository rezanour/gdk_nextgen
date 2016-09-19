//==============================================================================
// VulkanGraphicsResource.h - Vulkan base graphics resource.
// Reza Nourai, 2016
//==============================================================================
#pragma once

#include "../GraphicsResource.h"

class VulkanGraphicsResource : public GraphicsResource
{
public:
    VulkanGraphicsResource(const std::shared_ptr<GraphicsSystem>& system);
    virtual ~VulkanGraphicsResource() {}
};
