#ifndef _STR_UTIL_H
#define _STR_UTIL_H

#include <string>

namespace utils {

bool WideStrToAnsiStr(std::string &str, const WCHAR * wstr);

}

#endif // _STR_UTIL_H
