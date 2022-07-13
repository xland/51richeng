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

#include "../include/ShellFileInterface.h"
#include <stdio.h>
#include <map>
#include <RmlUi/Core.h>

struct FileData {
	FILE* file_handle;
	int idr_number;
	size_t seek_state; //todo
	size_t length;
};

ShellFileInterface::ShellFileInterface(std::map<std::string, int>& resourcePathMap) : resourcePathMap(resourcePathMap) {}
ShellFileInterface::~ShellFileInterface() {}

Rml::FileHandle ShellFileInterface::Open(const Rml::String& path)
{
	FileData* data = new FileData{};
	if (path.substr(0, 6) != "assets") {
		data->file_handle = fopen(path.c_str(), "rb");
	}
	else if (resourcePathMap[path]) {
		data->idr_number = resourcePathMap[path];
	}
	else {
		Rml::Log::Message(Rml::Log::LT_ERROR, "ShellFileInterface Open %s",path);
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
		HRSRC resID = ::FindResource(instance, MAKEINTRESOURCE(data->idr_number), L"UI");
		if (resID == 0) {
			return 0;
		}
		HGLOBAL res = ::LoadResource(instance, resID);
		if (res == 0) return 0;
		LPVOID resData = ::LockResource(res);
		memcpy(buffer, resData, size);
		return size;
	}
}
bool ShellFileInterface::Seek(Rml::FileHandle file, long offset, int origin)
{
	FileData* data = (FileData*)file;
	if (data->file_handle) {
		return fseek((FILE*)data->file_handle, offset, origin) == 0;
	}
	return true;
}
size_t ShellFileInterface::Tell(Rml::FileHandle file)
{
	FileData* data = (FileData*)file;
	if (data->file_handle) {
		return ftell((FILE*)data->file_handle);
	}
	else
	{
		if (!data->length){
			HMODULE instance = ::GetModuleHandle(NULL);
			HRSRC resID = ::FindResource(instance, MAKEINTRESOURCE(data->idr_number), L"UI");
			if (resID != 0) {
				size_t resSize = ::SizeofResource(instance, resID);
				data->length = resSize;
				return resSize;
			}
		}
		return data->length;
	}
}
