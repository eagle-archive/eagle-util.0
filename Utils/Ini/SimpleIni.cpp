
#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "SimpleIni.h"
#include "FS/FileUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace utils {

CSimpleIni::CSimpleIni(const char * pathname)
{
    if (pathname != NULL)
        m_strPathname = pathname;
    else
        m_strPathname.clear();

    if (m_strPathname.empty())
    {
        std::string modulePathName = GetCurModulePathname();
        m_strPathname = GetFilePath(modulePathName.c_str()) + 
            GetFileTitle(modulePathName.c_str()) + ".ini";
    }

    if (!FileExists(m_strPathname.c_str()))
    {
        std::string path = GetFilePath(m_strPathname.c_str());
        if (!DirectoryExists(path.c_str()))
        {
            RecursiveCreateDirectory(path.c_str());
        }

        // create a dummy file if no ini file existing
        FILE*  fp = fopen(m_strPathname.c_str(), "wt");
        if (fp != NULL)
        {
            fwrite(" ", 1, 1, fp);
            fclose (fp);
        }
    }

}

CSimpleIni::~CSimpleIni(void)
{
}

std::string CSimpleIni::GetString(const char* entry,  const char* defaultValue)
{
    std::string strValue;

    return strValue;
}

bool CSimpleIni::WriteString(const char * lpszEntry, const char * lpszValue)
{
    // return WritePrivateProfileString(strSection, lpszEntry, lpszValue, m_strPathname);
    return true;
}

int CSimpleIni::GetInt(const char * lpszEntry, int nDefault)
{
    std::string strDefualtValue;
    //strDefualtValue.Format(_T("%d"), nDefault);
    //CString strValue = GetString(strSection, lpszEntry, strDefualtValue);

    int nValue;
    //_stscanf_s(strValue, _T("%d"), &nValue);
    return nValue;
}

bool CSimpleIni::WriteInt(const char * lpszEntry, int nValue)
{
    std::string strValue;
    //strValue.Format(_T("%d"), nValue);
    //return WriteString(strSection, lpszEntry, strValue);
    return true;
}

bool CSimpleIni::GetBoolean(const char * lpszEntry, bool bDefault)
{
    return 0 != GetInt(lpszEntry, (int)bDefault);
}

bool CSimpleIni::WriteBoolean(const char * lpszEntry, bool bValue)
{
    return WriteInt(lpszEntry, bValue);
}

}
