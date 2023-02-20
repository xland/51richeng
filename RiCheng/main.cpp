#include <Windows.h>
#include <string>
#include <RmlUi/Core.h>
#include "spdlog/spdlog.h"
#include "Shell.h"
#include "WindowMain.h"
#include "WindowToDo.h"
#include "App.h"



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) {
	App::Init();
	new WindowMain(1024,768);
	App::Start();
	App::Dispose();	
	return 0;
}