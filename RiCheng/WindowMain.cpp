#include "WindowMain.h"
#include <Windows.h>
#include <format>
#include "ResourceHelper.h"

using namespace std::chrono;

WindowMain::WindowMain() {
	initDocument();
	setBtn();
	initCurDate();
	initCalendar();	
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
	setEleIcon("switchDropDownBtn");
	
	document->GetElementById("switchBtn")->AddEventListener(Rml::EventId::Click, this);
	auto switchOption = document->GetElementById("switchOptionDay");
	switchOption->AddEventListener(Rml::EventId::Click, this);
	switchOption = switchOption->GetNextSibling();
	switchOption->AddEventListener(Rml::EventId::Click, this);
	switchOption->GetNextSibling()->AddEventListener(Rml::EventId::Click, this);

	auto ele = document->GetElementById("todoItem1");
	ele->AddEventListener(Rml::EventId::Mousedown, this);
}

void WindowMain::initCurDate() {
	today = date::year_month_day{ floor<days>(system_clock::now()) };
	auto curDayStr = std::to_string((int)(today.year())) + (const char*)u8"年" 
		+ std::to_string((unsigned)(today.month())) + (const char*)u8"月" 
		+ std::to_string((unsigned)(today.day())) + (const char*)u8"日";
	document->GetElementById("currentDay")->SetInnerRML(curDayStr);
}

void WindowMain::initCalendar() {
	auto dayEle = document->GetElementById("calendarEleBox")->GetChild(0);

	date::year_month_day firstDay { today.year(),today.month(),date::day{1} };
	date::year_month_day_last lastDay { today.year(),date::month_day_last{today.month()} };
	iso_week::year_weeknum_weekday weekNumWeekDay{ firstDay };
	auto weekDay = (unsigned)(weekNumWeekDay.weekday())-1;
	if (weekDay == 0) {
	}
	else
	{
		date::year_month_day_last temp{ today.year() ,date::month_day_last { --today.month() } };
		int day = (unsigned)temp.day() - weekDay;
		for (size_t i = 0; i < weekDay; i++)
		{
			day += 1;
			dayEle->SetInnerRML(std::to_string(day));
			dayEle->SetClassNames("notCurMonthEle");
			dayEle = dayEle->GetNextSibling();
		}
	}
	auto curMonthLastDay = (unsigned)lastDay.day();
	for (size_t i = 1; i <= curMonthLastDay; i++)
	{
		dayEle->SetInnerRML(std::to_string(i));
		dayEle->SetClassNames("curMonthEle");
		dayEle = dayEle->GetNextSibling();
	}
	auto lastEleNum = 42 - curMonthLastDay - weekDay;
	for (size_t i = 1; i <= lastEleNum; i++)
	{
		dayEle->SetInnerRML(std::to_string(i));
		dayEle->SetClassNames("notCurMonthEle");
		dayEle = dayEle->GetNextSibling();
	}
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
		document->GetElementById("switchMenu")->SetProperty("display", "block");
		document->AddEventListener(Rml::EventId::Click, this);
		return true;
	}
	else if (eleId == "switchOptionDay")
	{
		document->GetElementById("switchBtn")->GetFirstChild()->SetInnerRML((const char*)u8"日");
		auto tarEle = document->GetElementById("viewDay");
		tarEle->SetProperty("display", "flex");
		tarEle = tarEle->GetNextSibling();
		tarEle->SetProperty("display", "none");
		tarEle->GetNextSibling()->SetProperty("display", "none");
		return true;
	}
	else if (eleId == "switchOptionWeek")
	{
		document->GetElementById("switchBtn")->GetFirstChild()->SetInnerRML((const char*)u8"周");
		auto tarEle = document->GetElementById("viewDay");
		tarEle->SetProperty("display", "none");
		tarEle = tarEle->GetNextSibling();
		tarEle->SetProperty("display", "block");
		tarEle->GetNextSibling()->SetProperty("display", "none");
		return true;
	}
	else if (eleId == "switchOptionMonth")
	{
		document->GetElementById("switchBtn")->GetFirstChild()->SetInnerRML((const char*)u8"月");
		auto tarEle = document->GetElementById("viewDay");
		tarEle->SetProperty("display", "none");
		tarEle = tarEle->GetNextSibling();
		tarEle->SetProperty("display", "none");
		tarEle->GetNextSibling()->SetProperty("display", "block");
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
		case Rml::EventId::Mousedown: {
			if (eleId == "todoItem1") {
				auto point1 = ele->GetAbsoluteOffset();
				auto point2 = event.GetUnprojectedMouseScreenPos();
				mousePointLeftTopPointSpace = point2 - point1;
				document->AddEventListener(Rml::EventId::Mousemove, this);
				document->AddEventListener(Rml::EventId::Mouseup, this);
			}
			break;
		}
		case Rml::EventId::Mousemove: {
			if (ele == document) {
				auto mousePoint = event.GetUnprojectedMouseScreenPos();
				auto leftTopPoint = mousePoint - mousePointLeftTopPointSpace;
				auto targetEle = document->GetElementById("todoItem1");
				targetEle->SetProperty(Rml::PropertyId::Top, Rml::Property(leftTopPoint.y, Rml::Property::PX));
			}
			break;
		}
		case Rml::EventId::Mouseup: {
			if (ele == document) {
				document->RemoveEventListener(Rml::EventId::Mousemove, this);
				document->RemoveEventListener(Rml::EventId::Mouseup, this);
			}
			break;
		}
		default:
			break;
	}
}