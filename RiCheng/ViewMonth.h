#pragma once
#include <RmlUi/Core.h>
#include "ViewBase.h"
class ViewMonth : public ViewBase
{
public:
	ViewMonth();
	ViewMonth(const ViewMonth&) = delete;
	ViewMonth& operator=(const ViewMonth&) = delete;
	void ProcessEvent(Rml::Event& event) override;
private:
};

