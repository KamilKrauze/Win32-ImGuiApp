#include "Renderer_Win_OGL.hpp"

#include <iostream>

#include "GL/GL.h"

Renderer::Renderer(Window* windowRef)
{
    m_WindowReference = windowRef;
}

bool Renderer::Create()
{
    if (!CreateDeviceWGL())
    {
        CleanupDeviceWGL();
        m_WindowReference->Destroy();
        return false;
    }
    MakeAsCurrentContext();
    return true;
}

bool Renderer::Destroy()
{
    CleanupDeviceWGL();
    wglDeleteContext(m_RenderContextHandle);
    m_WindowReference = nullptr;
    return true;
}

void Renderer::Draw()
{
    ::SwapBuffers(m_DeviceContext.handle);
}

void Renderer::ClearFrameBuffer()
{
    auto [width, height] = m_WindowReference->GetWindowSize();
    glViewport(0, 0, width, height);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::MakeAsCurrentContext() const
{
    wglMakeCurrent(m_DeviceContext.handle, m_RenderContextHandle);
}

HGLRC& Renderer::GetRenderContextRef()
{
    return m_RenderContextHandle;
}

DeviceContext& Renderer::GetDeviceContextRef()
{
    return m_DeviceContext;
}

bool Renderer::CreateDeviceWGL()
{
    auto& nativeWindow = m_WindowReference->GetNativeWindow();
    
    HDC hDc = ::GetDC(nativeWindow);
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(nativeWindow, hDc);

    m_DeviceContext.handle = ::GetDC(nativeWindow);
    if (!m_RenderContextHandle)
        m_RenderContextHandle = wglCreateContext(m_DeviceContext.handle);
    return true;
}

void Renderer::CleanupDeviceWGL() const
{
    wglMakeCurrent(nullptr, nullptr);
    ::ReleaseDC(m_WindowReference->GetNativeWindow(), m_DeviceContext.handle);
}
