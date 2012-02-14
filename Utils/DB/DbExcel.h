#pragma once

#include "DbBasics.h"

class DbExcel
{
public:
    DbExcel();
    ~DbExcel();

public:
    bool Open(const char *filename);
    bool Open(const WCHAR *wfilename);
    bool Close();

    bool Save();
    bool SaveAs(const char *filename);
    bool SaveAs(const WCHAR *wfilename);
    bool SelectSheet(const char *sheet);
    bool SelectSheet(const WCHAR *wsheet);
    bool SelectSheet(int index);

    // iRow: 1-based index, chCol: 'A' ~ 'Z'
    bool GetCell(int row, int col, Variant& var) const;
    bool GetCell(int row, const char* col, Variant& var) const;
    bool SetCell(int row, int col, const Variant& var);
    bool SetCell(int row, const char* col, const Variant& var);
    bool GetCell(int row, const char* col, CStringW& wstr) const;
    bool SetCell(int row, const char* col, const WCHAR* wstr);
    bool GetCell(int row, const char* col, CStringA& str) const;
    bool SetCell(int row, const char* col, const char* str);

    // iRowStart, iRowEnd: 1-based index
    bool GetRange(int iRowStart, int iRowEnd, const char* strColStart, const char* strColEnd, DbRange &range);
    int GetUsedRowCount() const;
    int GetUsedColCount() const;

private:
    CStringW    m_wfilename;
    void*       m_data;
};
