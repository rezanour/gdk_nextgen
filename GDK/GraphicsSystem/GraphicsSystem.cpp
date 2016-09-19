//==============================================================================
// GraphicsSystem
// Reza Nourai, 2016
//==============================================================================
#include "Precomp.h"
#include "Util/Log.h"
#include "GraphicsSystem.h"
#include "GraphicsResource.h"
#include "D3D12/D3D12GraphicsSystem.h"
#include "Vulkan/VulkanGraphicsSystem.h"

GraphicsSystem::GraphicsSystem(const GraphicsSystemCreateParams& params)
    : params_(params)
{
}

std::shared_ptr<GraphicsSystem> CreateGraphicsSystem(const GraphicsSystemCreateParams& params)
{
    std::shared_ptr<GraphicsSystem> system;

    switch (params.Type)
    {
    case GraphicsSystemType::D3D12:
        system = std::make_shared<D3D12GraphicsSystem>(params);
        break;

    case GraphicsSystemType::Vulkan:
        system = std::make_shared<VulkanGraphicsSystem>(params);
        break;

    default:
        assert(false);
        return nullptr;
    }

    if (!system->Initialize())
    {
        system.reset();
    }

    return system;
}
