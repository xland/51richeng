#include <RmlUi/Core.h>
#include "../RmlBackend/RmlUi_Include_Windows.h"
#include "../RmlBackend/RmlUi_Backend.h"
#include "../RmlShell/include/Shell.h"
#include "TopBar.h"
#include "resource.h"
#ifdef DEBUG
#include <RmlUi/Debugger.h>
#endif // DEBUG

Rml::Context* context = nullptr;
TopBar* topBar = nullptr;
std::map<std::string, int> resourcePathMap = {
	{"UI/BodyDay.rml",IDR_UI1},
	{"UI/BodyList.rml",IDR_UI2},
	{"UI/BodyMonth.rml",IDR_UI3},
	{"UI/BodyWeek.rml",IDR_UI4},
	{"UI/BodyYear.rml",IDR_UI5},
	{"UI/DialogCycle.rml",IDR_UI6},
	{"UI/DialogDelete.rml",IDR_UI7},
	{"UI/DialogDonate.rml",IDR_UI8},
	{"UI/DialogForm.rml",IDR_UI9},
	{"UI/DialogSetting.rml",IDR_UI10},
	{"UI/DialogTop.rml",IDR_UI11},
	{"UI/iconfont.ttf",IDR_UI12},
	{"UI/TopBar.rml",IDR_UI13},
	{"UI/weixin.png",IDR_UI14},
	{"UI/zhifubao.png",IDR_UI15}
};

int APIENTRY WinMain(HINSTANCE /*instance_handle*/, HINSTANCE /*previous_instance_handle*/, char* /*command_line*/, int /*command_show*/) {

	const int window_width = 2000;
	const int window_height = 1300; //600
	if (!Shell::Initialize(resourcePathMap))
		return -1;
	if (!Backend::Initialize(u8"ElectronÐ¶ÔØ³ÌÐò", window_width, window_height, true))
	{
		Shell::Shutdown();
		return -1;
	}
	Backend::isWindowResizable = true;
	Rml::SetSystemInterface(Backend::GetSystemInterface());
	Rml::SetRenderInterface(Backend::GetRenderInterface());
	Rml::Initialise();
	context = Rml::CreateContext("main", Rml::Vector2i(window_width, window_height));
	if (!context) {
		Rml::Shutdown();
		Backend::Shutdown();
		Shell::Shutdown();
		return -1;
	}
#ifdef DEBUG
	Rml::Debugger::Initialise(context);
#endif
	Shell::LoadFonts();
	topBar = new TopBar();
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
	delete topBar;
	return 0;
}