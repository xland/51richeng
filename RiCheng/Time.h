#pragma once
#include <RmlUi/Core.h>
#include <chrono>
#include <vector>
#include <functional>
#include <mutex>
using namespace std::chrono;
namespace Time {
	/// <summary>
	/// 24小时的秒数
	/// </summary>
	const int daySecondCount = 86400;
	static inline year_month_day currentDay{ floor<days>(system_clock::now()) };
	year_month_day getDay(int year1, unsigned int month1, unsigned int day1);
	year_month_day getCurrentMonthFirstDay();
	year_month_day getMonthFirstDay(year_month& ymd);
	year_month_day getCurrentMonthLastDay();
	year_month_day getMonthLastDay(year_month& ymd);
	year_month getCurrentYearMonth();
	bool isToday(year_month_day ymd);
	unsigned int dayOfWeek(year_month_day& ymd);
	int toSecondCount(int h, int m, int s);
	hh_mm_ss<seconds> fromSecondCount(int d);
	int toDayCount(year_month_day& ymd);
	year_month_day fromDayCount(int dayCount);
	void registNewDayEventObj(Rml::EventListener*);
	Rml::EventId getNewDayEventId();
}
