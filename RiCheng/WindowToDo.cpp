#include "WindowToDo.h"
#include <RmlUi/Debugger.h>


static int winToDoCount = 0;

WindowToDo::WindowToDo(int width, int height)
	: WindowBase(width, height, "windowToDo"+std::to_string(++winToDoCount)) 
{
	SetWindowText(hwnd, L"ÎÞÓÇÈÕ³Ì");
	initDocument();
}
void WindowToDo::initDocument() {
	document = context->LoadDocument("ui/windowToDo.rml");
	document->SetId("windowToDo" + std::to_string(winToDoCount));
	document->Show();
}
bool WindowToDo::IsMouseInCaptionArea(int x, int y) {
	return true;
}
void WindowToDo::WindowShowNormal() {
	//auto ele = document->GetElementById("maximizeBtn");
	//ele->SetInnerRML((const char*)u8"\ue6e5");
}
void WindowToDo::ProcessEvent(Rml::Event& event) {
	
}