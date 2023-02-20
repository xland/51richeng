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
		spdlog::error("GLFW ���� code:{0},info:{1}", error, description);
	});
	if (!glfwInit()) {
		spdlog::error("glfwInitʧ��");
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
	systemFontPath.append(L"Fonts\\msyh.ttc");  //΢���ź�
	Rml::LoadFontFace(systemFontPath.string());
}

void App::Start() {
	//int flag = 0;
	while (instance->windows.size()>0)
	{
		for (auto& win: instance->windows)
		{
			win->ProcessEvents();
		}
		//flag += 1;
	}
	//std::this_thread::sleep_for(std::chrono::seconds(99999999999));
}

void App::Dispose() {
	Rml::Shutdown();
	RmlGL3::Shutdown();
	glfwTerminate();
	delete instance->fileInterface;
	delete instance->systemInterface;
	delete instance;
}

void App::RemoveWindow(WindowBase* window) {
	for (auto it = windows.begin(); it != windows.end();) {
		if (*it == window) {
			it = windows.erase(it);
			break;
		}
	}
}
