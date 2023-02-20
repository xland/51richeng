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
	/// 获取App全局实例
	/// </summary>
	/// <returns></returns>
	static App* Get();
	/// <summary>
	/// 初始化App对象
	/// </summary>
	static void Init();
	/// <summary>
	/// 开启消息循环，无限循环
	/// </summary>
	static void Start();
	/// <summary>
	/// 释放资源
	/// </summary>
	static void Dispose();
	/// <summary>
	/// 移除某个窗口
	/// </summary>
	/// <param name="window"></param>
	void RemoveWindow(WindowBase* window);
	SystemInterface_GLFW* systemInterface;
	ShellFileInterface* fileInterface;
	std::vector<WindowBase*> windows;
private:
	/// <summary>
	/// 构造函数，私有，只能从Init方法调用
	/// </summary>
	App();
	/// <summary>
	/// 初始化GLFW
	/// </summary>
	void initGLFW();
	/// <summary>
	/// 初始化默认字体：微软雅黑,还有图标字体
	/// </summary>
	void initFont();
	inline static App* instance{ nullptr };
};

