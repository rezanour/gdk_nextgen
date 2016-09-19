//==============================================================================
// NonCopyable.h - Utility base class for making a derived class non-copyable
// Reza Nourai, 2016
//==============================================================================
#pragma once

class NonCopyable
{
protected:
    NonCopyable() {}

private:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator= (const NonCopyable&) = delete;
};
