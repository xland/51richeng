#ifndef RMLUI_SHELL_SHELLFILEINTERFACE_H
#define RMLUI_SHELL_SHELLFILEINTERFACE_H

#include <RmlUi/Core/Types.h>
#include <RmlUi/Core/FileInterface.h>

/**
	RmlUi file interface for the shell examples.
	@author Lloyd Weehuizen
 */

class ShellFileInterface : public Rml::FileInterface
{
public:
	ShellFileInterface();
	virtual ~ShellFileInterface();

	/// Opens a file.		
	Rml::FileHandle Open(const Rml::String& path) override;

	/// Closes a previously opened file.		
	void Close(Rml::FileHandle file) override;

	/// Reads data from a previously opened file.		
	size_t Read(void* buffer, size_t size, Rml::FileHandle file) override;

	/// Seeks to a point in a previously opened file.		
	bool Seek(Rml::FileHandle file, long offset, int origin) override;

	/// Returns the current position of the file pointer.		
	size_t Tell(Rml::FileHandle file) override;
};

#endif
