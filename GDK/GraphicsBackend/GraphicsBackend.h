//=============================================================================
// GraphicsBackend.h - Interface that each graphics backend implements so
// it can be used by the engine to provide core services.
// Reza Nourai, 2016
//=============================================================================
#pragma once

#include "Util/NonCopyable.h"

enum class GraphicsBackendType
{
    None = 0,
    D3D12,
    Vulkan
};

class GraphicsBackend : NonCopyable
{
public:
    virtual ~GraphicsBackend() {}

    virtual bool InitializeSystem() = 0;
};

std::shared_ptr<GraphicsBackend> CreateGraphicsBackend(GraphicsBackendType type);
