#include "Time.h"
#include <mutex>
namespace {
	std::mutex mylock;
	static Rml::EventId NewDayEventId;
	static std::vector<Rml::EventListener*> newDayProcessor;
	void timerFunc() {
		auto now = system_clock::now();
		auto d1 = floor<minutes>(now).time_since_epoch();
		auto dayCount = floor<days>(now).time_since_epoch().count();
		auto d2 = floor<minutes>(days{ dayCount + 1 });
		auto d3 = d2.count() - d1.count() - 480;//480是8小时，VS时区的库是坏的，还不知道为啥 todo
		std::this_thread::sleep_for(minutes(d3+1));
		mylock.lock();
		Time::currentDay = year_month_day{ floor<days>(system_clock::now()) };
		auto size = newDayProcessor.size();
		for (auto& processor : newDayProcessor)
		{
			Rml::Dictionary dic;
			Rml::Event e(nullptr, NewDayEventId, "", dic, false);
			processor->ProcessEvent(e);
		}
		mylock.unlock();
		timerFunc();
	}
}

namespace Time {
	year_month getCurrentYearMonth() {
		year_month_day ymd{ floor<days>(system_clock::now()) };
		return year_month{ ymd.year(),ymd.month() };
	}
	bool isToday(year_month_day ymd) {		
		return currentDay == ymd;
	}
	year_month_day getDay(int year1, unsigned int month1, unsigned int day1) {
		year_month_day result{ year{year1},month{month1},day{day1} };
		return result;
	}
	year_month_day getCurrentMonthFirstDay() {
		year_month_day ymd{ floor<days>(system_clock::now()) };
		year_month_day result{ ymd.year(),ymd.month(),day {1} };
		return result;
	}
	year_month_day getMonthFirstDay(year_month& ymd) {
		year_month_day result{ ymd.year(),ymd.month(),day {1} };
		return result;
	}
	year_month_day getCurrentMonthLastDay() {
		year_month_day ymd{ floor<days>(system_clock::now()) };
		year_month_day_last ymdl{ ymd.year(),month_day_last{ymd.month()} };
		year_month_day result{ ymdl };
		return result;
	}
	year_month_day getMonthLastDay(year_month& ymd) {
		year_month_day_last ymdl{ ymd.year(),month_day_last{ymd.month()} };
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
	void registNewDayEventObj(Rml::EventListener* obj) {
		mylock.lock();
		newDayProcessor.push_back(obj);
		mylock.unlock();
		if (NewDayEventId == Rml::EventId::Invalid) {
			NewDayEventId = Rml::RegisterEventType("NewDayEvent", false, false);
			std::thread t(timerFunc);
			t.detach();
		}
	}
	Rml::EventId getNewDayEventId() {
		return NewDayEventId;
	}
}


