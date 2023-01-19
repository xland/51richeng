#include <Windows.h>
#include <string>
#include <RmlUi/Core.h>
#include "spdlog/spdlog.h"
#include "RmlUi_Backend.h"
#include "Shell.h"
#include "WindowMain.h"
#include "WindowToDo.h"
#include "App.h"



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) {
	App::init();
	new WindowMain(1024,768);
	App::start();
	App::dispose();
	/*int width = 1024;
	int height = 768;	
	if (!Shell::Initialize()) {
		spdlog::error("Shell::Initialize()Ê§°Ü");
		return -1;
	}
	if (!Backend::Initialize("51RiCheng", width, height, true)) {
		Shell::Shutdown();
		spdlog::error("Backend::InitializeÊ§°Ü");
		return -1;
	}
	Rml::SetSystemInterface(Backend::GetSystemInterface());
	Rml::SetRenderInterface(Backend::GetRenderInterface());
	Rml::Initialise();
	Rml::Context* context = Rml::CreateContext("main", Rml::Vector2i(width, height));
	if (context == nullptr)
	{
		spdlog::error("Rml::CreateContext mainÊ§°Ü");
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
	delete windowMain;*/
	return 0;
}