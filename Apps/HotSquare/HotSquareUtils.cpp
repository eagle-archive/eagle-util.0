#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Windows.h>
#include <string>
#include "HotSquare.h"

using namespace std;



std::string FormatTimeStr(unsigned int uTimeMs)
{
    char buff[64];
    sprintf(buff, "%02d:%02d:%03d",
        (uTimeMs/60000) % 100, (uTimeMs/1000) % 100, uTimeMs % 1000);
    return buff;
}

static unsigned int g_dwStart = ::GetTickCount();
std::string ElapsedTimeStr() {
    return FormatTimeStr(::GetTickCount() - g_dwStart);
}
