// Syndrome.h : main header file for the SYNDROME application
//

#if !defined(AFX_SYNDROME_H__DAF2F5B8_D0FC_11D6_B5C0_00C0F05B64CB__INCLUDED_)
#define AFX_SYNDROME_H__DAF2F5B8_D0FC_11D6_B5C0_00C0F05B64CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSyndromeApp:
// See Syndrome.cpp for the implementation of this class
//

class CSyndromeApp : public CWinApp
{
public:
	CSyndromeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSyndromeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSyndromeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNDROME_H__DAF2F5B8_D0FC_11D6_B5C0_00C0F05B64CB__INCLUDED_)
