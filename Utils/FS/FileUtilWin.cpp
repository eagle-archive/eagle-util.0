#include <afxwin.h>         // MFC core and standard components
#include "FileUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


bool FileExists(const char* pathname)
{
    DWORD att = ::GetFileAttributes(pathname);
    if (att == (DWORD)-1)
        return FALSE;
    if(att & FILE_ATTRIBUTE_DIRECTORY)
        return FALSE;
    else
        return TRUE;
}

// Checks the existence of a Windows directory.
bool DirectoryExists(const char* directory)
{
    DWORD att = ::GetFileAttributes(directory);
    if (att == (DWORD)-1)
        return FALSE;
    return (att & FILE_ATTRIBUTE_DIRECTORY) != 0;
}
