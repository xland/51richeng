#include "DB.h"
#include <shlwapi.h>
#include <shlobj.h>
#include <format>
#include <fstream>
#include <iostream>
#include "RmlUi/Core.h"
#include "resource.h"
#include "AppData.h"


DB::DB(sqlite3* db):db {db} {
        
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
    return instance;
}
void DB::init(sqlite3* db) {
    instance = new DB(db);
}