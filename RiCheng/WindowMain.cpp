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
	setEleIcon("switchDropDownBtn")->AddEventListener(Rml::EventId::Click, this);
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