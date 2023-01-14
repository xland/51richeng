#include "CalendarSmall.h"
#include "ResourceHelper.h"
#include "Time.h"
#include "CalendarModel.h"
#include "CalendarItem.h"

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
	auto model = CalendarModel::get();
	for (auto& item:model->data)
	{
		dayEle->SetInnerRML(std::to_string(item->day));
		dayEle->SetClassNames(item->isCurrentMonthDay?"curMonthEle":"notCurMonthEle");
		dayEle = dayEle->GetNextSibling();
	}	
}

void CalendarSmall::ProcessEvent(Rml::Event& event) {
}