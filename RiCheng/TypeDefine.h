#pragma once
#include <tuple>

/// <summary>
/// �꣬�£��գ����ڼ�
/// </summary>
using CalendarDay = std::tuple<int, unsigned, unsigned, unsigned>;
/// <summary>
/// �գ��Ƿ�Ϊ���죬�Ƿ�Ϊ��ǰ��
/// </summary>
using CalendarItem = std::tuple<unsigned, bool, bool>;