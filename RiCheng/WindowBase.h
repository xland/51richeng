#pragma once
#include <map>
#include <RmlUi/Core.h>
#include <Windows.h>
#include <windowsx.h>
#include <GLFW/glfw3.h>
#include "RmlUi_Renderer_GL3.h"
#include "RmlUi_Backend.h"
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
	static inline std::map<GLFWwindow*, WindowBase*> winMap;
private:
	HWND hwnd;
	GLFWwindow* window;
	WNDPROC OldProc;
	int glfw_active_modifiers = 0;
	bool context_dimensions_dirty = true;
	bool needProcessEvent;
	void initGLFWwindow();
	void framelessWindow();
	LRESULT hitTest(HWND hwnd, LPARAM lParam);
	void setupCallbacks();
	bool ProcessEvents(Rml::Context* context);
	bool ProcessKeyDownShortcuts(Rml::Context* context, Rml::Input::KeyIdentifier key, int key_modifier, float native_dp_ratio, bool priority);
};

