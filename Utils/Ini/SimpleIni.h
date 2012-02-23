#ifndef _SIMPLE_INI_H_
#define _SIMPLE_INI_H_

#include <string>

namespace utils {

class CSimpleIni
{
public:
    CSimpleIni(const char* pathname = NULL);
    virtual ~CSimpleIni(void);

public:
    std::string GetString(const char* entry, const char * lpszDefault);
    bool WriteString(const char * entry, const char * lpszValue);

    int  GetInt(const char * entry, int nDefault);
    bool WriteInt(const char * entry, int nValue);

    bool GetBoolean(const char * entry, bool bDefault);
    bool WriteBoolean(const char * entry, bool bValue);

protected:
    std::string     m_strPathname;
    //void*           m_IniFile; // IniFile*
};

}

#endif // _SIMPLE_INI_H_
