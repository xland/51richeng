#include "CalendarModel.h"
#include <chrono>
#include <thread>
using namespace std::chrono;

void preNextMonth(int& year1, unsigned& month1) {
	if (month1 < 1) {
		year1 = year1 - 1;
		month1 = 12;
	}
	else if (month1 > 12) {
		year1 = year1 + 1;
		month1 = 1;
	}
}
year_month_day getMonthLastDay(int year1,unsigned month1) {
	preNextMonth(year1, month1);
	year_month_day_last ymdl{ year{year1},month_day_last{month{month1}} };
	year_month_day result{ ymdl };
	return result;
}

CalendarModel::CalendarModel() {
	std::time_t time = std::time(nullptr);
	tm now;
	localtime_s(&now, &time);
	auto secondCount = daySecondCount - now.tm_sec - 60 * now.tm_min - 3600 * now.tm_hour;
	currentDay = { now.tm_year + 1900,now.tm_mon + 1,now.tm_mday,now.tm_wday == 0 ? 7 : now.tm_wday };
	setModel(std::get<0>(currentDay), std::get<1>(currentDay));

	NewDayEventId = Rml::RegisterEventType("NewDayEvent", false, false);
	SwitchMonthEventId = Rml::RegisterEventType("SwitchMonthEvent", false, false);
	std::thread t(&CalendarModel::timerFunc, this,secondCount);
	t.detach();
}

void CalendarModel::setItem(int year1,unsigned month1,unsigned day1,bool flag) {
	preNextMonth(year1, month1);
	bool isToday = std::get<0>(currentDay) == year1 &&
		std::get<1>(currentDay) == month1 &&
		std::get<2>(currentDay) == day1;
	data.push_back({ day1,isToday,flag });
}
void CalendarModel::setModel(int year1,unsigned int month1) {
	auto firstDay = year_month_day{ year{year1},month{month1},day{1} };
	auto lastDay = getMonthLastDay(year1,month1);
	auto dayOfWeek = firstDay._Calculate_weekday()==0 ? 7 : firstDay._Calculate_weekday();
	if (dayOfWeek != 1) {
		year_month_day preMonthLastDay = getMonthLastDay(year1, month1-1);
		unsigned temp = (unsigned)preMonthLastDay.day() - dayOfWeek + 1;
		for (size_t i = 1; i < dayOfWeek; i++)
		{
			setItem((int)preMonthLastDay.year(),(unsigned)preMonthLastDay.month(), temp + i, false);
		}
	}
	auto curMonthLastDay = (unsigned)lastDay.day();
	for (size_t i = 1; i <= curMonthLastDay; i++)
	{
		setItem(year1, month1, i, true);
	}
	auto lastEleNum = 42 - curMonthLastDay - dayOfWeek + 1;
	for (size_t i = 1; i <= lastEleNum; i++)
	{
		setItem(year1, month1+1, i, false);
	}
}

void CalendarModel::registNewDayEventObj(Rml::EventListener* obj) {
	mylock.lock();
	newDayProcessor.push_back(obj);
	mylock.unlock();
}
void CalendarModel::timerFunc(int secondCount) {
	std::this_thread::sleep_for(seconds(secondCount + 6));
	mylock.lock();
	for (auto& processor : newDayProcessor)
	{
		Rml::Dictionary dic;
		Rml::Event e(nullptr, NewDayEventId, "", dic, false);
		processor->ProcessEvent(e);
	}
	mylock.unlock();
	timerFunc(daySecondCount);
}

std::shared_ptr<CalendarModel> CalendarModel::get() {
    if (instance == nullptr) {
        instance = std::make_shared<CalendarModel>();
    }
    return instance;
}