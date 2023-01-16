#include "DB.h"
#include <shlwapi.h>
#include <shlobj.h>
#include <format>
#include <fstream>
#include <iostream>
#include "RmlUi/Core.h"
#include "resource.h"


DB::DB() {
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
    {
        PathAppend(szPath, L"\\51RiCheng");
        //todo error
    }
    std::filesystem::path dbPath{ szPath };
    if (!std::filesystem::exists(dbPath)) {
        std::filesystem::create_directories(dbPath);
    }
    dbPath /= "db.db";
    if (!std::filesystem::exists(dbPath)) {
        createDBFile(dbPath);
    }
    int rc  = sqlite3_open(dbPath.generic_string().c_str(), &db);
}
void DB::createDBFile(std::filesystem::path& dbPath) {
    HMODULE instance = ::GetModuleHandle(NULL);
    HRSRC resID = ::FindResource(instance, MAKEINTRESOURCE(IDR_DB1), L"DB");
    if (resID == 0) {
        //todo
        return;
    }
    HGLOBAL res = ::LoadResource(instance, resID);
    if (res == 0) {
        //todo
        return;
    }
    LPVOID resData = ::LockResource(res);
    DWORD dwResSize = ::SizeofResource(instance, resID);
    if (!dwResSize)
    {
        return;
    }
    HANDLE hResFile = CreateFile(dbPath.wstring().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD dwWritten = 0;
    WriteFile(hResFile, resData, dwResSize, &dwWritten, NULL);
    CloseHandle(hResFile);
    if (dwResSize != dwWritten);
    {
        return;
    }
}
void DB::close() {
    sqlite3_close(db);
}

std::vector<ToDo> DB::getTodo(std::chrono::year_month_day& day) {
    std::string sqlStr = std::format("selec * from todo where startTime > {0} and endTime < {1}",0,1);
    sqlite3_stmt* stmt = NULL;
    const char* zTail;
    auto prepareResult = sqlite3_prepare_v2(db, sqlStr.c_str(), -1, &stmt, &zTail);
    std::vector<ToDo> vect;
    return vect;
}


DB* DB::get() {
    if (instance == nullptr) {
        instance = new DB();
    }
    return instance;
}