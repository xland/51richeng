#include "ViewWeek.h"
ViewWeek::ViewWeek() {
	auto context = Rml::GetContext("main");
	document = context->LoadDocument("ui/viewWeek.rml");
	document->SetId("viewWeek");
	document->SetProperty(Rml::PropertyId::Top, Rml::Property(50, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::Left, Rml::Property(400, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::ZIndex, Rml::Property(1, Rml::Property::NUMBER));
	document->AddEventListener(Rml::EventId::Resize, this);
	document->Show();
	Rml::ElementList list;
	document->QuerySelectorAll(list, ".todoItem > div");
	for (auto& ele : list)
	{
		ele->AddEventListener(Rml::EventId::Mousedown, this);
	}
}
void ViewWeek::ProcessEvent(Rml::Event& event) {

}