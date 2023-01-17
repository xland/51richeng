#pragma once
#include <RmlUi/Core.h>
class WindowBase :public Rml::EventListener
{
public:
	WindowBase(int width, int height, std::string& name);
	WindowBase(const WindowBase&) = delete;
	WindowBase& operator=(const WindowBase&) = delete;
private:

};

