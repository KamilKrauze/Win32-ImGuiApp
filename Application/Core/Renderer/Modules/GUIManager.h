#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include <deque>
#include <functional>

#include <imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_win32.h>

#include "Core/Platform.hpp"

class Renderer;
class Window;

enum GUIDefaultStyle : uint8_t
{
    Custom = 0,
    Light = 1,
    Dark = 2,
    Classic = 4,
};

class GUIManager
{
public:
    GUIManager() = default;
    GUIManager(Window& window, Renderer& renderer);
    ~GUIManager();

public:
    void Initialize();
    void SetupBackends();

    void Dispose();
    void SetDefaultStyle(GUIDefaultStyle style = GUIDefaultStyle::Dark);
public:
    void NewFrame();
    void RenderGUI();
    void UpdateGUIData();
    
private:
    static void AppendWin32Hooks();

// Hook functions (registered into ImGui)
private:
#if defined(WINDOWS_NATIVE)
    static void Hook_CreateWindow(ImGuiViewport* viewport);
    static void Hook_DestroyWindow(ImGuiViewport* viewport);
    static void Hook_RenderWindow(ImGuiViewport* viewport, void*);
    static void Hook_SwapBuffers(ImGuiViewport* viewport, void*);
#endif
    
private:
    std::deque<std::function<void()>> m_guiDrawQ;
    Renderer* m_RendererReference = nullptr;
    Window* m_WindowReference = nullptr;

    ImGuiContext* m_ImGuiContext = nullptr;
};

#endif