#pragma once
#include <string>

struct Repeat
{
	std::string id;
	/// <summary>
	/// 0ÿ�죬1ÿ�ܣ�2ÿ�£�3ÿ��
	/// </summary>
	short every;
	/// <summary>
	/// 0��������1ָ������ʱ�䣬2ָ���ظ�����
	/// </summary>
	short endType;
	/// <summary>
	/// �ظ�����
	/// </summary>
	int repeatTime;
	/// <summary>
	/// ����ʱ��
	/// </summary>
	long long endTime;
	/// <summary>
	/// ��everyΪ2ʱ��0ÿ�µ�N�죬1ÿ�µ�N�����ڵ�����N��N��ToDo�л�ȡ��
	/// </summary>
	short repeatIndex;

	std::string todoId;
};
