#include "DB.h"
#include <filesystem>
#include <shlwapi.h>
#include <shlobj.h>
#include "sqlite/sqlite3.h"
#include <format>
#include "RmlUi/Core.h"

DB::DB() {
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath)))
    {
        PathAppend(szPath, L"\\Roaming\\51RiCheng");
        //todo error
    }
    std::filesystem::path dbPath{ szPath };
    if (!std::filesystem::exists(dbPath)) {
        std::filesystem::create_directories(dbPath);
    }
    dbPath /= "db.db";
    if (!std::filesystem::exists(dbPath)) {
        //todo 创建数据库
    }
    int rc  = sqlite3_open(dbPath.generic_string().c_str(), &db);
}
DB* DB::get() {
    if (instance == nullptr) {
        instance = new DB();
    }
    return instance;
}