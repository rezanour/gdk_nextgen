//=============================================================================
// D3D12GraphicsBackend.h - D3D12 implementation of Graphics Backend
// Reza Nourai, 2016
//=============================================================================
#pragma once

#include "GraphicsBackend/GraphicsBackend.h"

class D3D12GraphicsBackend : public GraphicsBackend
{
public:
    virtual ~D3D12GraphicsBackend();

    virtual bool InitializeSystem() override;
};
