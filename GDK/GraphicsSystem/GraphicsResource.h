//==============================================================================
// GraphicsResource.h - Base interface for a resource owned and managed by
// the graphics system.
// Reza Nourai, 2016
//==============================================================================
#pragma once

#include "Util/NonCopyable.h"

class GraphicsSystem;

class GraphicsResource : NonCopyable
{
public:
    virtual ~GraphicsResource() {}

    virtual void SetName(const wchar_t* name);

    bool GetName(uint32_t size, wchar_t* out_name) const;

protected:
    GraphicsResource(const std::shared_ptr<GraphicsSystem>& system);

    std::weak_ptr<GraphicsSystem> system_;

private:
    std::wstring name_;
};
