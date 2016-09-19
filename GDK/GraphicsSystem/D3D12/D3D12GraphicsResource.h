//==============================================================================
// D3D12GraphicsResource.h - D3D12 base graphics resource.
// Reza Nourai, 2016
//==============================================================================
#pragma once

#include "../GraphicsResource.h"

class D3D12GraphicsResource : public GraphicsResource
{
public:
    D3D12GraphicsResource(const std::shared_ptr<GraphicsSystem>& system, const ComPtr<ID3D12Resource>& resource);
    virtual ~D3D12GraphicsResource() {}

    virtual void SetName(const wchar_t* name) override;

private:
    ComPtr<ID3D12Resource> resource_;
};
