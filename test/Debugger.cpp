// Debugger.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "Debugger.h"
#include <string>
#include <cstring>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDebugger dialog


CDebugger::CDebugger(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugger::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDebugger)
	m_editState = _T("");
	m_editNum = _T("");
	//}}AFX_DATA_INIT
}


void CDebugger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebugger)
	DDX_Text(pDX, IDC_EDIT1, m_editState);
	DDX_Text(pDX, IDC_EDIT2, m_editNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDebugger, CDialog)
	//{{AFX_MSG_MAP(CDebugger)
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonFind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebugger message handlers

void CDebugger::OnButtonOpen() 
{
	// TODO: Add your control notification handler code here
	if(IDD_PowerOn() == IFD_OK) {
        // 启动
        isOpen = true;
        ((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText("已开启");
    }
    else {
        // 启动失败
        isOpen = false;
        ((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText("开启失败");
    }
}

void CDebugger::OnButtonFind() 
{
	if(isOpen){
		unsigned char buffer[2000] ;
		int uid_len;
		int code = find_14443(buffer,&uid_len);
		CString uid="",temp;
		for(int i=0;i<uid_len;i++){
			temp.Format(_T("%02x"),buffer[i]);
			uid+=temp;
		}
		if(code==0){//成功读取
			GetDlgItem(IDC_EDIT2)->SetWindowText(uid);
		}else{//读取失败
			GetDlgItem(IDC_EDIT2)->SetWindowText("error");
		}
	}
	// TODO: Add your control notification handler code here
	
}
