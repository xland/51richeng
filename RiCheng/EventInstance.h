#pragma once
#include <RmlUi/Core.h>
class EventInstance : public Rml::EventListenerInstancer
{
public:
	Rml::EventListener* InstanceEventListener(const Rml::String& value, Rml::Element* /*element*/) override;
};

