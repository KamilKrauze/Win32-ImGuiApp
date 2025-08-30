#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#if defined(_WIN32) || defined(_WIN64)
#define WINDOWS_NATIVE
#endif

#if defined(WINDOWS_NATIVE) && defined(CROSS_PLATFORM)
#undef WINDOWS_NATIVE
#endif

#endif