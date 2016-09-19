//==============================================================================
// GraphicsResource
// Reza Nourai, 2016
//==============================================================================
#include "Precomp.h"
#include "Util/Log.h"
#include "GraphicsSystem.h"
#include "GraphicsResource.h"

GraphicsResource::GraphicsResource(const std::shared_ptr<GraphicsSystem>& system)
    : system_(system)
{
}

void GraphicsResource::SetName(const wchar_t* name)
{
    name_.clear();

    if (name)
    {
        name_.assign(name);
    }
}

bool GraphicsResource::GetName(uint32_t size, wchar_t* out_name) const
{
    if (!out_name)
    {
        return false;
    }

    if (size <= name_.size())
    {
        LOG_ERROR(L"Output buffer not big enough.\n");
        return false;
    }

    wcscpy_s(out_name, size, name_.c_str());
    return true;
}
