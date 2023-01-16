#pragma once
#include <vector>
#include <chrono>
#include "sqlite/sqlite3.h"
#include "ToDo.h"
#include <filesystem>
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
    void createDBFile(std::filesystem::path& dbPath);
    sqlite3* db;
    inline static DB* instance{ nullptr };
};

