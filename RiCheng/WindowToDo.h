#pragma once
#include <RmlUi/Core.h>
#include "WindowBase.h"
class WindowToDo :public WindowBase
{
public:
	WindowToDo(int width, int height);
	bool IsMouseInCaptionArea(int x, int y) override;
	void WindowShowNormal() override;
	void initDocument();
	WindowToDo(const WindowToDo&) = delete;
	WindowToDo& operator=(const WindowToDo&) = delete;
	void ProcessEvent(Rml::Event& event) override;
private:
	Rml::ElementDocument* document;
};

