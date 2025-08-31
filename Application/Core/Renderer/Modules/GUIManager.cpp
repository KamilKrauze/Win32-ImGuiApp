#include "Core/Renderer/Modules/GUIManager.h"

#include <iostream>

#include "Core/Window/Window.hpp"
#include "Core/Renderer/Renderer.hpp"

struct WGL_WindowData
{
    HDC   hDC   = nullptr;
    HGLRC hGLRC = nullptr;
};

GUIManager::GUIManager(Window& window, Renderer& renderer)
{
    m_WindowReference = &window;
    m_RendererReference = &renderer;
}

GUIManager::~GUIManager()
{
    m_RendererReference = nullptr;
    m_WindowReference = nullptr;
}

void GUIManager::Initialize()
{
    IMGUI_CHECKVERSION();
    
    // Create ImGui context just for this window
    m_ImGuiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(m_ImGuiContext);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags =
        ImGuiConfigFlags_NavEnableKeyboard  // Enable Keyboard Control
        | ImGuiConfigFlags_NavEnableGamepad // Enable Gamepad Controls
        | ImGuiConfigFlags_DockingEnable   // Enable Docking
        | ImGuiConfigFlags_ViewportsEnable;

    AppendWin32Hooks();
}

void GUIManager::SetupBackends()
{
    // Setup Platform/Renderer backends
#if defined(WINDOWS_NATIVE)
    auto& nativeWinRef = m_WindowReference->GetNativeWindow();
    ImGui_ImplWin32_InitForOpenGL(nativeWinRef);
#endif
    ImGui_ImplOpenGL3_Init();
}

void GUIManager::Dispose()
{
    ImGui_ImplOpenGL3_Shutdown();
#if defined(WINDOWS_NATIVE)
    ImGui_ImplWin32_Shutdown();
#endif
    ImGui::SetCurrentContext(m_ImGuiContext);
    ImGui::DestroyContext(m_ImGuiContext);
    m_ImGuiContext = nullptr;
}

void GUIManager::SetDefaultStyle(GUIDefaultStyle style)
{
    switch (style)
    {
    case GUIDefaultStyle::Custom:
        return;
    case GUIDefaultStyle::Light:
        ImGui::StyleColorsLight();
        break;
    case GUIDefaultStyle::Dark:
        ImGui::StyleColorsDark();
        break;
    case GUIDefaultStyle::Classic:
        ImGui::StyleColorsClassic();
        break;
    }
}

void GUIManager::NewFrame()
{
    ImGui::SetCurrentContext(m_ImGuiContext);

    ImGui_ImplOpenGL3_NewFrame();

#if defined(WINDOWS_NATIVE)
    ImGui_ImplWin32_NewFrame();
#endif

    ImGui::NewFrame();
}


static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static float f = 0.0f;
static int counter = 0;

void GUIManager::RenderGUI()
{
    {
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
    
        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
    
        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        {
            counter++;
            std::cout << "Counter: " << counter << std::endl;
        }
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        
        ImGui::End();
    }
}

void GUIManager::UpdateGUIData()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
#if defined(WINDOWS_NATIVE)
    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        // Restore the OpenGL rendering context to the main window DC, since platform windows might have changed it.
        m_RendererReference->MakeAsCurrentContext();
    }
#endif
}


void GUIManager::AppendWin32Hooks()
{
#ifdef WINDOWS_NATIVE
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    IM_ASSERT(platform_io.Renderer_CreateWindow == NULL);
    IM_ASSERT(platform_io.Renderer_DestroyWindow == NULL);
    IM_ASSERT(platform_io.Platform_RenderWindow == NULL);
    IM_ASSERT(platform_io.Renderer_SwapBuffers == NULL);

    platform_io.Renderer_CreateWindow  = Hook_CreateWindow;
    platform_io.Renderer_DestroyWindow = Hook_DestroyWindow;
    platform_io.Platform_RenderWindow  = Hook_RenderWindow;
    platform_io.Renderer_SwapBuffers   = Hook_SwapBuffers;
#endif
}

#ifdef WINDOWS_NATIVE
void GUIManager::Hook_CreateWindow(ImGuiViewport* viewport)
{
    assert(viewport->RendererUserData == nullptr);

    auto* data = IM_NEW(WGL_WindowData);
    HWND hwnd = (HWND)viewport->PlatformHandle;

    HDC hDc = ::GetDC(hwnd);
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return;
    
    data->hDC = ::GetDC(hwnd);
    data->hGLRC = wglGetCurrentContext();

    viewport->RendererUserData = data;
}

void GUIManager::Hook_DestroyWindow(ImGuiViewport* viewport)
{
    if (viewport->RendererUserData == nullptr)
        return;

    auto* data = (WGL_WindowData*)viewport->RendererUserData;

    if (data->hGLRC)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(data->hGLRC);
    }

    if (data->hDC)
        ::ReleaseDC((HWND)viewport->PlatformHandle, data->hDC);

    IM_DELETE(data);
    viewport->RendererUserData = nullptr;
}

void GUIManager::Hook_RenderWindow(ImGuiViewport* viewport, void*)
{
    if (auto* data = (WGL_WindowData*)viewport->RendererUserData)
        wglMakeCurrent(data->hDC, data->hGLRC);
}

void GUIManager::Hook_SwapBuffers(ImGuiViewport* viewport, void*)
{
    if (auto* data = (WGL_WindowData*)viewport->RendererUserData)
        ::SwapBuffers(data->hDC);
}
#endif
