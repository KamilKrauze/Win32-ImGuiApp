#ifndef RENDERER_BASE_HPP
#define RENDERER_BASE_HPP

#include "Core/Window/Window.hpp"

template<typename RendererT>
class RendererBase
{
protected:
    RendererBase() = default;
public:
    ~RendererBase()
    {
        m_WindowReference = nullptr;
    }

public:
    bool Init()
    {
        return static_cast<RendererT*>(this)->Init();
    }

    bool Destroy()
    {
        m_bIsDestroyed = static_cast<RendererT*>(this)->Destroy();
        return m_bIsDestroyed;
    }

    void Draw()
    {
        static_cast<RendererT*>(this)->Draw();
    }

    void ClearFrameBuffer()
    {
        static_cast<RendererT*>(this)->ClearFrameBuffer();
    }

    void MakeAsCurrent() const
    {
        static_cast<RendererT*>(this)->MakeAsCurrent();
    }

protected:
    bool m_bIsDestroyed = false;
    Window* m_WindowReference = nullptr;
};

#endif //!RENDERER_BASE_HPP