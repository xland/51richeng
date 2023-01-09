#include "ShellFileInterface.h"
#include <stdio.h>
#include <Windows.h>
#include <RmlUi/Core.h>
#include "ResourceHelper.h"

struct FileData {
	FILE* file_handle;
	int idr_number;
	size_t seek_state; //todo
	size_t length;
};

ShellFileInterface::ShellFileInterface() {}

ShellFileInterface::~ShellFileInterface() {}

// Opens a file.
Rml::FileHandle ShellFileInterface::Open(const Rml::String& path)
{
	FileData* data = new FileData{};
	if (path.substr(0, 2) != "ui") {
		data->file_handle = fopen(path.c_str(), "rb");
	}
	else if (ResourceHelper::ResourcePathMap[path]) {
		data->idr_number = ResourceHelper::ResourcePathMap[path];
		HMODULE instance = ::GetModuleHandle(NULL);
		HRSRC resID = ::FindResource(instance, MAKEINTRESOURCE(data->idr_number), L"ui");
		if (resID != 0) {
			size_t resSize = ::SizeofResource(instance, resID);
			data->length = resSize;
		}
	}
	else {
		Rml::Log::Message(Rml::Log::LT_ERROR, "ShellFileInterface Open %s", path);
		return NULL;
	}
	return (Rml::FileHandle)data;
}
void ShellFileInterface::Close(Rml::FileHandle file)
{
	FileData* data = (FileData*)file;
	if (data->file_handle) {
		fclose((FILE*)data->file_handle);
	}
	delete data;
}
size_t ShellFileInterface::Read(void* buffer, size_t size, Rml::FileHandle file)
{
	FileData* data = (FileData*)file;
	if (data->file_handle) {
		return fread(buffer, 1, size, (FILE*)data->file_handle);
	}
	else
	{
		HMODULE instance = ::GetModuleHandle(NULL);
		HRSRC resID = ::FindResource(instance, MAKEINTRESOURCE(data->idr_number), L"ui");
		if (resID == 0) {
			return 0;
		}
		HGLOBAL res = ::LoadResource(instance, resID);
		if (res == 0) return 0;
		char* resData = (char*)::LockResource(res);
		memcpy(buffer, resData+data->seek_state, size);
		data->seek_state += size;
		return size;
	}
}
bool ShellFileInterface::Seek(Rml::FileHandle file, long offset, int origin)
{
	FileData* data = (FileData*)file;
	if (data->file_handle) {
		return fseek((FILE*)data->file_handle, offset, origin) == 0;
	}
	else
	{
		switch (origin)
		{
			case SEEK_SET:
				data->seek_state = offset;
				break;
			case SEEK_END:
				data->seek_state = data->length - offset;
				break;
			case SEEK_CUR:
				data->seek_state = data->seek_state + offset;
		}
		return true;
	}
}
size_t ShellFileInterface::Tell(Rml::FileHandle file)
{
	FileData* data = (FileData*)file;
	if (data->file_handle) {
		return ftell((FILE*)data->file_handle);
	}
	else
	{
		return data->seek_state;
	}
}
