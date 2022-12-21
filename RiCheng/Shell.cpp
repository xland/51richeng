#include <Windows.h>
#include <filesystem>
#include "Shell.h"
#include "ShellFileInterface.h"
#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Input.h>
#include <RmlUi/Debugger.h>

static Rml::UniquePtr<ShellFileInterface> file_interface;

bool Shell::Initialize()
{
	file_interface = Rml::MakeUnique<ShellFileInterface>();
	Rml::SetFileInterface(file_interface.get());
	return true;
}
void Shell::LoadFonts()
{
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
void Shell::Shutdown()
{
	file_interface.reset();
}
bool Shell::ProcessKeyDownShortcuts(Rml::Context* context, Rml::Input::KeyIdentifier key, int key_modifier, float native_dp_ratio, bool priority)
{
	if (!context)
		return true;
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
