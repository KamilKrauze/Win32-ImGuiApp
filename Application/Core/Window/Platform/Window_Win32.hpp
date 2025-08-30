#ifndef WINDOW_WIN32_HPP
#define WINDOW_WIN32_HPP

#include "Core/Window/WindowBase.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

/**
 * Win32 window class.
 */
class Window : public WindowBase<Window, HWND>
{
public:
    Window();
    Window(const WindowSpec& _spec);
    ~Window() = default;
    
public:
    bool Create(); 
    void Show() const;
    void Update() const;
    bool Destroy();

public:
    bool ShouldWindowClose();
    
public:
    HWND& GetNativeWindow();

public:
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    
private:
    WNDCLASSEXW m_wc;
};

#endif
