#pragma once
#include <filesystem>
class AppData
{
public:
	AppData(const AppData&) = delete;
	AppData& operator=(const AppData&) = delete;
	static AppData* get();
	static void init();
	std::filesystem::path appDataPath;
private:
	AppData();
	void initDB();
	void initLogger();
	void openDB();
	inline static AppData* instance{ nullptr };
};

