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
	~App();
	static App* get();
	static void init();
	static void start();
	static void dispose();
	void closeWindow(WindowBase* window);
	SystemInterface_GLFW* systemInterface;
	ShellFileInterface* fileInterface;
	std::vector<WindowBase*> windows;
	void newWindow(WindowBase* window);
private:
	App();
	void initGlfw();
	void initFont();
	inline static App* instance{ nullptr };
};

