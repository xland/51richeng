#pragma once
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

