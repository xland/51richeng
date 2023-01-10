#include "DB.h"
#include <shlwapi.h>
#include <shlobj.h>

DB::DB() {
    TCHAR szPath[MAX_PATH];
    // Get path for each computer, non-user specific and non-roaming data.
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath)))
    {
        // Append product-specific path
        PathAppend(szPath, L"\\My Company\\My Product\\1.0\\");
    }
    //auto dbFolderPath = appDataPath + L"/" + wxString::FromUTF8(appName);
    //bool flag = wxDir::Exists(dbFolderPath);
    //if (!flag) wxDir::Make(dbFolderPath);
    //dbPath = dbFolderPath + L"/db.db";
}
DB* DB::get() {
    if (instance == nullptr) {
        instance = new DB();
    }
    return instance;
}