// JIRADB.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


#define EXCEL_DB "D:\\Dev\\MyProgram\\Source\\SrcBase\\Apps\\JiraDB\\MotorolaCRs.xlsx"
#define SQLITE_DB "D:\\Dev\\MyProgram\\Source\\SrcBase\\Apps\\JiraDB\\MotorolaCRs.db"


// CJIRADBApp:
// See JIRADB.cpp for the implementation of this class
//

class CJIRADBApp : public CWinApp
{
public:
	CJIRADBApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CJIRADBApp theApp;
