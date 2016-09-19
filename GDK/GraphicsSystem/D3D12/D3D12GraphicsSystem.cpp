//==============================================================================
// D3D12GraphicsSystem
// Reza Nourai, 2016
//==============================================================================
#include "Precomp.h"
#include "Util/Log.h"
#include "D3D12GraphicsSystem.h"
#include "D3D12GraphicsResource.h"

D3D12GraphicsSystem::D3D12GraphicsSystem(const GraphicsSystemCreateParams& params)
    : GraphicsSystem(params)
{
}

D3D12GraphicsSystem::~D3D12GraphicsSystem()
{
    swapchain_.Reset();
    queue_.Reset();

    // Before releasing the device, query for debug device. If debug was enabled
    // then this will succeed and allow for printing any remaining references, which 
    // are likely leaks. If debug wasn't enabled, this call will fail
    if (device_)
    {
        ComPtr<ID3D12DebugDevice> debug_device;
        if (SUCCEEDED(device_.As(&debug_device)))
        {
            debug_device->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
        }
    }
    device_.Reset();
}

bool D3D12GraphicsSystem::Initialize()
{
    HRESULT hr = S_OK;

    if (params_.EnableDebug)
    {
        ComPtr<ID3D12Debug> debug;
        hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
        if (SUCCEEDED(hr))
        {
            debug->EnableDebugLayer();
        }
        else
        {
            LOG_WARNING(L"Failed to enable D3D12 debug layer (0x%08x).\n", hr);
        }
    }

    // TODO: Properly enumerate adapter to use

    hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device_));
    CHECKHR(hr, L"Failed to create device (0x%08x).\n", hr);

    D3D12_COMMAND_QUEUE_DESC queue_desc{};
    queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    hr = device_->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&queue_));
    CHECKHR(hr, L"Failed to create command queue (0x%08x).\n", hr);

    return true;
}

std::shared_ptr<GraphicsResource> D3D12GraphicsSystem::CreateTexture2D(uint32_t width, uint32_t height)
{
    D3D12_RESOURCE_DESC rd{};
    rd.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    rd.Width = width;
    rd.Height = height;
    rd.DepthOrArraySize = 1;
    rd.MipLevels = 1;
    rd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rd.SampleDesc.Count = 1;
    rd.Layout = D3D12_TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE;

    ComPtr<ID3D12Resource> resource;
    HRESULT hr = device_->CreateReservedResource(&rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource));
    if (FAILED(hr))
    {
        LOG_ERROR(L"Failed to create resource (0x%08x).\n", hr);
        return nullptr;
    }

    return std::make_shared<D3D12GraphicsResource>(shared_from_this(), resource);
}

bool D3D12GraphicsSystem::Present(bool vsync)
{
    HRESULT hr = swapchain_->Present(vsync ? 1 : 0, 0);
    CHECKHR(hr, L"Failed to present (0x%08x).\n", hr);

    return true;
}
