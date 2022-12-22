#pragma once
#include <string>
#include <map>
#include "resource.h"
namespace ResourceHelper
{
	static std::map<std::string, int> ResourcePathMap {
		{"ui/main.rml",IDR_MAIN},
		{"ui/iconfont.ttf",IDR_ICON}
	};
	static std::map<std::string, const char*> IconMap{
		{"appIcon",(const char*)u8"\ue685"},
		{"closeBtn",(const char*)u8"\ue6e7"},
		{"maximizeBtn",(const char*)u8"\ue6e5"},
		{"minimizeBtn",(const char*)u8"\ue6e8"},
		{"preBtn",(const char*)u8"\ue60e"},
		{"nextBtn",(const char*)u8"\ue638"},
		{"switchDropDownBtn",(const char*)u8"\ue60f"},
	};
}

