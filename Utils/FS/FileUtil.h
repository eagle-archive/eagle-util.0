#ifndef _FILE_UTIL_H
#define _FILE_UTIL_H

#include <string>

namespace utils {

bool FileExists(const char* pathname);
bool DirectoryExists(const char* directory);
bool RecursiveCreateDirectory(const char *directory);

std::string GetCurModulePathname();
std::string GetFilePath(const char* pathname);
std::string GetFileName(const char* pathname);
std::string GetFileTitle(const char* pathname);

}

#endif // _FILE_UTIL_H
