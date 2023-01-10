#pragma once
#include <format>
#include "sqlite/sqlite3ext.h"
class DB
{
public:
    DB();
    DB(const DB&) = delete;
    DB& operator=(const DB&) = delete;
    static DB* get();
private:
    DB();
    sqlite3* db;
    inline static DB* instance{ nullptr };
};

