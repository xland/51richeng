#include "App.h"
#include "AppData.h"

App* App::get() {
    return instance;
}
void App::init() {
    instance = new App();
}
App::App() {
    AppData::init();
    fileInterface = new ShellFileInterface();
    Rml::SetFileInterface(fileInterface);    
    systemInterface = new SystemInterface_GLFW();
    Rml::SetSystemInterface(systemInterface);
    renderInterface = new RenderInterface_GL3();
    Rml::SetRenderInterface(renderInterface);
    Rml::Initialise();
}
App::~App() {
    Rml::Shutdown();
    delete fileInterface;
    delete systemInterface;
    delete renderInterface;
    delete AppData::get();
}
