//=============================================================================
// GraphicsBackend
// Reza Nourai, 2016
//=============================================================================

#include "Precomp.h"
#include "GraphicsBackend.h"
#include "D3D12/D3D12GraphicsBackend.h"
#include "Vulkan/VulkanGraphicsBackend.h"

std::shared_ptr<GraphicsBackend> CreateGraphicsBackend(GraphicsBackendType type)
{
    std::shared_ptr<GraphicsBackend> backend;

    switch (type)
    {
    case GraphicsBackendType::D3D12:
        backend = std::make_shared<D3D12GraphicsBackend>();
        break;

    case GraphicsBackendType::Vulkan:
        backend = std::make_shared<VulkanGraphicsBackend>();
        break;

    default:
        assert(false);
        return nullptr;
    }

    backend->InitializeSystem();

    return backend;
}
