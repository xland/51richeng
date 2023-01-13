#pragma once
#include <string>

struct Repeat
{
	std::string id;
	/// <summary>
	/// 0每天，1每周，2每月，3每年
	/// </summary>
	short every;
	/// <summary>
	/// 0不结束，1指定结束时间，2指定重复次数
	/// </summary>
	short endType;
	/// <summary>
	/// 重复次数
	/// </summary>
	int repeatTime;
	/// <summary>
	/// 结束时间
	/// </summary>
	long long endTime;
	/// <summary>
	/// （every为2时）0每月第N天，1每月第N个星期的星期N（N从ToDo中获取）
	/// </summary>
	short repeatIndex;

	std::string todoId;
};
