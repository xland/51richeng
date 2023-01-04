#include "CalendarSmall.h"
#include "ResourceHelper.h"

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
	date::year_month_day firstDay{ ResourceHelper::now.year(),ResourceHelper::now.month(),date::day{1} };
	date::year_month_day_last lastDay{ ResourceHelper::now.year(),date::month_day_last{ResourceHelper::now.month()} };
	iso_week::year_weeknum_weekday weekNumWeekDay{ firstDay };
	auto weekDay = (unsigned)(weekNumWeekDay.weekday()) - 1;
	if (weekDay != 0) {
		date::year_month_day_last temp{ ResourceHelper::now.year() ,date::month_day_last { --ResourceHelper::now.month() } };
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

void CalendarSmall::ProcessEvent(Rml::Event& event) {
}