#pragma once
#include <RmlUi/Core.h>
#include <chrono>
#include "date/date.h"
#include "date/iso_week.h"

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
	date::year_month_day today;
	Rml::Vector2f mousePointLeftTopPointSpace;
	inline Rml::Element* setEleIcon(std::string&& id);
	inline void setBtn();
	inline void initDocument();
	inline void initCurDate();
	inline bool windowToolBtnEventProcess(std::string& eleId, Rml::Element* ele);
	inline bool switchViewModeProcess(std::string& eleId, Rml::Element* ele);
	void initCalendar();
};

