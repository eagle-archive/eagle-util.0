
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

        // create a dummy ini file
        FILE *fp = fopen(m_strPathname.c_str(), "wb");
        if (fp)
        {
            fwrite(" ", 1, 1, fp);
            fclose(fp);
        }
    }

    m_iniFd = ini_open(m_strPathname.c_str(), "a", NULL);
}

CSimpleIni::~CSimpleIni(void)
{
    if (m_iniFd)
    {
        ini_close(m_iniFd);
        m_iniFd = NULL;
    }
}

bool CSimpleIni::Flush(void)
{
    if (m_iniFd)
    {
        return 0 == ini_flush(m_iniFd);
    }
    return false;
}

std::string CSimpleIni::GetString(const char* section, const char* entry, const char* defaultValue)
{
    std::string strValue;
    if (defaultValue != NULL)
        strValue = defaultValue;

    if (m_iniFd != NULL)
    {
        if (true == LocateToSection(m_iniFd, section) &&
            true == LocateToEntry(m_iniFd, entry))
        {
            int len = ini_dataLength(m_iniFd);
            if (len != -1)
            {
                char *buff = (char *)malloc(len + 1);
                if (buff != NULL)
                {
                    if (ini_readString(m_iniFd, buff, len + 1) >= 0)
                    {
                        strValue = buff;
                    }
                    free(buff);
                }
            }
        }
    }
    return strValue;
}

bool CSimpleIni::WriteString(const char* section, const char* entry, const char* value)
{
    bool result = false;
    if (m_iniFd != NULL)
    {
        // Ignore the returned values of below calls. if no section and entry are located,
        // it creates will a temp one.
        LocateToSection(m_iniFd, section);
        LocateToEntry(m_iniFd, entry);

        if (ini_writeString(m_iniFd, value) >= 0)
            result = true;
    }
    return result;
}

int CSimpleIni::GetInt(const char* section, const char * entry, int defaultValue)
{
    int value = defaultValue;
    if (m_iniFd != NULL)
    {
        if (true == LocateToSection(m_iniFd, section) &&
            true == LocateToEntry(m_iniFd, entry))
        {
            ini_readInt(m_iniFd, &value);
        }
    }
    return value;
}

bool CSimpleIni::WriteInt(const char* section, const char* entry, int value)
{
    bool result = false;
    if (m_iniFd != NULL)
    {
        // Ignore the returned values of below calls. if no section and entry are located,
        // it creates will a temp one.
        LocateToSection(m_iniFd, section);
        LocateToEntry(m_iniFd, entry);

        if (ini_writeInt(m_iniFd, value) >= 0)
            result = true;
    }
    return result;
}

bool CSimpleIni::GetBoolean(const char* section, const char* entry, bool defaultValue)
{
    bool value = defaultValue;
    if (m_iniFd != NULL)
    {
        if (true == LocateToSection(m_iniFd, section) &&
            true == LocateToEntry(m_iniFd, entry))
        {
            int iValue;
            if (ini_readBool(m_iniFd, &iValue) >= 0)
            {
                value = (iValue != 0);
            }
        }
    }
    return value;
}

bool CSimpleIni::WriteBoolean(const char* section, const char* entry, bool value)
{
    bool result = false;
    if (m_iniFd != NULL)
    {
        // Ignore the returned values of below calls. if no section and entry are located,
        // it creates will a temp one.
        LocateToSection(m_iniFd, section);
        LocateToEntry(m_iniFd, entry);

        if (ini_writeBool(m_iniFd, value) >= 0)
            result = true;
    }
    return result;
}

} // namespace utils
