#pragma once
#include <map>
#include <RmlUi/Core.h>
#include <GLFW/glfw3.h>
#include "RmlUi_Renderer_GL3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

class WindowBase :public Rml::EventListener
{
public:
	WindowBase(int width, int height,const std::string& windowName);
	/// <summary>
	/// 关闭窗口，同时从全局对象App中移除引用，资源会自动释放
	/// </summary>
	void Close();
	/// <summary>
	/// 窗口自己的事件循环
	/// </summary>
	void ProcessEvents();
	virtual bool IsMouseInCaptionArea(int x,int y) = 0;
	/// <summary>
	/// 窗口从最大化变成还原状态 有可能是用户双击了标题栏
	/// </summary>
	virtual void WindowShowNormal() = 0;
	LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	WindowBase(const WindowBase&) = delete;
	WindowBase& operator=(const WindowBase&) = delete;
	int width;
	int height;
	std::string windowName;
	Rml::Context* context;
	GLFWwindow* glfwWindow;
	RenderInterface_GL3* renderInterface;
	HWND hwnd;
private:
	/// <summary>
	/// 使用GLFW创建窗口
	/// </summary>
	void initGLFWwindow();
	/// <summary>
	/// 截获窗口消息，使窗口成为无边框、无标题栏的窗口
	/// </summary>
	void framelessWindow();
	/// <summary>
	/// 无边框窗口可拖拽区域
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="lParam"></param>
	/// <returns></returns>
	LRESULT hitTest(HWND hwnd, LPARAM lParam);
	void setupCallbacks();
	bool ProcessKeyDownShortcuts(Rml::Context* context, Rml::Input::KeyIdentifier key, int key_modifier, float native_dp_ratio, bool priority);
	WNDPROC oldWindowProc;
	int glfwActiveModifiers = 0;
};