
#include "InetUtil.h"
#include "Wininet.h"


BOOL GetIEProxy(BOOL &bEnable, CStringA &proxy, CStringA &byPass)
{
    BOOL result = TRUE;
    bEnable = FALSE;

    char buff[1024 * 5];
    DWORD len = sizeof(buff);
    result = InternetQueryOption(NULL, INTERNET_OPTION_PROXY, &buff, &len);
    if (result)
    {
        INTERNET_PROXY_INFO *pinfo = (INTERNET_PROXY_INFO *)buff;

        bEnable = (pinfo->dwAccessType == INTERNET_OPEN_TYPE_PROXY);
        proxy = pinfo->lpszProxy;
        byPass = pinfo->lpszProxyBypass;
    }

    if (!result)
    {
        DWORD err = ::GetLastError();
    }
    return result;
}

BOOL EnableIEProxy(BOOL bEnable, LPCSTR sIP,  UINT32 nPort, LPCSTR sBypass) 
{
    CString proxy, ip(sIP), bypass(sBypass);
    proxy.Format(_T("%s:%u"), ip, nPort);

    HKEY hKeyIn = HKEY_CURRENT_USER, hKeyOut; 
    if (ERROR_SUCCESS != RegOpenKeyEx(hKeyIn, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), 0, KEY_WRITE | KEY_READ, &hKeyOut)) 
    { 
        DWORD err = ::GetLastError();
        return FALSE; 
    }

    if(bEnable)
    {
        if (ERROR_SUCCESS != RegSetValueEx(hKeyOut, _T("ProxyServer"), 0, REG_SZ, (BYTE *)(LPCTSTR)proxy, (proxy.GetLength() + 1)* sizeof(TCHAR))) 
        { 
            RegCloseKey(hKeyOut); 
            return FALSE; 
        }
        if (ERROR_SUCCESS != RegSetValueEx(hKeyOut, _T("ProxyOverride"), 0, REG_SZ, (BYTE *)(LPCTSTR)bypass, (bypass.GetLength() + 1)* sizeof(TCHAR))) 
        { 
            RegCloseKey(hKeyOut); 
            return FALSE; 
        }
    }

    DWORD value = bEnable;
    if(ERROR_SUCCESS != RegSetValueEx(hKeyOut, _T("ProxyEnable"), 0, REG_DWORD, (BYTE *)&value, sizeof(DWORD))) 
    { 
        RegCloseKey(hKeyOut); 
        return FALSE; 
    }

    RegCloseKey(hKeyOut); 

/*
    if (bEnable)
    {
        INTERNET_PROXY_INFO proxyinfo = {INTERNET_OPEN_TYPE_PROXY, 0};
        CStringA proxyA(proxy);
        proxyinfo.lpszProxy = (LPSTR)(LPCSTR)(proxyA);
        proxyinfo.lpszProxyBypass = sBypass;
        BOOL result = InternetSetOption(NULL, INTERNET_OPTION_PROXY, &proxyinfo, sizeof(proxyinfo));
        DWORD err = ::GetLastError();
    }
*/
    DWORD result;
    result = InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
    result = InternetSetOption(NULL,INTERNET_OPTION_REFRESH, NULL,0);

    Sleep(1); Sleep(1);
    return TRUE; 
}

// Function name is copied from Chrome (chromium\src\chrome\browser\ui\webui\options\advanced_options_utils_win.cc)
void ShowNetworkProxySettings()
{
    ShellExecute(NULL, TEXT("open"), TEXT("rundll32.exe"),
        TEXT("shell32.dll,Control_RunDLL Inetcpl.cpl,,4"), NULL, SW_SHOWNORMAL);
}
