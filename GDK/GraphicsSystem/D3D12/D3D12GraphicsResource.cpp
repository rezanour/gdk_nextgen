//==============================================================================
// D3D12GraphicsResource
// Reza Nourai, 2016
//==============================================================================
#include "Precomp.h"
#include "Util/Log.h"
#include "D3D12GraphicsSystem.h"
#include "D3D12GraphicsResource.h"

D3D12GraphicsResource::D3D12GraphicsResource(const std::shared_ptr<GraphicsSystem>& system, const ComPtr<ID3D12Resource>& resource)
    : GraphicsResource(system)
    , resource_(resource)
{
    assert(system->GetType() == GraphicsSystemType::D3D12);
}

void D3D12GraphicsResource::SetName(const wchar_t* name)
{
    GraphicsResource::SetName(name);
    resource_->SetName(name);
}
