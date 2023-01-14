#include "CalendarModel.h"
#include "Time.h"


CalendarModel::CalendarModel() :ym{Time::getCurrentYearMonth()} {
	setModel();
}
void CalendarModel::setModel(std::chrono::year_month& ymParam) {
	ym = ymParam;
	setModel();
}
void CalendarModel::setItem(unsigned int day,bool flag) {
	auto item = new CalendarItem();
	item->day = day;
	item->isToday = Time::isToday(year_month_day{ ym.year(),ym.month(),std::chrono::day{day} });
	item->isCurrentMonthDay = false;
	data.push_back(item);
}
void CalendarModel::setModel() {
	auto firstDay = Time::getMonthFirstDay(ym);
	auto lastDay = Time::getMonthLastDay(ym);
	auto dayOfWeek = Time::dayOfWeek(firstDay);
	if (dayOfWeek != 1) {
		year_month preMonth{ Time::currentDay.year(), --Time::currentDay.month() };
		year_month_day preMonthLastDay = Time::getMonthLastDay(preMonth);
		unsigned int temp = (unsigned int)preMonthLastDay.day() - dayOfWeek + 1;
		for (size_t i = 1; i < dayOfWeek; i++)
		{
			setItem(temp+i,false);
		}
	}
	auto curMonthLastDay = (unsigned)lastDay.day();
	for (size_t i = 1; i <= curMonthLastDay; i++)
	{
		setItem(i, true);
	}
	auto lastEleNum = 42 - curMonthLastDay - dayOfWeek + 1;
	for (size_t i = 1; i <= lastEleNum; i++)
	{
		setItem(i, false);
	}
}


std::shared_ptr<CalendarModel> CalendarModel::get() {
    if (instance == nullptr) {
        instance = std::make_shared<CalendarModel>();
    }
    return instance;
}