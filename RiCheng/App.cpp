#include "App.h"
#include "AppData.h"
#include "spdlog/spdlog.h"
#include "RmlUi_Renderer_GL3.h"

App* App::get() {
    return instance;
}
void App::init() {
    instance = new App();
}
App::App() {
	AppData::init();
	initGlfw();
    fileInterface = new ShellFileInterface();
    Rml::SetFileInterface(fileInterface);    
    systemInterface = new SystemInterface_GLFW();
    Rml::SetSystemInterface(systemInterface);
    Rml::Initialise();	
	initFont();
}
App::~App() {
    Rml::Shutdown();
	RmlGL3::Shutdown();
	glfwTerminate();
    delete fileInterface;
    delete systemInterface;
    delete AppData::get();
}
void App::initGlfw() {
	glfwSetErrorCallback([](int error, const char* description) {
		spdlog::error("GLFW ´íÎó code:{0},info:{1}", error, description);
	});
	if (!glfwInit()) {
		spdlog::error("glfwInitÊ§°Ü");
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
	systemFontPath.append(L"Fonts\\msyh.ttc");  //Î¢ÈíÑÅºÚ
	Rml::LoadFontFace(systemFontPath.string());
}
void App::start() {
	while (windows.size()>0)
	{
		for (auto& win:windows)
		{
			win->ProcessEvents();
		}		
	}
}
void App::closeWindow(WindowBase* window) {
	for (auto it = windows.begin(); it != windows.end();) {
		if (*it == window) {
			it = windows.erase(it);
			break;
		}
	}
	delete window;
}
