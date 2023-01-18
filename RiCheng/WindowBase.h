#pragma once
#include <map>
#include <RmlUi/Core.h>
#include <GLFW/glfw3.h>
#include "RmlUi_Renderer_GL3.h"
#include "RmlUi_Backend.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

class WindowBase :public Rml::EventListener
{
public:
	WindowBase(int width, int height,const std::string& windowName);
	~WindowBase();
	WindowBase(const WindowBase&) = delete;
	WindowBase& operator=(const WindowBase&) = delete;
	LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void ProcessEvents();
	int width, height;
	//std::wstring windowTitle;
	std::string windowName;
	Rml::Context* context;
	GLFWwindow* glfwWindow;
	RenderInterface_GL3* renderInterface;
	HWND hwnd;
private:
	WNDPROC oldWindowProc;
	int glfw_active_modifiers = 0;
	bool context_dimensions_dirty = true;
	bool needProcessEvent;
	void initGLFWwindow();
	void framelessWindow();
	LRESULT hitTest(HWND hwnd, LPARAM lParam);
	void setupCallbacks();
	bool ProcessKeyDownShortcuts(Rml::Context* context, Rml::Input::KeyIdentifier key, int key_modifier, float native_dp_ratio, bool priority);
};

