#include "WindowMain.h"
#include <Windows.h>
#include <format>
#include "ResourceHelper.h"
#include "Time.h"

using namespace std::chrono;

WindowMain::WindowMain()  {
	initDocument();
	setBtn();
	initCurDate();
	calendarSmall = std::make_unique<CalendarSmall>();
	viewDay = std::make_unique<ViewDay>();
}

void WindowMain::initDocument() {
	auto context = Rml::GetContext("main");
	document = context->LoadDocument("ui/main.rml");
	document->SetId("main");
	document->Show();
}

inline Rml::Element* WindowMain::setEleIcon(std::string&& id) {
	auto btn = document->GetElementById(id);
	auto icon = ResourceHelper::IconMap[id];
	btn->SetInnerRML(icon);
	return btn;
}

void WindowMain::setBtn() {
	setEleIcon("appIcon");
	setEleIcon("closeBtn")->AddEventListener(Rml::EventId::Click, this);
	setEleIcon("maximizeBtn")->AddEventListener(Rml::EventId::Click, this);
	setEleIcon("minimizeBtn")->AddEventListener(Rml::EventId::Click, this);
	setEleIcon("preBtn")->AddEventListener(Rml::EventId::Click, this);
	setEleIcon("nextBtn")->AddEventListener(Rml::EventId::Click, this);
	
	auto switchOption = document->GetElementById("switchOptionDay");
	switchOption->AddEventListener(Rml::EventId::Click, this);
	switchOption = switchOption->GetNextSibling();
	switchOption->AddEventListener(Rml::EventId::Click, this);
	switchOption->GetNextSibling()->AddEventListener(Rml::EventId::Click, this);
		
}

void WindowMain::initCurDate() {
	auto curDayStr = std::to_string((int)(Time::currentDay.year())) + (const char*)u8"年"
		+ std::to_string((unsigned)(Time::currentDay.month())) + (const char*)u8"月"
		+ std::to_string((unsigned)(Time::currentDay.day())) + (const char*)u8"日";
	document->GetElementById("currentDay")->SetInnerRML(curDayStr);
}

bool WindowMain::windowToolBtnEventProcess(std::string& eleId,Rml::Element* ele) {
	if (eleId == "minimizeBtn") {
		ShowWindow(::GetForegroundWindow(), SW_MINIMIZE);
		return true;
	}
	else if (eleId == "closeBtn")
	{
		PostMessage(::GetForegroundWindow(), WM_CLOSE, 0, 0);
		return true;
	}
	else if (eleId == "maximizeBtn")
	{
		if (ele->GetInnerRML() == std::string{ ResourceHelper::IconMap["maximizeBtn"] }) {
			ele->SetInnerRML(ResourceHelper::IconMap["restoreBtn"]);
			HWND hWnd = ::GetForegroundWindow();
			ShowWindow(hWnd, SW_MAXIMIZE);
		}
		else
		{
			ele->SetInnerRML(ResourceHelper::IconMap["maximizeBtn"]);
			HWND hWnd = ::GetForegroundWindow();
			ShowWindow(hWnd, SW_RESTORE);
		}
		return true;
	}
	return false;
}

bool WindowMain::switchViewModeProcess(std::string& eleId, Rml::Element* ele) {
	if (eleId == "switchBtn") {
		auto menu = document->GetElementById("switchMenu");
		menu->SetProperty("display", "block");
		document->AddEventListener(Rml::EventId::Click, this);
		return true;
	}
	else if (eleId == "switchOptionDay")
	{
		if (ele->GetClassNames().find("switchBtnSelected") != std::string::npos) return true;
		ele->SetClass("switchBtnSelected", true);
		auto nextEle = ele->GetNextSibling();
		nextEle->SetClass("switchBtnSelected", false);
		nextEle->GetNextSibling()->SetClass("switchBtnSelected", false);
		if (viewWeek.get() != nullptr) {
			viewWeek->hide();
		}
		if (viewMonth.get() != nullptr) {
			viewMonth->hide();
		}
		viewDay->show();
		return true;
	}
	else if (eleId == "switchOptionWeek")
	{
		if (ele->GetClassNames().find("switchBtnSelected") != std::string::npos) return true;
		ele->SetClass("switchBtnSelected", true);
		ele->GetNextSibling()->SetClass("switchBtnSelected", false);
		ele->GetPreviousSibling()->SetClass("switchBtnSelected", false);
		viewDay->hide();
		if (viewMonth.get() != nullptr) {
			viewMonth->hide();
		}
		if (viewWeek.get() == nullptr) {
			viewWeek = std::make_unique<ViewWeek>();
		}
		viewWeek->show();
		return true;
	}
	else if (eleId == "switchOptionMonth")
	{
		if (ele->GetClassNames().find("switchBtnSelected") != std::string::npos) return true;
		ele->SetClass("switchBtnSelected", true);
		auto preEle = ele->GetPreviousSibling();
		preEle->SetClass("switchBtnSelected", false);
		preEle->GetPreviousSibling()->SetClass("switchBtnSelected", false);
		viewDay->hide();
		if (viewWeek.get() != nullptr) {
			viewWeek->hide();
		}
		if (viewMonth.get() == nullptr) {
			viewMonth = std::make_unique<ViewMonth>();
		}
		viewMonth->show();
		return true;
	}
	else if (document == ele) {
		document->GetElementById("switchMenu")->SetProperty("display", "none");
		document->RemoveEventListener(Rml::EventId::Click, this);
		return true;
	}
	return false;
}



void WindowMain::ProcessEvent(Rml::Event& event) {
	auto ele = event.GetCurrentElement();
	auto eleId = ele->GetId();
	switch (event.GetId())
	{
	case Rml::EventId::Click: {
		if (windowToolBtnEventProcess(eleId, ele)) return;
		else if (switchViewModeProcess(eleId, ele)) return;
		break;
	}
	}
}