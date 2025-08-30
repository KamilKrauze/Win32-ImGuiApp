#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Core/Platform.hpp"

#if defined(WINDOWS_NATIVE)
#include "Core/Renderer/Platform/Renderer_Win_OGL.hpp"
#elif defined(CROSS_PLATFORM) && !defined(WINDOWS_NATIVE)
#include "Core/Window/Platform/Renderer_XPlat_OGL.hpp"
#endif

#endif