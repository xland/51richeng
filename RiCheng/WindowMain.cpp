#include "WindowMain.h"

WindowMain::WindowMain() {
	auto context = Rml::GetContext("main");
	document = context->LoadDocument("ui/main.rml");
	document->SetId("main");
	document->Show();
	
}

void WindowMain::ProcessEvent(Rml::Event& event) {

}