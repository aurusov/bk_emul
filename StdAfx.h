// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A0AA5235_1165_461C_83E2_200ED165258A__INCLUDED_)
#define AFX_STDAFX_H__A0AA5235_1165_461C_83E2_200ED165258A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma warning( disable : 4786 )

#include <string>
#include <vector>

std::string format( const char* str, ... );
std::string format( UINT resource, ... );
void trim( std::string& str );
void trimLeft( std::string& str );
void trimRight( std::string& str );

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A0AA5235_1165_461C_83E2_200ED165258A__INCLUDED_)
