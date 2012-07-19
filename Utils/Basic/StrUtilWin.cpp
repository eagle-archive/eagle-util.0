#include <afx.h>         // MFC core and standard components
#include <string>
#include "StrUtil.h"
#include <shlobj.h>


#if defined(_MSC_VER) && defined(_DEBUG)
#define new DEBUG_NEW
#endif


namespace utils {

bool WideStrToAnsiStr(std::string &str, const WCHAR * wstr)
{
    bool result = false;
    char *buff = NULL;

	str.clear();

    if( NULL != wstr )
    {
        int size = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
        if (size > 0)
        {
            buff = new char[size];
            if (NULL != buff)
            {
                if (0 < WideCharToMultiByte(CP_ACP, 0, wstr, -1, buff, size, NULL, NULL))
                {
                    str = buff;
                    result = TRUE;
                }
            }
        }
    }

    if (buff) delete []buff;
    return result;
}


}
