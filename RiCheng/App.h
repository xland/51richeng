#pragma once
#include <vector>
#include <RmlUi/Core.h>
#include "ShellFileInterface.h"
#include "RmlUi_Platform_GLFW.h"
#include "WindowBase.h"
class App
{
public:
	App(const App&) = delete;
	App& operator=(const App&) = delete;
	/// <summary>
	/// ��ȡAppȫ��ʵ��
	/// </summary>
	/// <returns></returns>
	static App* Get();
	/// <summary>
	/// ��ʼ��App����
	/// </summary>
	static void Init();
	/// <summary>
	/// ������Ϣѭ��������ѭ��
	/// </summary>
	static void Start();
	/// <summary>
	/// �ͷ���Դ
	/// </summary>
	static void Dispose();
	/// <summary>
	/// �Ƴ�ĳ������
	/// </summary>
	/// <param name="window"></param>
	void RemoveWindow(WindowBase* window);
	SystemInterface_GLFW* systemInterface;
	ShellFileInterface* fileInterface;
	std::vector<WindowBase*> windows;
private:
	/// <summary>
	/// ���캯����˽�У�ֻ�ܴ�Init��������
	/// </summary>
	App();
	/// <summary>
	/// ��ʼ��GLFW
	/// </summary>
	void initGLFW();
	/// <summary>
	/// ��ʼ��Ĭ�����壺΢���ź�,����ͼ������
	/// </summary>
	void initFont();
	inline static App* instance{ nullptr };
};

