#pragma once
#include <RmlUi/Core.h>
#include <Windows.h>
#include <WinUser.h>
class TopBar :public Rml::EventListener
{
public:
	TopBar();
	void ProcessEvent(Rml::Event& event) override;
private:
	Rml::ElementDocument* document;
	void setHitTestArea();
};