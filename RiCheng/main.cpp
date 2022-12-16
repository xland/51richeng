#include <Windows.h>
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include "RmlUi_Backend.h"
#include "include/Shell.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) {
	int width = 1024;
	int height = 768;
	if (!Shell::Initialize()) {
		return -1;
	}
	if (!Backend::Initialize("test", width, height, true))
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
	auto docStr = R"(<rml><head><title>Demo</title>
<style>body{background:#fff;font-family: "Microsoft YaHei";font-size: 20dp;width: 100%;height:100%;color: #000;}</style></head>
<body>Hello World</body></rml>)";
	if (Rml::ElementDocument* document = context->LoadDocumentFromMemory(docStr))
	{
		document->Show();
	}
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
	return 0;
}