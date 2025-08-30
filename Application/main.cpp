#include <iostream>

#include "Core/Window/Window.hpp"
#include "Core/Renderer/Renderer.hpp"
#include "Core/Renderer/Modules/GUIManager.h"


int main()
{
	Window window;
	Renderer renderer;
	GUIManager guiManager;
	
	window = Window({L"Window 1",800,600});

	window.Create();
	window.Update();
	
	renderer = Renderer(&window);
	renderer.Create();

	guiManager = GUIManager(window, renderer);
	guiManager.Initialize();
	guiManager.SetupBackends();
	guiManager.SetDefaultStyle(GUIDefaultStyle::Light);

	window.Show();
	bool quit = false;
	while(!quit)
	{
		quit = window.ShouldWindowClose();
		if (quit)
			break;
		if (::IsIconic(window.GetNativeWindow()))
		{
			::Sleep(1);
			continue;
		}

		guiManager.NewFrame();

		ImGui::ShowDemoWindow();
		
		guiManager.RenderGUI();
		renderer.ClearFrameBuffer();
		guiManager.UpdateGUIData();
		renderer.Draw();
	}

	guiManager.Dispose();
	renderer.Destroy();
	window.Destroy();
	
	return 0;
}
	