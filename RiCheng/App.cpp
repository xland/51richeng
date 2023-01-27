#include "App.h"
#include "AppData.h"
#include "spdlog/spdlog.h"
#include "RmlUi_Renderer_GL3.h"
#include <mutex>

std::mutex locker;

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

}
void App::initGlfw() {
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
void App::start() {
	size_t size = instance->windows.size();
	while (size > 0)
	{		
		for (int i = 0; i < size; i++) {
			auto win = instance->windows.at(i);
			glfwPollEvents();
			bool result = glfwWindowShouldClose(win->glfwWindow);
			if (result) {
				instance->closeWindow(win);
			}
			else {
				glfwMakeContextCurrent(win->glfwWindow);
				win->context->Update();
				win->renderInterface->BeginFrame();
				win->renderInterface->Clear();
				win->context->Render();
				win->renderInterface->EndFrame();
				glfwSwapBuffers(win->glfwWindow);
			}
		}
		size = instance->windows.size();
	}
}
void App::dispose() {
	Rml::Shutdown();
	RmlGL3::Shutdown();
	glfwTerminate();
	delete instance->fileInterface;
	delete instance->systemInterface;
	delete instance;
}
void App::closeWindow(WindowBase* window) {
	for (auto it = windows.begin(); it != windows.end();) {
		if (*it == window) {
			it = windows.erase(it);
			break;
		}
	}
	window->Dispose();
	auto size = windows.size();
	delete window;
}

void App::newWindow(WindowBase* window) {
	windows.push_back(window);
}
