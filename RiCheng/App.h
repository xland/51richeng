#pragma once
#include <RmlUi/Core.h>
#include "ShellFileInterface.h"
#include "RmlUi_Platform_GLFW.h"
#include "RmlUi_Renderer_GL3.h"
class App
{
public:
	App(const App&) = delete;
	App& operator=(const App&) = delete;
	~App();
	static App* get();
	static void init();
	SystemInterface_GLFW* systemInterface;
	RenderInterface_GL3* renderInterface;
	ShellFileInterface* fileInterface;
private:
	App();
	inline static App* instance{ nullptr };
};

