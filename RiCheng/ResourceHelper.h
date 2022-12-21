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
}

