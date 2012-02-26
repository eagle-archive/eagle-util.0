#include <afxwin.h>         // MFC core and standard components
#include <string>
#include "FileUtil.h"
#include <shlobj.h>

#if defined(_MSC_VER) && defined(_DEBUG)
#define new DEBUG_NEW
#endif


namespace utils {

bool FileExists(const char* pathname)
{
    DWORD att = ::GetFileAttributes(pathname);
    if (att == (DWORD)-1)
        return FALSE;
    if(att & FILE_ATTRIBUTE_DIRECTORY)
        return false;
    else
        return true;
}

bool RecursiveCreateDirectory(const char *directory)
{
    int res = SHCreateDirectoryEx(NULL, directory, NULL);
    return (ERROR_SUCCESS == res);
}

// Checks the existence of a Windows directory.
bool DirectoryExists(const char* directory)
{
    DWORD att = ::GetFileAttributes(directory);
    if (att == (DWORD)-1)
        return false;
    return (att & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

std::string GetCurModulePathname()
{
    std::string name;
    TCHAR buff[MAX_PATH];
    DWORD res = GetModuleFileNameA(NULL, buff, MAX_PATH);
    if (res != 0)
        name = buff;
    return name;
}

// "c:\folder\file.txt" => "c:\folder\"
std::string GetFilePath(const char* pathname)
{
    std::string path;
    if (pathname)
        path = pathname;
    size_t k = path.rfind('\\');
    if (k == std::string::npos)
    {
        path.clear();
    }
    else
    {
        path = path.substr(0, k+1);
    }
    return path;
}

// "c:\folder\file.txt" => "file.txt"
std::string GetFileName(const char* pathname)
{
    std::string name;
    if (pathname)
        name = pathname;
    size_t k = name.rfind('\\');
    if (k != std::string::npos)
    {
        name = name.substr(k+1);
    }
    return name;
}

// "c:\folder\file.txt" => "file"
std::string GetFileTitle(const char* pathname)
{
    std::string title, name;
    name = GetFileName(pathname);
    if (name.empty())
        return name;

    size_t k = name.rfind('.');
    if (k != std::string::npos)
    {
        title = name.substr(0, k);
    }
    else
    {
        title = name;
    }

    return title;
}

}
