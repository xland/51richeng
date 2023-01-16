#pragma once
#include <RmlUi/Core.h>
#include <string>
#include <map>
#include <chrono>
#include "resource.h"
using namespace std::chrono;
namespace ResourceHelper
{
	static std::map<std::string, int> ResourcePathMap {
		{"ui/main.rml",IDR_Main},
		{"ui/style.rcss",IDR_Style},
		{"ui/calendarSmall.rml",IDR_CalendarSmall},
		{"ui/viewDay.rml",IDR_ViewDay},
		{"ui/viewMonth.rml",IDR_ViewMonth},
		{"ui/viewWeek.rml",IDR_ViewWeek},
		{"ui/iconfont.ttf",IDR_Icon}
	};
}

