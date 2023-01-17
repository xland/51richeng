#pragma once
#include <RmlUi/Core.h>
#include <Windows.h>
#include <windowsx.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

class WindowBase :public Rml::EventListener
{
public:
	WindowBase(int width, int height, std::wstring&& windowTitle, std::string&& windowName);
	WindowBase(const WindowBase&) = delete;
	WindowBase& operator=(const WindowBase&) = delete;
	LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	int width, height;
	std::wstring windowTitle;
	std::string windowName;
	Rml::Context* context;
private:
	HWND hwnd;
	GLFWwindow* window;
	WNDPROC OldProc;
	int glfw_active_modifiers;
	void initGLFWwindow();
	void framelessWindow();
	LRESULT hitTest(HWND hwnd, LPARAM lParam);
	void setupCallbacks();
};

