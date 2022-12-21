#include "WindowMain.h"
#include <Windows.h>

WindowMain::WindowMain() {
	auto context = Rml::GetContext("main");
	document = context->LoadDocument("ui/main.rml");
	document->SetId("main");
	document->Show();
	
	auto closeBtn = document->GetElementById("closeBtn");
	closeBtn->SetInnerRML((const char*)u8"\ue6e7");
	closeBtn->AddEventListener(Rml::EventId::Click, this);

	auto maximizeBtn = document->GetElementById("maximizeBtn");
	maximizeBtn->SetInnerRML((const char*)u8"\ue6e5");
	maximizeBtn->AddEventListener(Rml::EventId::Click, this);

	auto minimizeBtn = document->GetElementById("minimizeBtn");
	minimizeBtn->SetInnerRML((const char*)u8"\ue6e8");
	minimizeBtn->AddEventListener(Rml::EventId::Click, this);
}

void WindowMain::ProcessEvent(Rml::Event& event) {
	switch (event.GetId())
	{
		case Rml::EventId::Click: {
			auto eleId = event.GetTargetElement()->GetId();
			if (eleId == "minimizeBtn") {
				HWND hWnd = ::GetForegroundWindow();
				ShowWindow(hWnd, SW_MINIMIZE);
			}
			else if(eleId == "closeBtn")
			{
				HWND hWnd = ::GetForegroundWindow();
				PostMessage(hWnd, WM_CLOSE, 0, 0);
			}
			else if (eleId == "maximizeBtn")
			{
				auto ele = event.GetTargetElement();
				if (ele->GetInnerRML() == std::string{ (const char*)u8"\ue6e5" }) {
					ele->SetInnerRML((const char*)u8"\ue6e9");
					HWND hWnd = ::GetForegroundWindow();
					ShowWindow(hWnd, SW_MAXIMIZE);
				}
				else
				{
					ele->SetInnerRML((const char*)u8"\ue6e5");
					HWND hWnd = ::GetForegroundWindow();
					ShowWindow(hWnd, SW_RESTORE);
				}
			}
			break;
		}
		default:
			break;
	}
}