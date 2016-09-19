//==============================================================================
// GraphicsSystem.h - Base interface that each graphics system implements so
// it can be used by the engine to provide core services.
// Reza Nourai, 2016
//==============================================================================
#pragma once

#include "Util/NonCopyable.h"

class GraphicsResource;

enum class GraphicsSystemType
{
    None = 0,
    D3D12,
    Vulkan
};

struct GraphicsSystemCreateParams
{
    GraphicsSystemType Type;
    bool EnableDebug;

    HINSTANCE Instance;
    HWND Window;

    // Backbuffer dimensions, regardless of window size
    uint32_t BackBufferWidth;
    uint32_t BackBufferHeight;
};

class GraphicsSystem : NonCopyable, public std::enable_shared_from_this<GraphicsSystem>
{
public:
    virtual ~GraphicsSystem() {}

    GraphicsSystemType GetType() const
        { return params_.Type; }

    virtual bool Initialize() = 0;

    // Resource management
    virtual std::shared_ptr<GraphicsResource> CreateTexture2D(uint32_t width, uint32_t height) = 0;

    // Swapchain
    virtual bool Present(bool vsync) = 0;

protected:
    GraphicsSystem(const GraphicsSystemCreateParams& params);

    GraphicsSystemCreateParams params_;
};

std::shared_ptr<GraphicsSystem> CreateGraphicsSystem(const GraphicsSystemCreateParams& params);
