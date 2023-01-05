#pragma once
#include <RmlUi/Core.h>
class ViewBase : public Rml::EventListener
{
public:
	ViewBase() = default;
	ViewBase(const ViewBase&) = delete;
	ViewBase& operator=(const ViewBase&) = delete;
	void show();
	void hide();
	std::string PositionToTime(float totalHeight,float position);
protected:
	Rml::ElementDocument* document = nullptr;
};

