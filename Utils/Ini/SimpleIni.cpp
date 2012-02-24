
#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "SimpleIni.h"
#include "FS/FileUtil.h"
#include "libini/include/libini.h"

#if defined(_MSC_VER) && defined(_DEBUG)
#define new DEBUG_NEW
#endif

namespace utils {

static bool LocateToSection(ini_fd_t fd, const char *section)
{
    const char* curSection = ini_currentHeading(fd);
    if (curSection != NULL)
    {
        if (strcmp(section, curSection) == 0)
            return true;
    }

    if (0 == ini_locateHeading(fd, section))
        return true;
    else
        return false;

}

static bool LocateToEntry(ini_fd_t fd, const char *entry)
{
    const char* curEntry = ini_currentKey(fd);
    if (curEntry != NULL)
    {
        if (strcmp(entry, curEntry) == 0)
            return true;
    }
    if (0 == ini_locateKey(fd, entry))
        return true;
    else
        return false;
}

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
    }
}

CSimpleIni::~CSimpleIni(void)
{
}

std::string CSimpleIni::GetString(const char* section, const char* entry, const char* defaultValue)
{
    std::string strValue;
    if (defaultValue != NULL)
        strValue = defaultValue;
    ini_fd_t inifd = ini_open(m_strPathname.c_str(), "r", NULL);
    if (inifd != NULL)
    {
        if (true == LocateToSection(inifd, section) &&
            true == LocateToEntry(inifd, entry))
        {
            int len = ini_dataLength(inifd);
            char *buff = (char *)malloc(len + 1);
            if (buff != NULL)
            {
                if (ini_readString(inifd, buff, len + 1) >= 0)
                {
                    strValue = buff;
                }
                free(buff);
            }
        }
        ini_close(inifd);
    }
    return strValue;
}

bool CSimpleIni::WriteString(const char* section, const char* entry, const char* value)
{
    bool result = false;
    ini_fd_t inifd = ini_open(m_strPathname.c_str(), "w", NULL);
    if (inifd != NULL)
    {
#error to change here
        if (true == LocateToSection(inifd, section) &&
            true == LocateToEntry(inifd, entry))
        {
            if (ini_writeString(inifd, value) >= 0)
                result = true;
        }
        ini_close(inifd);
    }
    return result;
}

int CSimpleIni::GetInt(const char* section, const char * entry, int nDefault)
{
    std::string strDefualtValue;
    //strDefualtValue.Format(_T("%d"), nDefault);
    //CString strValue = GetString(strSection, entry, strDefualtValue);

    int nValue = 0;
    //_stscanf_s(strValue, _T("%d"), &nValue);
    return nValue;
}

bool CSimpleIni::WriteInt(const char* section, const char * entry, int nValue)
{
    std::string strValue;
    //strValue.Format(_T("%d"), nValue);
    //return WriteString(strSection, entry, strValue);
    return true;
}

bool CSimpleIni::GetBoolean(const char* section, const char* entry, bool defaultValue)
{
    return 0 != GetInt(section, entry, (int)defaultValue);
}

bool CSimpleIni::WriteBoolean(const char* section, const char* entry, bool value)
{
    return WriteInt(section, entry, value);
}

} // namespace utils
