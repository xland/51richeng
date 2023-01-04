#pragma once
#include <RmlUi/Core.h>
#include "ViewBase.h"
class ViewDay :public ViewBase
{
public:
	ViewDay();
	ViewDay(const ViewDay&) = delete;
	ViewDay& operator=(const ViewDay&) = delete;
	void ProcessEvent(Rml::Event& event) override;
private:
	void updateTargetTime();
	void processMouseMove(const Rml::Vector2f& mousePoint);
	void processMouseDown(const std::string& className, const Rml::Vector2f& mousePoint);
	int mousePointTopSpan = 0;
	int mousePointLeftSpan = 0;
	Rml::Element* targetEle = nullptr;
	int targetEleHeight = 0;
	int dragType = 0;
};

