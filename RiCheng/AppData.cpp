#include "AppData.h"
#include <Windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <RmlUi/Core.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "sqlite/sqlite3.h"
#include "resource.h"
#include "DB.h"

AppData* AppData::get() {
    return instance;
}
void AppData::init() {
    instance = new AppData();
}
AppData::AppData() {
    initDataPath();
    initLogger();
    initDB();
}
/// <summary>
/// 初始化数据目录：C:\Users\liuxiaolun\AppData\Roaming\51RiCheng
/// </summary>
void AppData::initDataPath() {
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
    {
        dataPath = std::filesystem::path{ szPath };
        dataPath /= "51RiCheng";
    }
    if (!std::filesystem::exists(dataPath)) {
        std::filesystem::create_directories(dataPath);
    }
}
/// <summary>
/// 初始化日志文件
/// </summary>
void AppData::initLogger() {
    std::string logPath = dataPath.string() + "\\log.txt";
    auto logger = spdlog::basic_logger_mt("logger", logPath.c_str(), true);
    spdlog::set_default_logger(logger);
#ifdef NDEBUG
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_on(spdlog::level::info);
#else
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::trace);
#endif
}
/// <summary>
/// 初始化客户端数据库
/// </summary>
void AppData::initDB() {
    std::wstring dbPath = dataPath.wstring() + L"\\db.db";
    if (std::filesystem::exists(dbPath)) {
        openDB();
        return;
    }
    HMODULE instance = ::GetModuleHandle(NULL);
    HRSRC resID = ::FindResource(instance, MAKEINTRESOURCE(IDR_DB), L"DB");
    if (resID == 0) {
        spdlog::error("::FindResource(instance, MAKEINTRESOURCE(IDR_DB1), LDB)失败");
        return;
    }
    HGLOBAL res = ::LoadResource(instance, resID);
    if (res == 0) {
        spdlog::error("::LoadResource(instance, resID);失败");
        return;
    }
    LPVOID resData = ::LockResource(res);
    DWORD dwResSize = ::SizeofResource(instance, resID);
    HANDLE hResFile = CreateFile(dbPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD dwWritten = 0;
    WriteFile(hResFile, resData, dwResSize, &dwWritten, NULL);
    CloseHandle(hResFile);
    if (dwResSize != dwWritten)
    {
        spdlog::error("DB写入不完整");
        return;
    }
    openDB();
}
/// <summary>
/// 建立客户端数据库连接，并初始化数据库处理对象
/// </summary>
void AppData::openDB() {
    std::string dbPath2 = dataPath.string() + "\\db.db";
    sqlite3* db;
    int rc = sqlite3_open(dbPath2.c_str(), &db);
    if (rc != 0) {
        spdlog::error("数据库打开失败");
    }
    DB::init(db);
}
