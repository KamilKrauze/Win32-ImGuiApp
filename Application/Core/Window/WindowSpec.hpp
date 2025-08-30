#ifndef WINDOW_SPEC_HPP
#define WINDOW_SPEC_HPP

#include "Core/Platform.hpp"

struct WindowSpec
{
    WindowSpec() = default;
    WindowSpec(const WindowSpec&) = default;
    ~WindowSpec() = default;

public:
#if defined(WINDOWS_NATIVE)
    const wchar_t* title;
#elif defined(CROSS_PLATFORM)
    const char* title;
#endif

    int width = 0;
    int height = 0;
};

#endif