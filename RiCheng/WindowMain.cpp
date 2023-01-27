#include "WindowMain.h"
#include <RmlUi/Debugger.h>
#include <format>
#include "TypeDefine.h"
#include "ResourceHelper.h"
#include "CalendarModel.h"

using namespace std::chrono;

WindowMain::WindowMain(int width, int height)
	: WindowBase(width,height, "windowMain") 
{
	SetWindowText(hwnd, L"无忧日程");
	initDocument();
	setBtn();
	initCurDate();
	calendarSmall = std::make_unique<CalendarSmall>();
	viewDay = std::make_unique<ViewDay>();
	CalendarModel::get()->registNewDayEventObj(this);
}

bool WindowMain::IsMouseInCaptionArea(int x, int y) {
	if (y < 50) {
		if (x < 430) {
			return true;
		}
		else if (x > 860 && x < width - 180) {
			return true;
		}
	}
	return false;
}

void WindowMain::WindowShowNormal() {
	auto ele = document->GetElementById("maximizeBtn");
	ele->SetInnerRML((const char*)u8"\ue6e5");
}

void WindowMain::initDocument() {
	document = context->LoadDocument("ui/windowMain.rml");
	document->SetId("windowMain");
	document->Show();
}

void WindowMain::setBtn() {
	document->GetElementById("closeBtn")->AddEventListener(Rml::EventId::Click, this);
	document->GetElementById("maximizeBtn")->AddEventListener(Rml::EventId::Click, this);
	document->GetElementById("minimizeBtn")->AddEventListener(Rml::EventId::Click, this);
	document->GetElementById("preBtn")->AddEventListener(Rml::EventId::Click, this);
	document->GetElementById("nextBtn")->AddEventListener(Rml::EventId::Click, this);
	
	Rml::Element* switchOption = document->GetElementById("switchOptionDay");
	switchOption->AddEventListener(Rml::EventId::Click, this);
	switchOption = switchOption->GetNextSibling();
	switchOption->AddEventListener(Rml::EventId::Click, this);
	switchOption->GetNextSibling()->AddEventListener(Rml::EventId::Click, this);		
}

void WindowMain::initCurDate() {
	auto [y,d,m,wod] = CalendarModel::get()->currentDay;
	auto curDayStr = std::to_string(y) + (const char*)u8"年"
		+ std::to_string(d) + (const char*)u8"月"
		+ std::to_string(m) + (const char*)u8"日";
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
		if (ele->GetInnerRML() == (const char*)u8"\ue6e5") {
			ele->SetInnerRML("&#xe6e9;");
			HWND hWnd = ::GetForegroundWindow();
			ShowWindow(hWnd, SW_MAXIMIZE);
		}
		else
		{
			ele->SetInnerRML("&#xe6e5;");
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
	auto eventId = event.GetId();
	if (eventId == Rml::EventId::Click) {
		auto ele = event.GetCurrentElement();
		auto eleId = ele->GetId();
		if (windowToolBtnEventProcess(eleId, ele)) return;
		else if (switchViewModeProcess(eleId, ele)) return;
	}
	else if (eventId == CalendarModel::get()->NewDayEventId) {
		initCurDate();
		return;
	}
}