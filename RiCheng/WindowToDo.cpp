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
void WindowToDo::ProcessEvent(Rml::Event& event) {
	
}