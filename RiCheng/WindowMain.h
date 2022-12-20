#pragma once
#include <RmlUi/Core.h>
class WindowMain :public Rml::EventListener
{
public:
	WindowMain();
	WindowMain(const WindowMain&) = delete;
	WindowMain& operator=(const WindowMain&) = delete;
	void ProcessEvent(Rml::Event& event) override;
private:
	Rml::ElementDocument* document;
};

