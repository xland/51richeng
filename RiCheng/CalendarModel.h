#pragma once
#include <memory>
#include <chrono>
#include "CalendarItem.h"

class CalendarModel
{
public:
	CalendarModel(const CalendarModel&) = delete;
	CalendarModel& operator=(const CalendarModel&) = delete;
	static std::shared_ptr<CalendarModel> get();
	void setModel(std::chrono::year_month& ymParam);
	std::vector<CalendarItem*> data;
	std::chrono::year_month ym;
	CalendarModel();
	void setModel();
	void setItem(unsigned int day, bool flag);
private:
	inline static std::shared_ptr<CalendarModel> instance{ nullptr };
};

