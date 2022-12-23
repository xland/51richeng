#pragma once
#include <RmlUi/Core.h>

class EventProcessor : public Rml::EventListener
{
public:
	EventProcessor(const Rml::String& value);
	void ProcessEvent(Rml::Event& event) override;
	void OnDetach(Rml::Element* /*element*/) override { delete this; }
private:
	Rml::String eventId;
};

