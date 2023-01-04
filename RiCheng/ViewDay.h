#pragma once
#include <RmlUi/Core.h>
class ViewDay :public Rml::EventListener
{
public:
	ViewDay();
	ViewDay(const ViewDay&) = delete;
	ViewDay& operator=(const ViewDay&) = delete;
	void ProcessEvent(Rml::Event& event) override;
private:
	void updateTargetTime();
	int mousePointTopSpan;
	int mousePointLeftSpan;
	Rml::Element* targetEle;
	int targetEleHeight;
	int dragType;
	int viewMode = 1;
	Rml::ElementDocument* document;
};

