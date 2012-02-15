#ifndef _SIMPLE_INI_H_
#define _SIMPLE_INI_H_

#include <string>

class CSimpleIni
{
public:
    CSimpleIni(const char* pathname = NULL);
    virtual ~CSimpleIni(void);

public:
    std::string GetString(const char* lpszEntry, const char * lpszDefault = NULL);
    bool WriteString(const char * lpszEntry, const char * lpszValue);

    int  GetInt(const char * lpszEntry, int nDefault);
    bool WriteInt(const char * lpszEntry, int nValue);

    bool GetBoolean(const char * lpszEntry, bool bDefault);
    bool WriteBoolean(const char * lpszEntry, bool bValue);

protected:
    std::string     m_strPathname;
};


#endif // _SIMPLE_INI_H_
