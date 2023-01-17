#include "WindowBase.h"
#include <dwmapi.h>
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include "spdlog/spdlog.h"
#include "App.h"


namespace {	
	static void LogErrorFromGLFW(int error, const char* description)
	{
		spdlog::error("GLFW 错误 code:{0},info:{1}", error, description);
	}	
	LRESULT CALLBACK WindowHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		WindowBase* win;
		if (msg == WM_CREATE) {
			win = (WindowBase*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)win);
		}
		else {
			win = (WindowBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}
		return win->winProc(hwnd, msg, wParam, lParam);	
	}	
}

WindowBase::WindowBase(int width, int height, std::wstring&& windowTitle, std::string&& windowName)
	:width{ width }, height{height} , windowTitle{ windowTitle },windowName{ windowName }
{
	initGLFWwindow();
	framelessWindow();
	context = Rml::CreateContext(windowName, Rml::Vector2i(width, height));
	if (context == nullptr) {
		spdlog::error("Rml::CreateContext main失败");
	}
#ifdef DEBUG
	Rml::Debugger::Initialise(context);
#endif // DEBUG
	bool running = true;
	while (running)
	{
		running = ProcessEvents(context);
		context->Update();
		Backend::BeginFrame();
		context->Render();
		Backend::PresentFrame();
	}
}

void WindowBase::ProcessEvent(Rml::Event& event) {
	auto eventId = event.GetId();
}

void WindowBase::initGLFWwindow() {
	glfwSetErrorCallback(LogErrorFromGLFW);
	if (!glfwInit()) {
		spdlog::error("GLFW初始化失败");
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
	GLFWwindow* window = glfwCreateWindow(width, height, "", nullptr, nullptr);
	if (!window) {
		spdlog::error("glfwCreateWindow失败");
	}
	winMap.insert({ window, this });
	hwnd = glfwGetWin32Window((GLFWwindow*)window);
	SetWindowText(hwnd, windowTitle.c_str());
	framelessWindow();
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	// Load the OpenGL functions.
	Rml::String renderer_message;
	if (!RmlGL3::Initialize(&renderer_message)) {
		spdlog::error("RmlGL3::Initialize失败");
	}
	App::get()->systemInterface->LogMessage(Rml::Log::LT_INFO, renderer_message);
	// The window size may have been scaled by DPI settings, get the actual pixel size.
	glfwGetFramebufferSize(window, &width, &height);
	App::get()->renderInterface->SetViewport(width, height);
	// Receive num lock and caps lock modifiers for proper handling of numpad inputs in text fields.
	glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
	// Setup the input and window event callback functions.
	setupCallbacks();
}

void WindowBase::framelessWindow()
{
	auto borderlessStyle = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
	SetWindowLongPtr(hwnd, GWL_STYLE, borderlessStyle);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
	static const MARGINS shadow_state{ 1,1,1,1 };
	DwmExtendFrameIntoClientArea(hwnd, &shadow_state);
	SetWindowPos(hwnd, nullptr, 110, 110, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE);
	OldProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WindowHandler);
	RECT screenRect, winRect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
	GetWindowRect(hwnd, &winRect);
	auto left = (screenRect.right - winRect.right) / 2;
	auto top = (screenRect.bottom - winRect.bottom) / 2;
	SetWindowPos(hwnd, HWND_TOP, left, top, winRect.right, winRect.bottom, SWP_FRAMECHANGED | SWP_NOSIZE);
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
			else if (wp.showCmd == SW_SHOWNORMAL) {
				auto context = Rml::GetContext("main");
				if (context) {
					auto doc = context->GetDocument("main");
					auto ele = doc->GetElementById("maximizeBtn");
					ele->SetInnerRML((const char*)u8"\ue6e5");
				}
			}
			return 0;
		}
		case WM_NCHITTEST: {
			return hitTest(hwnd, lParam);
		}
		case WM_GETMINMAXINFO: {
			MINMAXINFO* mminfo;
			mminfo = (PMINMAXINFO)lParam;
			mminfo->ptMinTrackSize.x = 898;
			mminfo->ptMinTrackSize.y = 542;
			mminfo->ptMaxPosition.x = 0;
			mminfo->ptMaxPosition.y = 0;
			break;
		}
		case WM_SIZE: {
			const int width = LOWORD(lParam);
			const int height = HIWORD(lParam);
			context->SetDimensions({ width,height });
			context->Update();
			App::get()->renderInterface->BeginFrame();
			App::get()->renderInterface->Clear();
			context->Render();
			App::get()->renderInterface->EndFrame();
			glfwSwapBuffers(window);
			break;
		}
	}
	return CallWindowProc(OldProc, hwnd, msg, wParam, lParam); //窗口消息处理权力交给原有的窗口处理逻辑
}

LRESULT WindowBase::hitTest(HWND hwnd, LPARAM lParam) {
	POINT absoluteCursor = POINT{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	RECT winRect;
	::GetWindowRect(hwnd, &winRect);
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
		else if (absoluteCursor.y < winRect.top + 50) {
			if (absoluteCursor.x < winRect.left + 430) {
				return HTCAPTION;
			}
			else if (absoluteCursor.x > winRect.left + 860 && absoluteCursor.x < winRect.right - 180) {
				return HTCAPTION;
			}
		}
		return HTCLIENT;
	}
	else
	{
		return HTNOWHERE;
	}
}
void WindowBase::setupCallbacks()
{	
	// Key input
	glfwSetKeyCallback(window, [](GLFWwindow* window, int glfw_key, int /*scancode*/, int glfw_action, int glfw_mods) {
		auto self = WindowBase::winMap[window];
		if (!self->context) return;	
		self->glfw_active_modifiers = glfw_mods;
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
				glfwGetWindowContentScale(self->window, &dp_ratio, nullptr);

				// See if we have any global shortcuts that take priority over the context.
				if (self->needProcessEvent && !self->ProcessKeyDownShortcuts(context, key, key_modifier, dp_ratio, true))
					break;
				// Otherwise, hand the event over to the context by calling the input handler as normal.
				if (!RmlGLFW::ProcessKeyCallback(context, glfw_key, glfw_action, glfw_mods))
					break;
				// The key was not consumed by the context either, try keyboard shortcuts of lower priority.
				if (self->needProcessEvent && !self->ProcessKeyDownShortcuts(context, key, key_modifier, dp_ratio, false))
					break;
			}
			break;
			case GLFW_RELEASE:
				RmlGLFW::ProcessKeyCallback(context, glfw_key, glfw_action, glfw_mods);
				break;
		}
	});

	glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int codepoint) { 
		auto self = WindowBase::winMap[window];
		RmlGLFW::ProcessCharCallback(self->context, codepoint); 
	});

	glfwSetCursorEnterCallback(window, [](GLFWwindow* window, int entered) { 
		auto self = WindowBase::winMap[window];
		RmlGLFW::ProcessCursorEnterCallback(self->context, entered); 
	});

	// Mouse input
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
		auto self = WindowBase::winMap[window];
		RmlGLFW::ProcessCursorPosCallback(self->context, xpos, ypos, self->glfw_active_modifiers);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
		auto self = WindowBase::winMap[window];
		self->glfw_active_modifiers = mods;
		RmlGLFW::ProcessMouseButtonCallback(self->context, button, action, mods);
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double /*xoffset*/, double yoffset) {
		auto self = WindowBase::winMap[window];
		RmlGLFW::ProcessScrollCallback(self->context, yoffset, self->glfw_active_modifiers);
	});

	// Window events
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		auto self = WindowBase::winMap[window];
		App::get()->renderInterface->SetViewport(width, height);
		RmlGLFW::ProcessFramebufferSizeCallback(self->context, width, height);
	});

	glfwSetWindowContentScaleCallback(window,[](GLFWwindow* window, float xscale, float /*yscale*/) { 
		auto self = WindowBase::winMap[window];
		RmlGLFW::ProcessContentScaleCallback(self->context, xscale); 
	});
}
bool WindowBase::ProcessEvents(Rml::Context* context)
{
	// The initial window size may have been affected by system DPI settings, apply the actual pixel size and dp-ratio to the context.
	if (context_dimensions_dirty) {
		context_dimensions_dirty = false;
		Rml::Vector2i window_size;
		float dp_ratio = 1.f;
		glfwGetFramebufferSize(window, &window_size.x, &window_size.y);
		glfwGetWindowContentScale(window, &dp_ratio, nullptr);
		context->SetDimensions(window_size);
		context->SetDensityIndependentPixelRatio(dp_ratio);
	}
	context = context;
	needProcessEvent = true;
	glfwPollEvents();
	needProcessEvent = false;
	context = nullptr;
	const bool result = !glfwWindowShouldClose(window);
	glfwSetWindowShouldClose(window, GLFW_FALSE);
	return result;
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