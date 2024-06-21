#include "gui.h"
#include "encrypt.hpp"
#include <thread>

int __stdcall wWinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow)
{
	// create gui
	gui::CreateHWindow("CrackME ALPHA");
	gui::CreateDevice();
	gui::CreateImGui();
	protector::time01 = 1000;
	protector::scan = true;
	protector::Title = true;
	protector::Driver = true;
	protector::loopkill = false;

	protector::Protection(); // Start Protection
	protector::anti_dbgSimple(); // Anti Debug
	protector::anti_dbgO();

	while (gui::isRunning)
	{
		gui::BeginRender();
		gui::Render();
		gui::EndRender();


		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	// destroy gui
	gui::DestroyImGui();
	gui::DestroyDevice();
	gui::DestroyHWindow();

	return EXIT_SUCCESS;
}
