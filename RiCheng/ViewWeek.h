#pragma once
#include <RmlUi/Core.h>
#include "ViewBase.h"
class ViewWeek : public ViewBase
{
public:
	ViewWeek();
	ViewWeek(const ViewWeek&) = delete;
	ViewWeek& operator=(const ViewWeek&) = delete;
	void ProcessEvent(Rml::Event& event) override;
};

