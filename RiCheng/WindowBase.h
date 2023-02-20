#pragma once
#include <map>
#include <RmlUi/Core.h>
#include <GLFW/glfw3.h>
#include "RmlUi_Renderer_GL3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

class WindowBase :public Rml::EventListener
{
public:
	WindowBase(int width, int height,const std::string& windowName);
	/// <summary>
	/// �رմ��ڣ�ͬʱ��ȫ�ֶ���App���Ƴ����ã���Դ���Զ��ͷ�
	/// </summary>
	void Close();
	/// <summary>
	/// �����Լ����¼�ѭ��
	/// </summary>
	void ProcessEvents();
	virtual bool IsMouseInCaptionArea(int x,int y) = 0;
	/// <summary>
	/// ���ڴ���󻯱�ɻ�ԭ״̬ �п������û�˫���˱�����
	/// </summary>
	virtual void WindowShowNormal() = 0;
	LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	WindowBase(const WindowBase&) = delete;
	WindowBase& operator=(const WindowBase&) = delete;
	int width;
	int height;
	std::string windowName;
	Rml::Context* context;
	GLFWwindow* glfwWindow;
	RenderInterface_GL3* renderInterface;
	HWND hwnd;
private:
	/// <summary>
	/// ʹ��GLFW��������
	/// </summary>
	void initGLFWwindow();
	/// <summary>
	/// �ػ񴰿���Ϣ��ʹ���ڳ�Ϊ�ޱ߿��ޱ������Ĵ���
	/// </summary>
	void framelessWindow();
	/// <summary>
	/// �ޱ߿򴰿ڿ���ק����
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="lParam"></param>
	/// <returns></returns>
	LRESULT hitTest(HWND hwnd, LPARAM lParam);
	void setupCallbacks();
	bool ProcessKeyDownShortcuts(Rml::Context* context, Rml::Input::KeyIdentifier key, int key_modifier, float native_dp_ratio, bool priority);
	WNDPROC oldWindowProc;
	int glfwActiveModifiers = 0;
};