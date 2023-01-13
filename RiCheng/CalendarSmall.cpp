#include "CalendarSmall.h"
#include "ResourceHelper.h"
#include "Time.h"

CalendarSmall::CalendarSmall() {
	auto context = Rml::GetContext("main");
	document = context->LoadDocument("ui/calendarSmall.rml");
	document->SetId("calendarSmall");
	document->SetProperty(Rml::PropertyId::Top, Rml::Property(50, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::Width, Rml::Property(400, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::Height, Rml::Property(342, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::ZIndex, Rml::Property(2, Rml::Property::NUMBER));
	document->Show();
	initCalendar();
}

void CalendarSmall::initCalendar() {
	auto dayEle = document->GetElementById("calendarEleBox")->GetChild(0);
	auto firstDay = Time::getCurrentMonthFirstDay();
	auto lastDay = Time::getCurrentMonthLastDay();
	auto dayOfWeek = Time::dayOfWeek(firstDay);
	if (dayOfWeek != 1) {
		year_month_day preMonthLastDay = Time::getMonthLastDay(Time::currentDay.year(), --Time::currentDay.month());	
		auto temp = (unsigned)preMonthLastDay.day() - dayOfWeek +1;
		for (size_t i = 1; i < dayOfWeek; i++)
		{
			dayEle->SetInnerRML(std::to_string(temp+i));
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
	auto lastEleNum = 42 - curMonthLastDay - dayOfWeek+1;
	for (size_t i = 1; i <= lastEleNum; i++)
	{
		dayEle->SetInnerRML(std::to_string(i));
		dayEle->SetClassNames("notCurMonthEle");
		dayEle = dayEle->GetNextSibling();
	}
}

void CalendarSmall::ProcessEvent(Rml::Event& event) {
}