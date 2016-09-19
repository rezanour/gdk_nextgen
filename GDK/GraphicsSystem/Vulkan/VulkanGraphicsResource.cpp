//==============================================================================
// VulkanGraphicsResource
// Reza Nourai, 2016
//==============================================================================
#include "Precomp.h"
#include "Util/Log.h"
#include "VulkanGraphicsSystem.h"
#include "VulkanGraphicsResource.h"

VulkanGraphicsResource::VulkanGraphicsResource(const std::shared_ptr<GraphicsSystem>& system)
    : GraphicsResource(system)
{
    assert(system->GetType() == GraphicsSystemType::Vulkan);
}
