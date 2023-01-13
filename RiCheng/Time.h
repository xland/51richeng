#pragma once
#include <chrono>
using namespace std::chrono;
namespace Time {
	/// <summary>
	/// 24小时的秒数
	/// </summary>
	const int daySecondCount = 86400;
	inline year_month_day currentDay{ floor<days>(system_clock::now()) };
	year_month_day getDay(int year1, unsigned int month1, unsigned int day1);
	year_month_day getCurrentMonthFirstDay();
	year_month_day getMonthFirstDay(year_month_day& ymd);
	year_month_day getCurrentMonthLastDay();
	year_month_day getMonthLastDay(year_month_day& ymd);
	year_month_day getMonthLastDay(year year1, month month1);
	unsigned int dayOfWeek(year_month_day& ymd);
	int toSecondCount(int h, int m, int s);
	hh_mm_ss<seconds> fromSecondCount(int d);
	int toDayCount(year_month_day& ymd);
	year_month_day fromDayCount(int dayCount);

}
