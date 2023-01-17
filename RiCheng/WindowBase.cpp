#include "WindowBase.h"
#include <Windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <GLFW/glfw3.h>
#include <RmlUi/Core.h>
#include "spdlog/spdlog.h"
#include "RmlUi_Renderer_GL3.h"
#include "App.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


namespace {
	WNDPROC OldProc;
	static void LogErrorFromGLFW(int error, const char* description)
	{
		spdlog::error("GLFW 错误 code:{0},info:{1}", error, description);
	}
	static LRESULT HitTest(HWND hwnd, LPARAM lParam) {
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
	LRESULT CALLBACK WindowHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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
			return HitTest(hwnd, lParam);
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
			data->context->SetDimensions({ width,height });
			data->context->Update();
			App::get()->renderInterface->BeginFrame();
			App::get()->renderInterface->Clear();
			data->context->Render();
			App::get()->renderInterface->EndFrame();
			glfwSwapBuffers(data->window);
			break;
		}
		}
		return CallWindowProc(OldProc, hwnd, msg, wParam, lParam); //窗口消息处理权力交给原有的窗口处理逻辑
	}

	void CustomWindowHandler(HWND hwnd)
	{
		auto borderlessStyle = WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_VISIBLE;
		::SetWindowLongPtr(hwnd, GWL_STYLE, borderlessStyle);
		static const MARGINS shadow_state{ 1,1,1,1 };
		::DwmExtendFrameIntoClientArea(hwnd, &shadow_state);
		::SetWindowPos(hwnd, nullptr, 110, 110, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE);
		OldProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WindowHandler);
		RECT screenRect, winRect;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
		GetWindowRect(hwnd, &winRect);
		SetWindowPos(hwnd, HWND_TOP, (screenRect.right - winRect.right) / 2, (screenRect.bottom - winRect.bottom) / 2, winRect.right, winRect.bottom, SWP_FRAMECHANGED | SWP_NOSIZE);
	}

	static void SetupCallbacks(GLFWwindow* window)
	{
		// Key input
		glfwSetKeyCallback(window, [](GLFWwindow* /*window*/, int glfw_key, int /*scancode*/, int glfw_action, int glfw_mods) {
			if (!data->context)
				return;
			// Store the active modifiers for later because GLFW doesn't provide them in the callbacks to the mouse input events.
			data->glfw_active_modifiers = glfw_mods;
			// Override the default key event callback to add global shortcuts for the samples.
			Rml::Context* context = data->context;
			KeyDownCallback key_down_callback = data->key_down_callback;
			switch (glfw_action)
			{
				case GLFW_PRESS:
				case GLFW_REPEAT:
				{
					const Rml::Input::KeyIdentifier key = RmlGLFW::ConvertKey(glfw_key);
					const int key_modifier = RmlGLFW::ConvertKeyModifiers(glfw_mods);
					float dp_ratio = 1.f;
					glfwGetWindowContentScale(data->window, &dp_ratio, nullptr);

					// See if we have any global shortcuts that take priority over the context.
					if (key_down_callback && !key_down_callback(context, key, key_modifier, dp_ratio, true))
						break;
					// Otherwise, hand the event over to the context by calling the input handler as normal.
					if (!RmlGLFW::ProcessKeyCallback(context, glfw_key, glfw_action, glfw_mods))
						break;
					// The key was not consumed by the context either, try keyboard shortcuts of lower priority.
					if (key_down_callback && !key_down_callback(context, key, key_modifier, dp_ratio, false))
						break;
				}
				break;
				case GLFW_RELEASE:
					RmlGLFW::ProcessKeyCallback(context, glfw_key, glfw_action, glfw_mods);
					break;
			}
		});

		glfwSetCharCallback(window, [](GLFWwindow* /*window*/, unsigned int codepoint) { RmlGLFW::ProcessCharCallback(data->context, codepoint); });

		glfwSetCursorEnterCallback(window, [](GLFWwindow* /*window*/, int entered) { RmlGLFW::ProcessCursorEnterCallback(data->context, entered); });

		// Mouse input
		glfwSetCursorPosCallback(window, [](GLFWwindow* /*window*/, double xpos, double ypos) {
			RmlGLFW::ProcessCursorPosCallback(data->context, xpos, ypos, data->glfw_active_modifiers);
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* /*window*/, int button, int action, int mods) {
			data->glfw_active_modifiers = mods;
			RmlGLFW::ProcessMouseButtonCallback(data->context, button, action, mods);
		});

		glfwSetScrollCallback(window, [](GLFWwindow* /*window*/, double /*xoffset*/, double yoffset) {
			RmlGLFW::ProcessScrollCallback(data->context, yoffset, data->glfw_active_modifiers);
		});

		// Window events
		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* /*window*/, int width, int height) {
			data->render_interface.SetViewport(width, height);
			RmlGLFW::ProcessFramebufferSizeCallback(data->context, width, height);
		});

		glfwSetWindowContentScaleCallback(window,
			[](GLFWwindow* /*window*/, float xscale, float /*yscale*/) { RmlGLFW::ProcessContentScaleCallback(data->context, xscale); });
	}
}

WindowBase::WindowBase(int width,int height,std::string& name) {
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
	GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	if (!window) {
		spdlog::error("glfwCreateWindow失败");
	}
	HWND hwnd = glfwGetWin32Window((GLFWwindow*)window);
	SetWindowText(hwnd, L"日程");
	CustomWindowHandler(hwnd);
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
	SetupCallbacks(window);
}