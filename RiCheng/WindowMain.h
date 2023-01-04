#pragma once
#include <RmlUi/Core.h>
#include <chrono>
#include <memory>
#include "CalendarSmall.h"
#include "ViewDay.h"

using namespace std::chrono;

class WindowMain :public Rml::EventListener
{
public:
	WindowMain();
	WindowMain(const WindowMain&) = delete;
	WindowMain& operator=(const WindowMain&) = delete;
	void ProcessEvent(Rml::Event& event) override;
private:
	Rml::ElementDocument* document;
	std::unique_ptr<CalendarSmall> calendarSmall;
	std::unique_ptr<ViewDay> viewDay;
	
	inline Rml::Element* setEleIcon(std::string&& id);
	inline void setBtn();
	inline void initDocument();
	inline void initCurDate();
	inline bool windowToolBtnEventProcess(std::string& eleId, Rml::Element* ele);
	inline bool switchViewModeProcess(std::string& eleId, Rml::Element* ele);
	
};

