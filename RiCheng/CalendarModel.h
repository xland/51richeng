#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <mutex>
#include <chrono>
#include <tuple>
#include "TypeDefine.h"
#include <RmlUi/Core.h>

class CalendarModel
{
public:
	CalendarModel();
	CalendarModel(const CalendarModel&) = delete;
	CalendarModel& operator=(const CalendarModel&) = delete;
	const int daySecondCount = 86400;
	CalendarDay currentDay;
	std::vector<CalendarItem> data;
	static std::shared_ptr<CalendarModel> get();
	void setModel(int year, unsigned int month);
	void registNewDayEventObj(Rml::EventListener* obj);
	Rml::EventId NewDayEventId;	
	Rml::EventId SwitchMonthEventId;
private:
	inline static std::shared_ptr<CalendarModel> instance{ nullptr };
	void setItem(int year, unsigned month, unsigned day, bool flag);
	void timerFunc(int secondCount);
	std::mutex mylock;
	std::vector<Rml::EventListener*> newDayProcessor;
};

