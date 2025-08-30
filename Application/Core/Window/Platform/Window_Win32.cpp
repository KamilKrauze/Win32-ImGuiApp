#include "Window_Win32.hpp"

#include <imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_win32.h>

#include <iostream>

static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Window::Window()
{
    m_WindowSpec = WindowSpec({L"Please change window title...",400,300});        
}

Window::Window(const WindowSpec& _spec)
{
    m_WindowSpec = _spec;
}

bool Window::Create()
{
    m_wc = { sizeof(m_wc), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, m_WindowSpec.title, nullptr };
    ::RegisterClassExW(&m_wc);
    m_WindowHandle = ::CreateWindowW(m_wc.lpszClassName, m_WindowSpec.title, WS_OVERLAPPEDWINDOW, 0, 0, m_WindowSpec.width, m_WindowSpec.height, nullptr, nullptr, m_wc.hInstance, this);

    return m_WindowHandle != nullptr;
}

void Window::Show() const
{
    ::ShowWindow(m_WindowHandle, SW_SHOWDEFAULT);
}

void Window::Update() const
{
    ::UpdateWindow(m_WindowHandle);
}

bool Window::Destroy()
{
    ::DestroyWindow(m_WindowHandle);
    ::UnregisterClassW(m_wc.lpszClassName, m_wc.hInstance);
    m_WindowHandle = nullptr;
    return m_WindowHandle == nullptr;
}

bool Window::ShouldWindowClose()
{
    if (m_CloseSignalSent)
    {
        return m_CloseSignalSent;
    }
    
    MSG msg;
    bool result = false;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        result = msg.message == WM_QUIT;
    }

    if (result)
    {
        std::cout << "Shutting window down..." << std::endl;
        m_CloseSignalSent = result;
    }
    
    return result;
}

HWND& Window::GetNativeWindow()
{
    return m_WindowHandle;
}

LRESULT Window::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            m_WindowSpec.width = LOWORD(lParam);
            m_WindowSpec.height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return ::DefWindowProcW(m_WindowHandle, msg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* windowObjRef = nullptr;
    
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        windowObjRef = reinterpret_cast<Window*>(cs->lpCreateParams);

        // Store pointer in window user data
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowObjRef));
        auto& nativeWindowHandle = windowObjRef->GetNativeWindow();
        nativeWindowHandle = hWnd;
    }
    else
    {
        // Retrieve pointer for subsequent messages
        windowObjRef = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (windowObjRef)
    {
        return windowObjRef->HandleMessage(msg, wParam, lParam);
    }

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}
