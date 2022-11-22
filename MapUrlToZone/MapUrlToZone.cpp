// MapUrlToZone.cpp : This file contains the 'main' function. Program execution begins and ends there.
// A tool to know the Zone of a given url/network share.
//

#include <iostream>

#include <urlmon.h>

const char* rgZoneNames[] = { "Local", "Intranet", "Trusted", "Internet", "Restricted" };

void whatsTheZoneForThisUrl(LPCWSTR szUrl)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (!SUCCEEDED(hr))
    {
        printf("Error in CoInitializeEx! Error %08x", hr);
    }
    IInternetSecurityManager* pInetSecMgr;
    hr = CoCreateInstance(CLSID_InternetSecurityManager, NULL, CLSCTX_ALL,
        IID_IInternetSecurityManager, (void**)&pInetSecMgr);
    if (SUCCEEDED(hr))
    {
        DWORD dwZone;
        hr = pInetSecMgr->MapUrlToZone(szUrl, &dwZone, 0);
        if (hr == S_OK)
        {
            if (dwZone < 5)
                printf("ZONE: %s (%d)\n", rgZoneNames[dwZone], dwZone);
            else
                printf("ZONE: Unknown (%d)\n", dwZone);
        }
        else
            printf("ZONE: Error %08x\n", hr);

        pInetSecMgr->Release();
    }
    else
    {
        printf("Error in creating IID_IInternetSecurityManager instance! Error %08x", hr);
    }
    CoUninitialize();
}


int main()
{
    LPWSTR* szArglist;
    int nArgs;
    
    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    if ((NULL == szArglist) || nArgs != 2)
    {
        wprintf(L"Usage: MapUrlToZone.exe <url>\n");
        return 0;
    }
    whatsTheZoneForThisUrl(szArglist[1]);

    // Free memory allocated for CommandLineToArgvW arguments.
    LocalFree(szArglist);
    return 0;
}

