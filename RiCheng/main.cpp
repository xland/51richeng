#include <Windows.h>
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include "RmlUi_Backend.h"
#include "Shell.h"
#include "WindowMain.h"
#include <string>
#include <cwchar>
#include <stdexcept>


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) {
	int width = 1024;
	int height = 768;
	if (!Shell::Initialize()) {
		return -1;
	}
	if (!Backend::Initialize("", width, height, true))
	{
		Shell::Shutdown();
		return -1;
	}
	Rml::SetSystemInterface(Backend::GetSystemInterface());
	Rml::SetRenderInterface(Backend::GetRenderInterface());
	Rml::Initialise();
	Rml::Context* context = Rml::CreateContext("main", Rml::Vector2i(width, height));
	if (context == nullptr)
	{
		Rml::Shutdown();
		Backend::Shutdown();
		Shell::Shutdown();
		return -1;
	}
	Rml::Debugger::Initialise(context);
	Shell::LoadFonts();
	WindowMain* windowMain{ new WindowMain() };
	bool running = true;
	while (running)
	{
		running = Backend::ProcessEvents(context, &Shell::ProcessKeyDownShortcuts);
		context->Update();
		Backend::BeginFrame();
		context->Render();
		Backend::PresentFrame();
	}
	Rml::Shutdown();
	Backend::Shutdown();
	Shell::Shutdown();
	delete windowMain;
	return 0;
}