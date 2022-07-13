/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 * Copyright (c) 2019 The RmlUi Team, and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "RmlUi_Backend.h"
#include "RmlUi_Include_Windows.h"
#include "RmlUi_Platform_Win32.h"
#include "RmlUi_Renderer_GL2.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/Profiling.h>
#include <RmlUi/Core/Input.h>
#include <windowsx.h>
#include <dwmapi.h>
/**
    High DPI support using Windows Per Monitor V2 DPI awareness.

    Requires Windows 10, version 1703.
 */
#ifndef DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
	#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((HANDLE)-4)
#endif
#ifndef WM_DPICHANGED
	#define WM_DPICHANGED 0x02E0
#endif

// Declare pointers to the DPI aware Windows API functions.
using ProcSetProcessDpiAwarenessContext = BOOL(WINAPI*)(HANDLE value);
using ProcGetDpiForWindow = UINT(WINAPI*)(HWND hwnd);
using ProcAdjustWindowRectExForDpi = BOOL(WINAPI*)(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle, UINT dpi);

static bool has_dpi_support = false;
static ProcSetProcessDpiAwarenessContext procSetProcessDpiAwarenessContext = NULL;
static ProcGetDpiForWindow procGetDpiForWindow = NULL;
static ProcAdjustWindowRectExForDpi procAdjustWindowRectExForDpi = NULL;
std::vector<Rml::Vector4f> Backend::WindowHitTestArea = {};
bool Backend::isWindowResizable = false;

// Make ourselves DPI aware on supported Windows versions.
static void InitializeDpiSupport()
{
	// Cast function pointers to void* first for MinGW not to emit errors.
	procSetProcessDpiAwarenessContext =
		(ProcSetProcessDpiAwarenessContext)(void*)GetProcAddress(GetModuleHandle(TEXT("User32.dll")), "SetProcessDpiAwarenessContext");
	procGetDpiForWindow = (ProcGetDpiForWindow)(void*)GetProcAddress(GetModuleHandle(TEXT("User32.dll")), "GetDpiForWindow");
	procAdjustWindowRectExForDpi =
		(ProcAdjustWindowRectExForDpi)(void*)GetProcAddress(GetModuleHandle(TEXT("User32.dll")), "AdjustWindowRectExForDpi");

	if (!has_dpi_support && procSetProcessDpiAwarenessContext != NULL && procGetDpiForWindow != NULL && procAdjustWindowRectExForDpi != NULL)
	{
		// Activate Per Monitor V2.
		if (procSetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2))
			has_dpi_support = true;
	}
}

static UINT GetWindowDpi(HWND window_handle)
{
	if (has_dpi_support)
	{
		UINT dpi = procGetDpiForWindow(window_handle);
		if (dpi != 0)
			return dpi;
	}
	return USER_DEFAULT_SCREEN_DPI;
}

static float GetDensityIndependentPixelRatio(HWND window_handle)
{
	return float(GetWindowDpi(window_handle)) / float(USER_DEFAULT_SCREEN_DPI);
}

static void DisplayError(HWND window_handle, const Rml::String& msg)
{
	MessageBoxW(window_handle, RmlWin32::ConvertToUTF16(msg).c_str(), L"Backend Error", MB_OK);
}

// Create the window but don't show it yet. Returns the pixel size of the window, which may be different than the passed size due to DPI settings.
static HWND InitializeWindow(HINSTANCE instance_handle, const std::wstring& name, int& inout_width, int& inout_height, bool allow_resize);
// Attach the OpenGL context.
static bool AttachToNative(HWND window_handle, HDC& out_device_context, HGLRC& out_render_context);
// Detach the OpenGL context.
static void DetachFromNative(HWND window_handle, HDC device_context, HGLRC render_context);

/**
    Global data used by this backend.

    Lifetime governed by the calls to Backend::Initialize() and Backend::Shutdown().
 */
struct BackendData {
	SystemInterface_Win32 system_interface;
	RenderInterface_GL2 render_interface;

	HINSTANCE instance_handle = nullptr;
	std::wstring instance_name;
	HWND window_handle = nullptr;

	HDC device_context = nullptr;
	HGLRC render_context = nullptr;

	bool context_dimensions_dirty = true;
	Rml::Vector2i window_dimensions;
	bool running = true;

	// Arguments set during event processing and nulled otherwise.
	Rml::Context* context = nullptr;
	KeyDownCallback key_down_callback = nullptr;
};
static Rml::UniquePtr<BackendData> data;

bool Backend::Initialize(const char* window_name, int width, int height, bool allow_resize)
{
	RMLUI_ASSERT(!data);

	const std::wstring name = RmlWin32::ConvertToUTF16(Rml::String(window_name));

	data = Rml::MakeUnique<BackendData>();

	data->instance_handle = GetModuleHandle(nullptr);
	data->instance_name = name;

	InitializeDpiSupport();

	// Initialize the window but don't show it yet.
	HWND window_handle = InitializeWindow(data->instance_handle, name, width, height, allow_resize);
	if (!window_handle)
		return false;

	// Attach the OpenGL context.
	if (!AttachToNative(window_handle, data->device_context, data->render_context))
	{
		::CloseWindow(window_handle);
		return false;
	}

	data->window_handle = window_handle;
	data->system_interface.SetWindow(window_handle);

	// Now we are ready to show the window.
	::ShowWindow(window_handle, SW_SHOW);
	::SetForegroundWindow(window_handle);
	::SetFocus(window_handle);

	return true;
}

void Backend::Shutdown()
{
	RMLUI_ASSERT(data);

	DetachFromNative(data->window_handle, data->device_context, data->render_context);

	::DestroyWindow(data->window_handle);
	::UnregisterClassW((LPCWSTR)data->instance_name.data(), data->instance_handle);

	data.reset();
}

Rml::SystemInterface* Backend::GetSystemInterface()
{
	RMLUI_ASSERT(data);
	return &data->system_interface;
}

Rml::RenderInterface* Backend::GetRenderInterface()
{
	RMLUI_ASSERT(data);
	return &data->render_interface;
}

bool Backend::ProcessEvents(Rml::Context* context, KeyDownCallback key_down_callback)
{
	RMLUI_ASSERT(data && context);

	// The initial window size may have been affected by system DPI settings, apply the actual pixel size and dp-ratio to the context.
	if (data->context_dimensions_dirty)
	{
		data->context_dimensions_dirty = false;
		const float dp_ratio = GetDensityIndependentPixelRatio(data->window_handle);
		context->SetDimensions(data->window_dimensions);
		context->SetDensityIndependentPixelRatio(dp_ratio);
	}

	data->context = context;
	data->key_down_callback = key_down_callback;

	MSG message;
	while (PeekMessage(&message, nullptr, 0, 0, PM_NOREMOVE))
	{
		GetMessage(&message, nullptr, 0, 0);

		// Dispatch the message to our local event handler below.
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	data->context = nullptr;
	data->key_down_callback = nullptr;

	return data->running;
}

void Backend::RequestExit()
{
	RMLUI_ASSERT(data);
	data->running = false;
}

void Backend::BeginFrame()
{
	RMLUI_ASSERT(data);
	data->render_interface.BeginFrame();
	data->render_interface.Clear();
}

void Backend::PresentFrame()
{
	RMLUI_ASSERT(data);
	data->render_interface.EndFrame();

	// Flips the OpenGL buffers.
	SwapBuffers(data->device_context);

	// Optional, used to mark frames during performance profiling.
	RMLUI_FrameMark;
}


static LRESULT windowHitTest(HWND window_handle, LPARAM l_param) {
	POINT absoluteCursor = POINT{ GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param) };
	RECT winRect;
	::GetWindowRect(window_handle, &winRect);
	POINT cursor = POINT{ absoluteCursor.x - winRect.left,absoluteCursor.y - winRect.top };
	if (absoluteCursor.x > winRect.left && absoluteCursor.y > winRect.top && absoluteCursor.x < winRect.right && absoluteCursor.y < winRect.bottom) {
		if (Backend::isWindowResizable) {
			int borderWidth = 4;
			if (absoluteCursor.x < winRect.left + borderWidth && absoluteCursor.y < winRect.top + borderWidth) return HTTOPLEFT;
			else if (absoluteCursor.x < winRect.left + borderWidth && absoluteCursor.y > winRect.bottom - borderWidth) return HTBOTTOMLEFT;
			else if (absoluteCursor.x > winRect.right - borderWidth && absoluteCursor.y > winRect.bottom - borderWidth) return HTBOTTOMRIGHT;
			else if (absoluteCursor.x > winRect.right - borderWidth && absoluteCursor.y < winRect.top + borderWidth) return HTTOPRIGHT;
			else if (absoluteCursor.x < winRect.left + borderWidth) return HTLEFT;
			else if (absoluteCursor.x > winRect.right - borderWidth) return HTRIGHT;
			else if (absoluteCursor.y < winRect.top + borderWidth) return HTTOP;
			else if (absoluteCursor.y > winRect.bottom - borderWidth) return HTBOTTOM;
		}
		for (auto captionRegion : Backend::WindowHitTestArea)
		{
			auto i = 1;
			if (cursor.x > captionRegion.x && cursor.y > captionRegion.y && cursor.x < captionRegion.z && cursor.y < captionRegion.w) {
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


// Local event handler for window and input events.
static LRESULT CALLBACK WindowProcedureHandler(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
{
	RMLUI_ASSERT(data);

	switch (message)
	{
	case WM_NCCALCSIZE: {
		return 0;
	}
	case WM_NCHITTEST:
	{
		return windowHitTest(window_handle, l_param);
		break;
	}
	case WM_WINDOWPOSCHANGING:
	{
		WINDOWPOS* pos = (WINDOWPOS*)l_param;
		// Show desktop (Windows + D) results in the window moved to -32000, -32000 and size changed
		if (pos->x == -32000) {
			// Set the flags to prevent this and "survive" to the desktop toggle
			pos->flags |= SWP_NOMOVE | SWP_NOSIZE;
		}
		// Also force the z order to ensure the window is always on bottom
		pos->hwndInsertAfter = HWND_BOTTOM;
		return 0;
	}
	case WM_GETMINMAXINFO: {
		MINMAXINFO* mminfo;
		mminfo = (PMINMAXINFO)l_param;
		mminfo->ptMinTrackSize.x = 800;
		mminfo->ptMinTrackSize.y = 600;
		mminfo->ptMaxPosition.x = 0;
		mminfo->ptMaxPosition.y = 0;
		break;
	}
	case WM_CLOSE:
	{
		data->running = false;
		return 0;
	}
	break;
	case WM_SIZE:
	{
		const int width = LOWORD(l_param);
		const int height = HIWORD(l_param);
		data->window_dimensions.x = width;
		data->window_dimensions.y = height;
		data->render_interface.SetViewport(width, height);
		if (data->context) {
			data->context->SetDimensions(data->window_dimensions);
			data->context->Update();

			Backend::BeginFrame();
			data->context->Render();
			Backend::PresentFrame();
		}			
		return 0;
	}
	break;
	case WM_DPICHANGED:
	{
		RECT* new_pos = (RECT*)l_param;
		SetWindowPos(window_handle, NULL, new_pos->left, new_pos->top, new_pos->right - new_pos->left, new_pos->bottom - new_pos->top,
			SWP_NOZORDER | SWP_NOACTIVATE);
		if (data->context && has_dpi_support)
			data->context->SetDensityIndependentPixelRatio(GetDensityIndependentPixelRatio(window_handle));
		return 0;
	}
	break;
	case WM_KEYDOWN:
	{
		// Override the default key event callback to add global shortcuts for the samples.
		Rml::Context* context = data->context;
		KeyDownCallback key_down_callback = data->key_down_callback;

		const Rml::Input::KeyIdentifier rml_key = RmlWin32::ConvertKey((int)w_param);
		const int rml_modifier = RmlWin32::GetKeyModifierState();
		const float native_dp_ratio = GetDensityIndependentPixelRatio(window_handle);

		// See if we have any global shortcuts that take priority over the context.
		if (key_down_callback && !key_down_callback(context, rml_key, rml_modifier, native_dp_ratio, true))
			return 0;
		// Otherwise, hand the event over to the context by calling the input handler as normal.
		if (!RmlWin32::WindowProcedure(context, window_handle, message, w_param, l_param))
			return 0;
		// The key was not consumed by the context either, try keyboard shortcuts of lower priority.
		if (key_down_callback && !key_down_callback(context, rml_key, rml_modifier, native_dp_ratio, false))
			return 0;
		return 0;
	}
	break;
	default:
	{
		// Submit it to the platform handler for default input handling.
		if (!RmlWin32::WindowProcedure(data->context, window_handle, message, w_param, l_param))
			return 0;
	}
	break;
	}

	// All unhandled messages go to DefWindowProc.
	return DefWindowProc(window_handle, message, w_param, l_param);
}

static HWND InitializeWindow(HINSTANCE instance_handle, const std::wstring& name, int& inout_width, int& inout_height, bool allow_resize)
{
	// Fill out the window class struct.
	WNDCLASSW window_class;
	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = &WindowProcedureHandler; // Attach our local event handler.
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = instance_handle;
	window_class.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_class.hbrBackground = nullptr;
	window_class.lpszMenuName = nullptr;
	window_class.lpszClassName = name.data();

	if (!RegisterClassW(&window_class))
	{
		DisplayError(NULL, "Could not register window class.");
		return nullptr;
	}

	HWND window_handle = CreateWindowExW(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		name.data(),                                                                // Window class name.
		name.data(), WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW, 0, 0, // Window position.
		0, 0,                                                                       // Window size.
		nullptr, nullptr, instance_handle, nullptr);

	if (!window_handle)
	{
		DisplayError(NULL, "Could not create window.");
		return nullptr;
	}

	UINT window_dpi = GetWindowDpi(window_handle);
	inout_width = (inout_width * (int)window_dpi) / USER_DEFAULT_SCREEN_DPI;
	inout_height = (inout_height * (int)window_dpi) / USER_DEFAULT_SCREEN_DPI;

	DWORD style = (allow_resize ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX));
	DWORD extended_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	// Adjust the window size to take the edges into account.
	RECT window_rect;
	window_rect.top = 0;
	window_rect.left = 0;
	window_rect.right = inout_width;
	window_rect.bottom = inout_height;
	if (has_dpi_support)
		procAdjustWindowRectExForDpi(&window_rect, style, FALSE, extended_style, window_dpi);
	else
		AdjustWindowRectEx(&window_rect, style, FALSE, extended_style);

	SetWindowLong(window_handle, GWL_EXSTYLE, extended_style);
	SetWindowLong(window_handle, GWL_STYLE, style);

	// Resize the window.
	SetWindowPos(window_handle, HWND_BOTTOM, 0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, SWP_NOZORDER|SWP_NOACTIVATE);

	return window_handle;
}

static bool AttachToNative(HWND window_handle, HDC& out_device_context, HGLRC& out_render_context)
{
	HDC device_context = GetDC(window_handle);

	if (!device_context)
	{
		DisplayError(window_handle, "Could not get device context.");
		return false;
	}

	PIXELFORMATDESCRIPTOR pixel_format_descriptor = {};
	pixel_format_descriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixel_format_descriptor.nVersion = 1;
	pixel_format_descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixel_format_descriptor.iPixelType = PFD_TYPE_RGBA;
	pixel_format_descriptor.cColorBits = 32;
	pixel_format_descriptor.cRedBits = 8;
	pixel_format_descriptor.cGreenBits = 8;
	pixel_format_descriptor.cBlueBits = 8;
	pixel_format_descriptor.cAlphaBits = 8;
	pixel_format_descriptor.cDepthBits = 24;
	pixel_format_descriptor.cStencilBits = 8;

	int pixel_format = ChoosePixelFormat(device_context, &pixel_format_descriptor);
	if (!pixel_format)
	{
		DisplayError(window_handle, "Could not choose 32-bit pixel format.");
		return false;
	}

	if (!SetPixelFormat(device_context, pixel_format, &pixel_format_descriptor))
	{
		DisplayError(window_handle, "Could not set pixel format.");
		return false;
	}

	HGLRC render_context = wglCreateContext(device_context);
	if (!render_context)
	{
		DisplayError(window_handle, "Could not create OpenGL rendering context.");
		return false;
	}

	// Activate the rendering context.
	if (!wglMakeCurrent(device_context, render_context))
	{
		DisplayError(window_handle, "Unable to make rendering context current.");
		return false;
	}

	out_device_context = device_context;
	out_render_context = render_context;

	return true;
}

static void DetachFromNative(HWND window_handle, HDC device_context, HGLRC render_context)
{
	// Shutdown OpenGL
	if (render_context)
	{
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(render_context);
	}

	if (device_context)
	{
		ReleaseDC(window_handle, device_context);
	}
}