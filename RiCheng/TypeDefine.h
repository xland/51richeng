#pragma once
#include <tuple>

/// <summary>
/// 年，月，日，星期几
/// </summary>
using CalendarDay = std::tuple<int, unsigned, unsigned, unsigned>;
/// <summary>
/// 日，是否为今天，是否为当前月
/// </summary>
using CalendarItem = std::tuple<unsigned, bool, bool>;