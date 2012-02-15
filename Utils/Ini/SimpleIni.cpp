
#include "SimpleIni.h"
#include "ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define MAX_STRING_LENGTH   1024



CSimpleIni::CSimpleIni(const char * pathname)
{
    m_strPathname = pathname;
    /*
    if (pathname == NULL)
    {
        CString modulePathName = GetCurModulePathname();
        m_strPathname = GetFilePath(modulePathName) + GetFileTitle(modulePathName) + _T(".ini");
    }

    if (!IsFileExist(m_strPathname))
    {
        CString path = GetFilePath(m_strPathname);
        if (!IsDirectoryExist(path))
        {
            SHCreateDirectoryEx(NULL, path, NULL);
        }
    }
    */
}

CSimpleIni::~CSimpleIni(void)
{
}

std::string CSimpleIni::GetString(const char * lpszEntry,  const char * lpszDefault/* = NULL*/)
{
    std::string strValue(lpszDefault);

    char buff[MAX_STRING_LENGTH];
    //GetPrivateProfileString(strSection, lpszEntry, lpszDefault, buff, MAX_STRING_LENGTH, this->m_strPathname);
    strValue = buff;

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
