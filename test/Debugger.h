#if !defined(AFX_DEBUGGER_H__C7E7DC1B_CD70_4803_8317_86C8997B2357__INCLUDED_)
#define AFX_DEBUGGER_H__C7E7DC1B_CD70_4803_8317_86C8997B2357__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Debugger.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDebugger dialog

#include "ZM124U.h"

class CDebugger : public CDialog
{
// Construction
public:
	CDebugger(CWnd* pParent = NULL);   // standard constructor
private:
	bool isOpen;
// Dialog Data
	//{{AFX_DATA(CDebugger)
	enum { IDD = IDD_DEBUGGER };
	CString	m_editState;
	CString	m_editNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDebugger)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDebugger)
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonFind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBUGGER_H__C7E7DC1B_CD70_4803_8317_86C8997B2357__INCLUDED_)
