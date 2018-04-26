#if !defined(AFX_APPDEV_H__BA3E115F_C067_4F8A_8ACA_7C7367D32B06__INCLUDED_)
#define AFX_APPDEV_H__BA3E115F_C067_4F8A_8ACA_7C7367D32B06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AppDev.h : header file
//

#include "ZM124U.h"


/////////////////////////////////////////////////////////////////////////////
// CAppDev dialog

class CAppDev : public CDialog
{
// Construction
public:
	CAppDev(CWnd* pParent = NULL);   // standard constructor
private:
	bool isOpen;
	bool walletinit;
	unsigned char pswtype;
// Dialog Data
	//{{AFX_DATA(CAppDev)
	enum { IDD = IDD_APPDEV };
	CEdit	m_charge;
	CEdit	m_deposit;
	CEdit	m_account;
	CButton	m_ButtonCharge;
	CButton	m_ButtonDeposit;
	CComboBox	m_BlockNum2;
	CComboBox	m_SectorNum2;
	CComboBox	m_SectorNum1;
	CComboBox	m_BlockNum1;
	CEdit	m_block3c;
	CEdit	m_block3a;
	CEdit	m_block3b;
	CEdit	m_block2;
	CEdit	m_block1;
	CEdit	m_block0;
	CEdit	m_password;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppDev)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAppDev)
	afx_msg void OnOpen();
	afx_msg void OnFind();
	afx_msg void OnButtonDefaultPW();
	afx_msg void clearView();
	afx_msg void OnButtonReadBlock();
	afx_msg void OnRadioTypeA();
	afx_msg void OnRadioTypeB();
	afx_msg void StringToHex(CString str,unsigned char *buffer);
	afx_msg void OnButtonReadSector();
	afx_msg void OnButtonWriteBlock();
	afx_msg void OnButtonInitializeWallet();
	afx_msg void OnButtonCheckBalance();
	afx_msg void OnButtonDeposit();
	afx_msg void OnButtonCharge();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPDEV_H__BA3E115F_C067_4F8A_8ACA_7C7367D32B06__INCLUDED_)
