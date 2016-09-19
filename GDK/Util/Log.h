//==============================================================================
// Log.h - Utilities for logging information.
// Reza Nourai, 2016
//==============================================================================
#pragma once

#ifdef _DEBUG
#define GDK_LOGGING_ENABLED 1
#else
#define GDK_LOGGING_ENABLED 0
#endif

#if GDK_LOGGING_ENABLED == 1

#define LOG(format, ...) \
{ \
    wchar_t message[512]; \
    swprintf_s(message, format, __VA_ARGS__); \
    OutputDebugString(message); \
}

#else

#define LOG(format, ...)

#endif

#define LOG_INFO(format, ...) LOG(L"INFO: " format, __VA_ARGS__);
#define LOG_WARNING(format, ...) LOG(L"WARNING: " format, __VA_ARGS__); assert(false);
#define LOG_ERROR(format, ...) LOG(L"ERROR: " __FUNCTIONW__ L": " format, __VA_ARGS__); assert(false);
