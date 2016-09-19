//==============================================================================
// D3D12GraphicsSystem.h - D3D12 implementation of GraphicsSystem interface
// Reza Nourai, 2016
//==============================================================================
#pragma once

#include "GraphicsSystem/GraphicsSystem.h"
#include "D3DUtil.h"

class D3D12GraphicsSystem : public GraphicsSystem
{
public:
    D3D12GraphicsSystem(const GraphicsSystemCreateParams& params);
    virtual ~D3D12GraphicsSystem();

    virtual bool Initialize() override;

    // Resource management
    virtual std::shared_ptr<GraphicsResource> CreateTexture2D(uint32_t width, uint32_t height) override;

    // Swapchain
    virtual bool Present(bool vsync) override;

private:
    ComPtr<ID3D12Device> device_;
    ComPtr<ID3D12CommandQueue> queue_;
    ComPtr<IDXGISwapChain> swapchain_;
};
