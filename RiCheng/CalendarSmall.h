#pragma once
#include <RmlUi/Core.h>
#include <chrono>

using namespace std::chrono;
class CalendarSmall :public Rml::EventListener
{
public:
	CalendarSmall();
	CalendarSmall(const CalendarSmall&) = delete;
	CalendarSmall& operator=(const CalendarSmall&) = delete;
	void ProcessEvent(Rml::Event& event) override;
private:
	void initCalendar();
	Rml::ElementDocument* document;
};

