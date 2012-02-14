#ifndef _INET_UTIL_H
#define _INET_UTIL_H

#include <afxwin.h>         // MFC core and standard components


// For IE
BOOL EnableIEProxy(BOOL bEnable, LPCSTR sIP = NULL, UINT32 nPort = NULL, LPCSTR sBypass = NULL);
BOOL GetIEProxy(BOOL &bEnable, CStringA &proxy, CStringA &byPass);
void ShowNetworkProxySettings();


#endif // _INET_UTIL_H
