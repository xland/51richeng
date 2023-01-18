#include "CalendarSmall.h"
#include "ResourceHelper.h"
#include "CalendarModel.h"
#include "TypeDefine.h"

CalendarSmall::CalendarSmall() {
	auto context = Rml::GetContext("windowMain");
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
	auto model = CalendarModel::get();
	for (auto[day,curDay,curMonth] : model->data)
	{
		dayEle->SetInnerRML(std::to_string(day));
		if (curDay) {
			dayEle->SetClass("curDayEle", curDay);
		}
		else
		{
			dayEle->SetClass("notCurMonthEle", !curMonth);
			dayEle->SetClass("curMonthEle", curMonth);
		}		
		dayEle = dayEle->GetNextSibling();
	}	
}

void CalendarSmall::ProcessEvent(Rml::Event& event) {
}