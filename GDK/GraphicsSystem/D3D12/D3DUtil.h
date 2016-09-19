//==============================================================================
// D3DUtil.h - Utilities for working with the D3D API
// Reza Nourai, 2016
//==============================================================================
#pragma once

#include <d3d12.h>

#include <wrl.h>
using Microsoft::WRL::ComPtr;

#define SAFERELEASE(x) if (x) { x->Release(); x = nullptr; }

#define CHECKHR(hr, format, ...) if (FAILED(hr)) { LOG_ERROR(format, __VA_ARGS__); return false; }
