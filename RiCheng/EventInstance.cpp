#include "EventInstance.h"
#include "EventProcessor.h"

Rml::EventListener* EventInstance::InstanceEventListener(const Rml::String& value, Rml::Element* /*element*/)
{
	return new EventProcessor(value);
}