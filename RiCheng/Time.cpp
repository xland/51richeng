#include "Time.h"

namespace Time {
	year_month_day getDay(int year1, unsigned int month1, unsigned int day1) {
		year_month_day result{ year{year1},month{month1},day{day1} };
		return result;
	}
	year_month_day getCurrentMonthFirstDay() {
		year_month_day ymd{ floor<days>(system_clock::now()) };
		year_month_day result{ ymd.year(),ymd.month(),day {1} };
		return result;
	}
	year_month_day getMonthFirstDay(year_month_day& ymd) {
		year_month_day result{ ymd.year(),ymd.month(),day {1} };
		return result;
	}
	year_month_day getCurrentMonthLastDay() {
		year_month_day ymd{ floor<days>(system_clock::now()) };
		year_month_day_last ymdl{ ymd.year(),month_day_last{ymd.month()} };
		year_month_day result{ ymdl };
		return result;
	}
	year_month_day getMonthLastDay(year_month_day& ymd) {
		year_month_day_last ymdl{ ymd.year(),month_day_last{ymd.month()} };
		year_month_day result{ ymdl };
		return result;
	}
	year_month_day getMonthLastDay(year year1, month month1) {
		year_month_day_last ymdl{ year1,month_day_last{month1} };
		year_month_day result{ ymdl };
		return result;
	}
	unsigned int dayOfWeek(year_month_day& ymd) {
		//int a = ymd._Calculate_weekday();
		weekday wd{ sys_days{ ymd } };
		return wd.iso_encoding();
	}
	int toSecondCount(int h, int m, int s) {
		hours a{ h };
		minutes b{ m };
		seconds c{ s };
		duration d = a + b + c;
		return d.count();
	}
	hh_mm_ss<seconds> fromSecondCount(int d) {
		hh_mm_ss hms{ seconds{d} };
		return hms;
	}
	int toDayCount(year_month_day& ymd) {
		auto sc = sys_days{ ymd };
		auto s = sc.time_since_epoch();
		return s.count();
	}
	year_month_day fromDayCount(int dayCount) {
		sys_days sd{ days{dayCount} };
		year_month_day ymd{ sd };
		return ymd;
	}
}


