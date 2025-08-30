#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Core/Platform.hpp"

#if defined(WINDOWS_NATIVE)
#include "Core/Window/Platform/Window_Win32.hpp"
#elif defined(CROSS_PLATFORM) && !defined(WINDOWS_NATIVE)
#include "Core/Window/Platform/Window_GLFW.hpp"
#endif

#endif

