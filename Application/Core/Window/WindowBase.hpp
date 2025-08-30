#ifndef WINDOW_BASE_HPP
#define WINDOW_BASE_HPP

#include <utility>

#include "Core/Window/WindowSpec.hpp"

template<typename WindowT, typename WindowHandle>
class WindowBase
{
protected:
	WindowBase() = default;
public:
	WindowBase(const WindowSpec& _spec)
	{
		static_cast<WindowT*>(this)->WindowT(_spec);
	}

	~WindowBase() = default;
	
public:

    bool Create()
    {
        return static_cast<WindowT*>(this)->Create();
    }
    
    void Show()
    {
        static_cast<WindowT*>(this)->Show();
    }
    
    void Update() const
    {
        static_cast<WindowT*>(this)->Update();
    }
    
    bool Destroy()
    {
        return static_cast<WindowT*>(this)->Destroy();
    }

    bool ShouldWindowClose()
    {
        return static_cast<WindowT*>(this)->ShouldWindowClose();
    }

    WindowHandle& GetNativeWindow()
    {
        return static_cast<WindowT*>(this)->GetNativeWindow();
    }

    std::pair<const int, const int> GetWindowSize() const
    {
        return {m_WindowSpec.width, m_WindowSpec.height};
    }
    
protected:
	WindowHandle m_WindowHandle;
	WindowSpec m_WindowSpec;
    bool m_CloseSignalSent = false;
};

#endif //! WINDOW_BASE_HPP