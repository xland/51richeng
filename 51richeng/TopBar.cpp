#include "TopBar.h"
#include "TopBar.h"
#include <RmlUi_Backend.h>
#include "resource.h"
TopBar::TopBar() {
	auto context = Rml::GetContext("main");
	document = context->LoadDocument("UI/topBar.rml");
	document->SetId("topBar");
	document->Show();
	document->AddEventListener(Rml::EventId::Resize, this);

	setHitTestArea();

}
void TopBar::setHitTestArea() {
	auto titleLabel = document->GetElementById("titleLabel");
	Rml::Vector4f rect = { titleLabel->GetAbsoluteLeft(),titleLabel->GetAbsoluteTop(),titleLabel->GetClientWidth(),titleLabel->GetClientHeight() };
	Backend::WindowHitTestArea.clear();
	Backend::WindowHitTestArea.push_back(rect);
}
void TopBar::ProcessEvent(Rml::Event& event) {
	switch (event.GetId())
	{
	case Rml::EventId::Click: {
		auto id = event.GetTargetElement()->GetId();
		if (id == "minimizeBtn") {
			HWND hWnd = ::GetForegroundWindow();
			ShowWindow(hWnd, SW_MINIMIZE);
		}
		else if (id == "maximizeBtn") {
			HWND hWnd = ::GetForegroundWindow();
			ShowWindow(hWnd, SW_MAXIMIZE);
		}
		else if (id == "closeBtn")
		{
			HWND hWnd = ::GetForegroundWindow();
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;
	}
	case Rml::EventId::Resize: {
		document->UpdateDocument();
		setHitTestArea();
		break;
	}
	default:
		break;
	}
}
