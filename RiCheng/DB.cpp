#include "DB.h"
#include <filesystem>
#include <shlwapi.h>
#include <shlobj.h>
#include <format>
#include "RmlUi/Core.h"


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
        //todo 创建数据库
    }
    int rc  = sqlite3_open(dbPath.generic_string().c_str(), &db);
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