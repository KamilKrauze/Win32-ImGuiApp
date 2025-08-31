#ifndef RENDERER_WGL_HPP
#define RENDERER_WGL_HPP

#include "Core/Window/Window.hpp"
#include "Core/Renderer/RendererBase.hpp"

struct DeviceContext
{
    HDC handle = nullptr;
};

class Renderer : RendererBase<Renderer, HGLRC, DeviceContext>
{
public:
    Renderer() = default;
    explicit Renderer(Window* windowRef);
    
    ~Renderer() { m_WindowReference = nullptr; }

public:
    bool Create();
    bool Destroy();

    void Draw();
    void ClearFrameBuffer();
    
    void MakeAsCurrentContext() const;

/// DATA ACCESS ///
public:
    HGLRC& GetRenderContextRef();
    DeviceContext& GetDeviceContextRef();
    
private:
    bool CreateDeviceWGL();
    void CleanupDeviceWGL() const;


private:
    DeviceContext m_DeviceContext;
    HGLRC m_RenderContextHandle = nullptr;
};

#endif //!RENDERER_WGL_HPP