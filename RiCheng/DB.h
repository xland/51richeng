#pragma once
#include <vector>
#include <chrono>
#include "sqlite/sqlite3.h"
#include "ToDo.h"
class DB
{
public:
    DB(const DB&) = delete;
    DB& operator=(const DB&) = delete;
    static DB* get();
    void close();
    std::vector<ToDo> getTodo(std::chrono::year_month_day& day);
private:
    DB();
    sqlite3* db;
    inline static DB* instance{ nullptr };
};

