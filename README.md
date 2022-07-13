计算机\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows Defender\Exclusions\Paths

Add-MpPreference -ExclusionPath "C:\Users\liuxiaolun\AppData\Roaming\Electron Fiddle\electron-bin\19.0.4"

https://xz.aliyun.com/t/10317
https://cloud.tencent.com/developer/article/1900214
https://docs.microsoft.com/en-us/windows/win32/wmisdk/example--getting-wmi-data-from-the-local-computer
https://github.com/MicrosoftDocs/microsoft-365-docs/blob/public/microsoft-365/security/defender-endpoint/configure-process-opened-file-exclusions-microsoft-defender-antivirus.md
https://docs.microsoft.com/en-us/previous-versions/windows/desktop/defender/add-msft-mppreference

```c++
#define _WIN32_DCOM

#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

int main(int iArgCnt, char** argv)
{
    WCHAR exclpath[] = L"C:\\Users\\liuxiaolun\\AppData\\Roaming\\Electron Fiddle\\electron-bin\\19.0.4";
    HRESULT hr;
    hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        ::wprintf(L"[-] CoInitializeEx has failed\n");
        return 0;
    }

    hr = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );
    if (FAILED(hr))
    {
        ::wprintf(L"[-] CoInitializeSecurity has failed\n");
        CoUninitialize();
        return 0;
    }

    IWbemLocator* pLoc = 0;
    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
    if (FAILED(hr))
    {
        ::wprintf(L"[-] CoCreateInstance has failed\n");
        CoUninitialize();
        return 0;
    }

    IWbemServices* pSvc = 0;
    hr = pLoc->ConnectServer(BSTR(L"ROOT\\Microsoft\\Windows\\Defender"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
    if (FAILED(hr))
    {
        ::wprintf(L"[-] ConnectServer has failed\n");
        pLoc->Release();
        CoUninitialize();
        return 0;
    }

    hr = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );
    if (FAILED(hr))
    {
        ::wprintf(L"[-] CoSetProxyBlanket has failed\n");
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 0;
    }

    IWbemClassObject* pClass = 0;
    BSTR Clname = BSTR(L"MSFT_MpPreference");
    hr = pSvc->GetObject(Clname, 0, NULL, &pClass, NULL);

    BSTR MethodName = BSTR(L"Add");
    IWbemClassObject* pInSignature = 0;
    hr = pClass->GetMethod(MethodName, 0, &pInSignature, NULL);
    if (FAILED(hr))
    {
        ::wprintf(L"[-] GetMethod has failed\n");
        pInSignature->Release();
        pClass->Release();
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 0;
    }

    IWbemClassObject* pClassInstance = NULL;
    hr = pInSignature->SpawnInstance(0, &pClassInstance);
    if (FAILED(hr))
    {
        ::wprintf(L"[-] SpawnInstance has failed\n");
        pClassInstance->Release();
        pInSignature->Release();
        pClass->Release();
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 0;
    }

    // Create an array
    SAFEARRAYBOUND rgsaBounds[1];
    rgsaBounds[0].cElements = 1;
    rgsaBounds[0].lLbound = 0;
    SAFEARRAY* psaStrings;
    psaStrings = SafeArrayCreate(VT_BSTR, 1, rgsaBounds);

    // Add a string to the array
    VARIANT vString;
    VariantInit(&vString);
    V_VT(&vString) = VT_BSTR;
    V_BSTR(&vString) = _bstr_t(exclpath);
    LONG lArrayIndex = 0;
    SafeArrayPutElement(psaStrings, &lArrayIndex, V_BSTR(&vString));
    VariantClear(&vString);

    // variant array
    VARIANT vStringList;
    VariantInit(&vStringList);
    V_VT(&vStringList) = VT_ARRAY | VT_BSTR;
    V_ARRAY(&vStringList) = psaStrings;

    // Store the value for the in parameters
    hr = pClassInstance->Put(L"ExclusionPath", 0, &vStringList, CIM_STRING | CIM_FLAG_ARRAY);
    if (FAILED(hr))
    {
        ::wprintf(L"[-] Put has failed %x\n", hr);
        VariantClear(&vStringList);
        pClassInstance->Release();
        pInSignature->Release();
        pClass->Release();
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 0;
    }

    IWbemClassObject* pOutParams = NULL;
    hr = pSvc->ExecMethod(Clname, MethodName, 0, NULL, pClassInstance, NULL, NULL);
    if (FAILED(hr))
    {
        ::wprintf(L"[-] ExecMethod has failed %x\n", hr);
        VariantClear(&vStringList);
        pClassInstance->Release();
        pInSignature->Release();
        pClass->Release();
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 0;
    }

    VariantClear(&vStringList);
    pClassInstance->Release();
    pInSignature->Release();
    pClass->Release();
    pLoc->Release();
    pSvc->Release();
    CoUninitialize();

    return 1;
}
```