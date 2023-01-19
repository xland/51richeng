#include "WindowToDo.h"
#include <RmlUi/Debugger.h>
WindowToDo::WindowToDo(int width, int height)
	: WindowBase(width, height, "windowToDo") {
#ifdef DEBUG
	Rml::Debugger::SetContext(context);
#endif // DEBUG
	SetWindowText(hwnd, L"ÎÞÓÇÈÕ³Ì");
	initDocument();
}
void WindowToDo::initDocument() {
	document = context->LoadDocument("ui/windowToDo.rml");
	document->SetId("windowToDo");
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