// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#ifndef _AFXDLL
	#ifndef _UNICODE
		#ifdef _DEBUG
			#pragma comment(lib, "nafxcwd.lib")
		#else
			#pragma comment(lib, "nafxcw.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "uafxcwd.lib")
		#else
			#pragma comment(lib, "uafxcw.lib")
		#endif
	#endif
#else
	#ifndef _UNICODE
		#ifdef _DEBUG
			#pragma comment(lib, "mfc" _MFC_FILENAME_VER "d.lib")
			#pragma comment(lib, "mfcs" _MFC_FILENAME_VER "d.lib")
		#else
			#pragma comment(lib, "mfc" _MFC_FILENAME_VER ".lib")
			#pragma comment(lib, "mfcs" _MFC_FILENAME_VER ".lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "mfc" _MFC_FILENAME_VER "ud.lib")
			#pragma comment(lib, "mfcs" _MFC_FILENAME_VER "ud.lib")
		#else
			#pragma comment(lib, "mfc" _MFC_FILENAME_VER "u.lib")
			#pragma comment(lib, "mfcs" _MFC_FILENAME_VER "u.lib")
		#endif
	#endif
#endif

#ifdef _DLL
	#if !defined(_AFX_NO_DEBUG_CRT) && defined(_DEBUG)
		#pragma comment(lib, "msvcrtd.lib")
	#else
		#pragma comment(lib, "msvcrt.lib")
	#endif
#else
	#if !defined(_AFX_NO_DEBUG_CRT) && defined(_DEBUG)
		#pragma comment(lib, "libcmtd.lib")
	#else
		#pragma comment(lib, "libcmt.lib")
	#endif
#endif

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "winspool.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "windowscodecs.lib")



// TODO: reference additional headers your program requires here
