#pragma once
#include <filesystem>
class AppData
{
public:
	AppData(const AppData&) = delete;
	AppData& operator=(const AppData&) = delete;
	static AppData* get();
	static void init();
	std::filesystem::path dataPath;
private:
	AppData();
	void initDB();
	void initLogger();
	void openDB();
	void initDataPath();
	inline static AppData* instance{ nullptr };
};

