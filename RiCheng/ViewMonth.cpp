#include "ViewMonth.h"

ViewMonth::ViewMonth() {
	auto context = Rml::GetContext("main");
	document = context->LoadDocument("ui/viewMonth.rml");
	document->SetId("viewMonth");
	document->SetProperty(Rml::PropertyId::Top, Rml::Property(50, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::Left, Rml::Property(400, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::Right, Rml::Property(0, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::Bottom, Rml::Property(0, Rml::Property::PX));
	document->SetProperty(Rml::PropertyId::ZIndex, Rml::Property(2, Rml::Property::NUMBER));
	document->Show();
	Rml::ElementList list;
	document->QuerySelectorAll(list, ".todoItem > div");
	for (auto& ele : list)
	{
		ele->AddEventListener(Rml::EventId::Mousedown, this);
	}
}
void ViewMonth::ProcessEvent(Rml::Event& event) {

}