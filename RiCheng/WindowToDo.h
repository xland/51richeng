#pragma once
#include <RmlUi/Core.h>
class WindowToDo :public Rml::EventListener
{
public:
	WindowToDo(const WindowToDo&) = delete;
	WindowToDo& operator=(const WindowToDo&) = delete;
	void ProcessEvent(Rml::Event& event) override;
private:
	Rml::ElementDocument* document;
};

