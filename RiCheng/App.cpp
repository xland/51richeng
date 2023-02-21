#include "App.h"
#include "AppData.h"
#include "spdlog/spdlog.h"
#include "RmlUi_Renderer_GL3.h"

App* App::Get() {
    return instance;
}

void App::Init() {
    instance = new App();
}

App::App() {
	AppData::init();
	initGLFW();
    fileInterface = new ShellFileInterface();
    Rml::SetFileInterface(fileInterface);    
    systemInterface = new SystemInterface_GLFW();
    Rml::SetSystemInterface(systemInterface);
    Rml::Initialise();	
	initFont();
}

void App::initGLFW() {
	glfwSetErrorCallback([](int error, const char* description) {
		spdlog::error("GLFW 错误 code:{0},info:{1}", error, description);
	});
	if (!glfwInit()) {
		spdlog::error("glfwInit失败");
	}
	// Set window hints for OpenGL 3.3 Core context creation.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	// Request stencil buffer of at least 8-bit size to supporting clipping on transformed elements.
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	// Enable MSAA for better-looking visuals, especially when transforms are applied.
	glfwWindowHint(GLFW_SAMPLES, 8);
	// Apply window properties and create it.
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
}

void App::initFont() {
	Rml::LoadFontFace("ui/iconfont.ttf", true);
	UINT size = GetWindowsDirectory(NULL, 0);
	wchar_t* path = new wchar_t[size];
	if (GetWindowsDirectory(path, size) == 0) {
		return;
	}
	auto systemFontPath = std::filesystem::path(path);
	systemFontPath.append(L"Fonts\\msyh.ttc");  //微软雅黑
	Rml::LoadFontFace(systemFontPath.string());
}

void App::Start() {
	while (instance->windows.size() > 0)
	{
		//循环过程中可能会添加新的元素和删除来的元素
		for (int i = 0; i < instance->windows.size();i++) {  
			auto item = instance->windows.at(i);
			glfwPollEvents();
			bool result = glfwWindowShouldClose(item->glfwWindow);
			if (result) {
				glfwDestroyWindow(item->glfwWindow);
				instance->windows.erase(instance->windows.begin()+i);
			}
			else
			{
				item->ProcessEvents();
			}
		}
	}
}

void App::Dispose() {
	Rml::Shutdown();
	RmlGL3::Shutdown();
	glfwTerminate();
	delete instance->fileInterface;
	delete instance->systemInterface;
	delete instance;
}
