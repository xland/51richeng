#pragma once
#include <RmlUi/Core.h>
#include <chrono>
#include <memory>
#include "CalendarSmall.h"
#include "ViewDay.h"
#include "ViewWeek.h"
#include "ViewMonth.h"
#include "WindowBase.h"

using namespace std::chrono;

class WindowMain : public WindowBase
{
public:
	WindowMain(int width, int height);
	WindowMain(const WindowMain&) = delete;
	WindowMain& operator=(const WindowMain&) = delete;
	void ProcessEvent(Rml::Event& event) override;
private:
	Rml::ElementDocument* document;
	std::unique_ptr<CalendarSmall> calendarSmall;
	std::unique_ptr<ViewDay> viewDay;
	std::unique_ptr<ViewWeek> viewWeek;
	std::unique_ptr<ViewMonth> viewMonth;
	inline void setBtn();
	inline void initDocument();
	inline void initCurDate();
	inline bool windowToolBtnEventProcess(std::string& eleId, Rml::Element* ele);
	inline bool switchViewModeProcess(std::string& eleId, Rml::Element* ele);	
};

