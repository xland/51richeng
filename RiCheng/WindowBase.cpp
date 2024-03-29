#include "WindowBase.h"
#include <algorithm>
#include <Windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include "spdlog/spdlog.h"
#include "App.h"


namespace {	
	WindowBase* findWindowByGlfwWindow(GLFWwindow* window) {
		for (auto& win : App::Get()->windows)
		{
			if (win->glfwWindow == window) {
				return win;
			}
		}
		return nullptr;
	}
	LRESULT CALLBACK newWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		WindowBase* win;
		if (msg == WM_CREATE) {
			win = (WindowBase*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)win);
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		else {
			win = (WindowBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (!win) {
				return DefWindowProc(hwnd, msg, wParam, lParam);
			}
		}
		return win->winProc(hwnd, msg, wParam, lParam);	
	}	
}

WindowBase::WindowBase(int width, int height,const std::string& windowName)
	:width{ width }, height{height} , windowName{ windowName }
{
	initGLFWwindow();
	context = Rml::CreateContext(windowName, Rml::Vector2i(width, height),renderInterface);
	if (context == nullptr) {
		spdlog::error("Rml::CreateContext main失败");
	}
	Rml::Vector2i windowSize;
	float dpRatio = 1.f;
	glfwGetFramebufferSize(glfwWindow, &windowSize.x, &windowSize.y);
	glfwGetWindowContentScale(glfwWindow, &dpRatio, nullptr);
	context->SetDimensions(windowSize);
	context->SetDensityIndependentPixelRatio(dpRatio);
	App::Get()->windows.push_back(this);
}

void WindowBase::initGLFWwindow() {
	glfwWindow = glfwCreateWindow(width, height, "", nullptr, nullptr);	
	if (!glfwWindow) {
		spdlog::error("glfwCreateWindow失败");
	}	
	hwnd = glfwGetWin32Window((GLFWwindow*)glfwWindow);	//todo 1558,1208
	framelessWindow();
	//必须重新设置窗口大小，前面设置的都是错的
	//glfw issue https://github.com/glfw/glfw/issues/414
	glfwSetWindowSize(glfwWindow, width, height);
	glfwMakeContextCurrent(glfwWindow);
	glfwSwapInterval(1);
	Rml::String renderer_message;
	if (!RmlGL3::Initialize(&renderer_message)) {
		spdlog::error("RmlGL3::Initialize失败");
	}
	// The window size may have been scaled by DPI settings, get the actual pixel size.
	glfwGetFramebufferSize(glfwWindow, &width, &height);
	renderInterface = new RenderInterface_GL3();
	renderInterface->SetViewport(width, height);
	// Receive num lock and caps lock modifiers for proper handling of numpad inputs in text fields.
	glfwSetInputMode(glfwWindow, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
	// Setup the input and window event callback functions.
	setupCallbacks();
}
void WindowBase::framelessWindow() {
	static RECT screenRect; //3840  2111
	static const MARGINS shadowState{ 1,1,1,1 };
	static auto borderlessStyle = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
	auto left = (screenRect.right - width) / 2; //1408
	auto top = (screenRect.bottom - height) / 2; //666
	SetWindowLongPtr(hwnd, GWL_STYLE, borderlessStyle);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
	DwmExtendFrameIntoClientArea(hwnd, &shadowState);
	oldWindowProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)newWindowProc);
	SetWindowPos(hwnd, HWND_TOP, left, top, width, height, SWP_FRAMECHANGED | SWP_NOSIZE);	
}
LRESULT CALLBACK WindowBase::winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_NCCALCSIZE: {
			WINDOWPLACEMENT      wp;
			LPNCCALCSIZE_PARAMS  szr;
			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hwnd, &wp);
			szr = LPNCCALCSIZE_PARAMS(lParam);
			if (wp.showCmd == SW_SHOWMAXIMIZED) {
				RECT WorkAreaRect;
				SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkAreaRect, 0);
				szr->rgrc[0] = WorkAreaRect;
			}
			else if (wp.showCmd == SW_SHOWNORMAL && context) {
				WindowShowNormal();
			}
			return 0;
		}
		case WM_NCHITTEST: {
			return hitTest(hwnd, lParam);
		}
		case WM_GETMINMAXINFO: {
			MINMAXINFO* mminfo;
			mminfo = (PMINMAXINFO)lParam;
			mminfo->ptMinTrackSize.x = 1024;
			mminfo->ptMinTrackSize.y = 768;
			mminfo->ptMaxPosition.x = 0;
			mminfo->ptMaxPosition.y = 0;
			break;
		}
		case WM_SIZE: {
			if (context) {
				width = LOWORD(lParam);
				height = HIWORD(lParam);
				context->SetDimensions({ width,height });
				context->Update();
				renderInterface->BeginFrame();
				renderInterface->Clear();
				context->Render();
				renderInterface->EndFrame();
				glfwSwapBuffers(glfwWindow);
			}
			break;
		}
	}
	return CallWindowProc(oldWindowProc, hwnd, msg, wParam, lParam); //窗口消息处理权力交给原有的窗口处理逻辑
}
void WindowBase::Close() {
	glfwSetWindowShouldClose(glfwWindow, true);
}
LRESULT WindowBase::hitTest(HWND hwnd, LPARAM lParam) {
	POINT absoluteCursor = POINT{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	RECT winRect;
	GetWindowRect(hwnd, &winRect);
	if (absoluteCursor.x > winRect.left && absoluteCursor.y > winRect.top && absoluteCursor.x < winRect.right && absoluteCursor.y < winRect.bottom) {
		int borderWidth = 6;
		//Rml::Log::Message(Rml::Log::LT_ERROR, "%d < %d,%d < 50", absoluteCursor.x, winRect.right - 260, absoluteCursor.y);
		if (absoluteCursor.x < winRect.left + borderWidth && absoluteCursor.y < winRect.top + borderWidth) return HTTOPLEFT;
		else if (absoluteCursor.x < winRect.left + borderWidth && absoluteCursor.y > winRect.bottom - borderWidth) return HTBOTTOMLEFT;
		else if (absoluteCursor.x > winRect.right - borderWidth && absoluteCursor.y > winRect.bottom - borderWidth) return HTBOTTOMRIGHT;
		else if (absoluteCursor.x > winRect.right - borderWidth && absoluteCursor.y < winRect.top + borderWidth) return HTTOPRIGHT;
		else if (absoluteCursor.x < winRect.left + borderWidth) return HTLEFT;
		else if (absoluteCursor.x > winRect.right - borderWidth) return HTRIGHT;
		else if (absoluteCursor.y < winRect.top + borderWidth) return HTTOP;
		else if (absoluteCursor.y > winRect.bottom - borderWidth) return HTBOTTOM;
		else if(IsMouseInCaptionArea(absoluteCursor.x - winRect.left, absoluteCursor.y - winRect.top))
		{
			return HTCAPTION;
		}
		return HTCLIENT;
	}
	else
	{
		return HTNOWHERE;
	}
}

void WindowBase::setupCallbacks(){	
	// Key input
	glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int glfw_key, int /*scancode*/, int glfw_action, int glfw_mods) {
		auto self = findWindowByGlfwWindow(window);
		if (!self->context) return;	
		self->glfwActiveModifiers = glfw_mods;
		// Override the default key event callback to add global shortcuts for the samples.
		Rml::Context* context = self->context;
		switch (glfw_action)
		{
			case GLFW_PRESS:
			case GLFW_REPEAT:
			{
				const Rml::Input::KeyIdentifier key = RmlGLFW::ConvertKey(glfw_key);
				const int key_modifier = RmlGLFW::ConvertKeyModifiers(glfw_mods);
				float dp_ratio = 1.f;
				glfwGetWindowContentScale(self->glfwWindow, &dp_ratio, nullptr);
				// See if we have any global shortcuts that take priority over the context.
				if (!self->ProcessKeyDownShortcuts(context, key, key_modifier, dp_ratio, true))
					break;
				// Otherwise, hand the event over to the context by calling the input handler as normal.
				if (!RmlGLFW::ProcessKeyCallback(context, glfw_key, glfw_action, glfw_mods))
					break;
				// The key was not consumed by the context either, try keyboard shortcuts of lower priority.
				if (!self->ProcessKeyDownShortcuts(context, key, key_modifier, dp_ratio, false))
					break;
			}
			break;
			case GLFW_RELEASE:
				RmlGLFW::ProcessKeyCallback(context, glfw_key, glfw_action, glfw_mods);
				break;
		}
	});

	glfwSetCharCallback(glfwWindow, [](GLFWwindow* window, unsigned int codepoint) { 
		auto self = findWindowByGlfwWindow(window);
		RmlGLFW::ProcessCharCallback(self->context, codepoint); 
	});

	glfwSetCursorEnterCallback(glfwWindow, [](GLFWwindow* window, int entered) { 
		auto self = findWindowByGlfwWindow(window);
		if (self && self->context) {
			RmlGLFW::ProcessCursorEnterCallback(self->context, entered);
		}		
	});

	// Mouse input
	glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double xpos, double ypos) {
		auto self = findWindowByGlfwWindow(window);
		if (self && self->context) {
			RmlGLFW::ProcessCursorPosCallback(self->context, xpos, ypos, self->glfwActiveModifiers);
		}
	});

	glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* window, int button, int action, int mods) {
		auto self = findWindowByGlfwWindow(window);
		if (self && self->context) {
			self->glfwActiveModifiers = mods;
			RmlGLFW::ProcessMouseButtonCallback(self->context, button, action, mods);
		}
	});

	glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double /*xoffset*/, double yoffset) {
		auto self = findWindowByGlfwWindow(window);
		RmlGLFW::ProcessScrollCallback(self->context, yoffset, self->glfwActiveModifiers);
	});

	// Window events
	glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow* window, int width, int height) {
		auto self = findWindowByGlfwWindow(window);
		self->renderInterface->SetViewport(width, height);
		RmlGLFW::ProcessFramebufferSizeCallback(self->context, width, height);
	});

	glfwSetWindowContentScaleCallback(glfwWindow,[](GLFWwindow* window, float xscale, float /*yscale*/) { 
		auto self = findWindowByGlfwWindow(window);
		RmlGLFW::ProcessContentScaleCallback(self->context, xscale); 
	});
}

void WindowBase::ProcessEvents()
{
	glfwMakeContextCurrent(glfwWindow);
	context->Update();
	renderInterface->BeginFrame();
	renderInterface->Clear();
	context->Render();
	renderInterface->EndFrame();
	glfwSwapBuffers(glfwWindow);
}
bool WindowBase::ProcessKeyDownShortcuts(Rml::Context* context, Rml::Input::KeyIdentifier key, int key_modifier, float native_dp_ratio, bool priority)
{
	if (!context) return true;
	// Result should return true to allow the event to propagate to the next handler.
	bool result = false;
	// This function is intended to be called twice by the backend, before and after submitting the key event to the context. This way we can
	// intercept shortcuts that should take priority over the context, and then handle any shortcuts of lower priority if the context did not
	// intercept it.
	if (priority)
	{
		// Priority shortcuts are handled before submitting the key to the context.
		// Toggle debugger and set dp-ratio using Ctrl +/-/0 keys.
		if (key == Rml::Input::KI_F8)
		{
			Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
		}
		else if (key == Rml::Input::KI_0 && key_modifier & Rml::Input::KM_CTRL)
		{
			context->SetDensityIndependentPixelRatio(native_dp_ratio);
		}
		else if (key == Rml::Input::KI_1 && key_modifier & Rml::Input::KM_CTRL)
		{
			context->SetDensityIndependentPixelRatio(1.f);
		}
		else if ((key == Rml::Input::KI_OEM_MINUS || key == Rml::Input::KI_SUBTRACT) && key_modifier & Rml::Input::KM_CTRL)
		{
			const float new_dp_ratio = Rml::Math::Max(context->GetDensityIndependentPixelRatio() / 1.2f, 0.5f);
			context->SetDensityIndependentPixelRatio(new_dp_ratio);
		}
		else if ((key == Rml::Input::KI_OEM_PLUS || key == Rml::Input::KI_ADD) && key_modifier & Rml::Input::KM_CTRL)
		{
			const float new_dp_ratio = Rml::Math::Min(context->GetDensityIndependentPixelRatio() * 1.2f, 2.5f);
			context->SetDensityIndependentPixelRatio(new_dp_ratio);
		}
		else
		{
			// Propagate the key down event to the context.
			result = true;
		}
	}
	else
	{
		// We arrive here when no priority keys are detected and the key was not consumed by the context. Check for shortcuts of lower priority.
		if (key == Rml::Input::KI_R && key_modifier & Rml::Input::KM_CTRL)
		{
			for (int i = 0; i < context->GetNumDocuments(); i++)
			{
				Rml::ElementDocument* document = context->GetDocument(i);
				const Rml::String& src = document->GetSourceURL();
				if (src.size() > 4 && src.substr(src.size() - 4) == ".rml")
				{
					document->ReloadStyleSheet();
				}
			}
		}
		else
		{
			result = true;
		}
	}
	return result;
}