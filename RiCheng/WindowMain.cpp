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
	context->EnableMouseCursor(true);
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
	
	Rml::ElementList list;
	document->QuerySelectorAll(list,".todoItem > div");
	for (auto& ele:list)
	{
		ele->AddEventListener(Rml::EventId::Mousedown, this);
	}	
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
		viewMode = 1;
		return true;
	}
	else if (eleId == "switchOptionWeek")
	{
		document->GetElementById("switchBtn")->GetFirstChild()->SetInnerRML((const char*)u8"周");
		auto tarEle = document->GetElementById("viewDay");
		tarEle->SetProperty("display", "none");
		tarEle = tarEle->GetNextSibling();
		tarEle->SetProperty("display", "flex");
		tarEle->GetNextSibling()->SetProperty("display", "none");
		viewMode = 2;
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
		viewMode = 3;
		return true;
	}
	else if (document == ele) {
		document->GetElementById("switchMenu")->SetProperty("display", "none");
		document->RemoveEventListener(Rml::EventId::Click, this);
		return true;
	}
	return false;
}

void WindowMain::updateTargetTime() {
	auto top = targetEle->GetProperty(Rml::PropertyId::Top)->value.Get<int>();
	auto bottom = top + targetEle->GetClientHeight();
	auto totalHeight = targetEle->GetParentNode()->GetClientHeight();
	std::string endTime;
	auto func = [&totalHeight](int p) {
		double h1 = 24 * p / totalHeight;
		double m1 = 60 * (h1 - (int)h1);
		if (m1 > 56) {
			h1 += 1;
			m1 = 0;
		}
		else if (m1 < 4) {
			m1 = 0;
		}
		auto hStr = h1 < 10 ? ("0" + std::to_string((int)h1)) : std::to_string((int)h1);
		auto mStr = m1 < 10 ? ("0" + std::to_string((int)m1)) : std::to_string((int)m1);
		return std::format("{0}:{1}", hStr, mStr);
	};
	std::string timeStr = std::format("{0}-{1}", func(top), func(bottom));
	targetEle->GetChild(1)->GetChild(0)->SetInnerRML(timeStr);
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
			auto eleClassName = ele->GetClassNames();
			if (eleClassName == "todoContent") {
				targetEle = ele->GetParentNode();
				targetEleHeight = targetEle->GetClientHeight();
				dragType = 1;
				mousePointTopSpan = event.GetUnprojectedMouseScreenPos().y - targetEle->GetAbsoluteTop() + 50;//50是标题栏的高度
				mousePointLeftSpan = event.GetUnprojectedMouseScreenPos().x - targetEle->GetAbsoluteLeft() + 499; //左边距
				document->AddEventListener(Rml::EventId::Mousemove, this);
				document->AddEventListener(Rml::EventId::Mouseup, this);
			}
			else if (eleClassName == "todoDragBottom") {
				targetEle = ele->GetParentNode();
				targetEleHeight = targetEle->GetClientHeight();
				dragType = 2;
				document->AddEventListener(Rml::EventId::Mousemove, this);
				document->AddEventListener(Rml::EventId::Mouseup, this);
			}
			else if (eleClassName == "todoDragTop") {
				targetEle = ele->GetParentNode();
				targetEleHeight = targetEle->GetClientHeight();
				dragType = 3;
				document->AddEventListener(Rml::EventId::Mousemove, this);
				document->AddEventListener(Rml::EventId::Mouseup, this);
			}
			break;
		}
		case Rml::EventId::Mousemove: {
			if (ele == document) {
				//todo 动态颜色
				if (dragType == 1) {
					auto mouseY = event.GetUnprojectedMouseScreenPos().y;
					auto yPoint = mouseY - mousePointTopSpan; 
					if (yPoint < 3) yPoint = 3;
					auto bottom = targetEle->GetParentNode()->GetClientHeight() - yPoint - targetEleHeight;
					targetEle->SetProperty(Rml::PropertyId::Top, Rml::Property(yPoint, Rml::Property::PX));
					targetEle->SetProperty(Rml::PropertyId::Bottom, Rml::Property(bottom, Rml::Property::PX));
					if (viewMode == 2) {
						auto xPoint = event.GetUnprojectedMouseScreenPos().x - mousePointLeftSpan;
						targetEle->SetProperty(Rml::PropertyId::Left, Rml::Property(xPoint, Rml::Property::PX));
					}
				}
				else if (dragType == 2) {
					auto height = event.GetUnprojectedMouseScreenPos().y - 50;
					auto bottom = targetEle->GetParentNode()->GetClientHeight() - height - 3 ;
					if (bottom < 3) {
						bottom = 3;
						document->RemoveEventListener(Rml::EventId::Mousemove, this);
						document->RemoveEventListener(Rml::EventId::Mouseup, this);
					}
					if (targetEle->GetParentNode()->GetClientHeight() - targetEle->GetProperty(Rml::PropertyId::Top)->value.Get<int>() - bottom <= 20) {
						bottom = targetEle->GetParentNode()->GetClientHeight() - targetEle->GetProperty(Rml::PropertyId::Top)->value.Get<int>() - 20;
					}
					targetEle->SetProperty(Rml::PropertyId::Bottom, Rml::Property(bottom, Rml::Property::PX));
				}
				else if (dragType == 3) {
					auto top = event.GetUnprojectedMouseScreenPos().y - 50 - 3;
					if (top < 3) {
						top = 3;
					}
					if (targetEle->GetParentNode()->GetClientHeight() - targetEle->GetProperty(Rml::PropertyId::Bottom)->value.Get<int>() - top <= 20) {
						top = targetEle->GetParentNode()->GetClientHeight() - targetEle->GetProperty(Rml::PropertyId::Bottom)->value.Get<int>() - 20;
					}
					targetEle->SetProperty(Rml::PropertyId::Top, Rml::Property(top, Rml::Property::PX));
					
				}
				updateTargetTime();
			}
			break;
		}
		case Rml::EventId::Mouseup: {
			if (ele == document) {
				document->RemoveEventListener(Rml::EventId::Mousemove, this);
				document->RemoveEventListener(Rml::EventId::Mouseup, this);
				if (viewMode == 2 && dragType == 1) {
					auto left = targetEle->GetAbsoluteLeft() - 499;
					int index = left / (targetEle->GetParentNode()->GetClientWidth() / 7);
					targetEle->SetProperty(Rml::PropertyId::Left, Rml::Property(14.2857*index, Rml::Property::PERCENT));
				}
			}
			break;
		}
		default:
			break;
	}
}