#pragma once
#include <string>
#include <map>
#include <chrono>
#include "date/date.h"
#include "date/iso_week.h"
#include "resource.h"
using namespace std::chrono;
namespace ResourceHelper
{
	static std::map<std::string, int> ResourcePathMap {
		{"ui/main.rml",IDR_Main},
		{"ui/calendarSmall.rml",IDR_CalendarSmall},
		{"ui/viewDay.rml",IDR_ViewDay},
		{"ui/viewMonth.rml",IDR_ViewMonth},
		{"ui/viewWeek.rml",IDR_ViewWeek},
		{"ui/iconfont.ttf",IDR_Icon}
	};
	static std::map<std::string, const char*> IconMap{
		{"appIcon",(const char*)u8"\ue685"},
		{"closeBtn",(const char*)u8"\ue6e7"},
		{"maximizeBtn",(const char*)u8"\ue6e5"},
		{"minimizeBtn",(const char*)u8"\ue6e8"},
		{"preBtn",(const char*)u8"\ue60e"},
		{"nextBtn",(const char*)u8"\ue638"},
		{"switchDropDownBtn",(const char*)u8"\ue60f"},
		{"restoreBtn",(const char*)u8"\ue6e9"},
	};
	static date::year_month_day now { floor<days>(system_clock::now()) };
}

