// AppDev.cpp : implementation file
//

#include "stdafx.h"
#include "test.h"
#include "AppDev.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAppDev dialog


CAppDev::CAppDev(CWnd* pParent /*=NULL*/)
	: CDialog(CAppDev::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAppDev)
	//}}AFX_DATA_INIT
}


void CAppDev::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppDev)
	DDX_Control(pDX, IDC_COMBO1, m_SectorNum1);
	DDX_Control(pDX, IDC_COMBO2, m_BlockNum1);
	DDX_Control(pDX, IDC_EDIT10, m_block3c);
	DDX_Control(pDX, IDC_EDIT8, m_block3a);
	DDX_Control(pDX, IDC_EDIT9, m_block3b);
	DDX_Control(pDX, IDC_EDIT7, m_block2);
	DDX_Control(pDX, IDC_EDIT6, m_block1);
	DDX_Control(pDX, IDC_EDIT5, m_block0);
	DDX_Control(pDX, IDC_EDIT4, m_password);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAppDev, CDialog)
	//{{AFX_MSG_MAP(CAppDev)
	ON_BN_CLICKED(IDC_BUTTON3, OnOpen)
	ON_BN_CLICKED(IDC_BUTTON4, OnFind)
	ON_BN_CLICKED(IDC_BUTTON5, OnButtonDefaultPW)
	ON_BN_CLICKED(IDC_BUTTON7, OnButtonReadBlock)
	ON_BN_CLICKED(IDC_RADIO1, OnRadioTypeA)
	ON_BN_CLICKED(IDC_RADIO2, OnRadioTypeB)
	ON_BN_CLICKED(IDC_BUTTON6, OnButtonReadSector)
	ON_BN_CLICKED(IDC_BUTTON8, OnButtonWriteBlock)
	ON_BN_CLICKED(IDC_BUTTON9, OnButtonInitializeWallet)
	ON_BN_CLICKED(IDC_BUTTON10, OnButtonCheckBalance)
	ON_BN_CLICKED(IDC_BUTTON11, OnButtonDeposit)
	ON_BN_CLICKED(IDC_BUTTON12, OnButtonCharge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppDev message handlers

void CAppDev::clearView()
{
	m_block0.SetWindowText("");
	m_block1.SetWindowText("");
	m_block2.SetWindowText("");
	m_block3a.SetWindowText("");
	m_block3b.SetWindowText("");
	m_block3c.SetWindowText("");
}

void CAppDev::StringToHex(CString str,unsigned char *buffer)
{
	str.MakeUpper();
	int i;
	unsigned char temp[2000];
	for(i=0;i<str.GetLength();i++){
		const char ch = str[i];
		if(ch>0x40){
			temp[i] = (unsigned char)ch - 0x37;
		}else{
			temp[i] = (unsigned char)atoi(&ch);
		}
	}
	int j;
	for(j=0,i=0;j<str.GetLength()/2;){
		buffer[j] = (unsigned char) (temp[i++]<<4);
		buffer[j++] |= temp[i++];
	}
}

void CAppDev::OnOpen() 
{
	// TODO: Add your control notification handler code here
	if(IDD_PowerOn() == IFD_OK) {
        // 启动
        isOpen = true;
        ((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText("已开启");
		clearView();
		//pswtype = 0x0b;
    }
    else {
        // 启动失败
        isOpen = false;
        ((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText("开启失败");
    }
}

void CAppDev::OnFind() 
{
	// TODO: Add your control notification handler code here
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
}

void CAppDev::OnButtonDefaultPW() 
{
	// TODO: Add your control notification handler code here
	if(pswtype==0x0B){
		m_password.SetWindowText("FFFFFFFFFFFF");
	}else{
		AfxMessageBox(_T("仅B类型秘钥可用"));
	}
}

void CAppDev::OnButtonReadBlock() //read_block
{
	clearView();
	// TODO: Add your control notification handler code here
	if(pswtype!=0x0A && pswtype!=0x0B){
		AfxMessageBox(_T("请选择秘钥类型"));
		return;
	}

	CString psw;
	m_password.GetWindowText(psw);
	int len = psw.GetLength();
	if(len==0){//if empty
		AfxMessageBox(_T("密码不能为空"));
		return;
	} else if (len!=12){
		AfxMessageBox(_T("密码长度错误"));
		return;
	}
	
	int SectorIndex = m_SectorNum1.GetCurSel(),BlockIndex = m_BlockNum1.GetCurSel();
	if(SectorIndex == CB_ERR || BlockIndex == CB_ERR){
		AfxMessageBox(_T("请选择扇区以及块号"));
		return;
	}
	if(len%2==1){
		psw = "0"+psw;
		len++;
	}
	unsigned char HexPSW[2000];
	StringToHex(psw,HexPSW);
	//m_block0.SetWindowText(psw);
	
	//CString test = "";
	//for(int w = 0;w<len/2;w++){
	//	CString s1;
	//	s1.Format("%02x",HexPSW[w]);
	//	test+=s1;
	//}
	//m_block1.SetWindowText(test);

	unsigned char des_data[2000];
	int des_len;
	int result = read_block(SectorIndex,BlockIndex,pswtype,HexPSW,des_data,&des_len);
	if(result<0){
		AfxMessageBox(_T("执行失败"));
		return;
	}

	CString show = "";
	if(BlockIndex == 3){
		for(int k=0;k<des_len;k++){
			CString s1;
			s1.Format("%02x",des_data[k]);
			show+=s1;
			if(k==5){
				m_block3a.SetWindowText(show);
				show.Empty();
			}
			else if(k==9){
				m_block3b.SetWindowText(show);
				show.Empty();
			}
		}
		m_block3c.SetWindowText(show);
	}else{
		for(int k=0;k<des_len;k++){
			CString s1;
			s1.Format("%02x",des_data[k]);
			show+=s1;
		}
		if(BlockIndex == 0) m_block0.SetWindowText(show);
		if(BlockIndex == 1) m_block1.SetWindowText(show);
		if(BlockIndex == 2) m_block2.SetWindowText(show);
	}
	return;
}

void CAppDev::OnRadioTypeA() 
{
	// TODO: Add your control notification handler code here
	pswtype = 0x0A;
}

void CAppDev::OnRadioTypeB() 
{
	// TODO: Add your control notification handler code here
	pswtype = 0x0B;
}

void CAppDev::OnButtonReadSector() 
{
	clearView();
	// TODO: Add your control notification handler code here
	if(pswtype!=0x0A && pswtype!=0x0B){
		AfxMessageBox(_T("请选择秘钥类型"));
		return;
	}

	CString psw;
	m_password.GetWindowText(psw);
	int len = psw.GetLength();
	if(len==0){//if empty
		AfxMessageBox(_T("密码不能为空"));
		return;
	} else if (len!=12){
		AfxMessageBox(_T("密码长度错误"));
		return;
	}
	
	int SectorIndex = m_SectorNum1.GetCurSel(),BlockIndex = m_BlockNum1.GetCurSel();
	if(SectorIndex == CB_ERR || BlockIndex == CB_ERR){
		AfxMessageBox(_T("请选择扇区以及块号"));
		return;
	}
	if(len%2==1){
		psw = "0"+psw;
		len++;
	}
	unsigned char HexPSW[2000];
	StringToHex(psw,HexPSW);
	//m_block0.SetWindowText(psw);
	
	//CString test = "";
	//for(int w = 0;w<len/2;w++){
	//	CString s1;
	//	s1.Format("%02x",HexPSW[w]);
	//	test+=s1;
	//}
	//m_block1.SetWindowText(test);

	unsigned char des_data[2000];
	for (BlockIndex = 0; BlockIndex < 4; BlockIndex++){
		int des_len;
		int result = read_block(SectorIndex,BlockIndex,pswtype,HexPSW,des_data,&des_len);
		if(result<0){
			AfxMessageBox(_T("执行失败"));
			return;
		}

		CString show = "";
		if(BlockIndex == 3){
			for(int k=0;k<des_len;k++){
				CString s1;
				s1.Format("%02x",des_data[k]);
				show+=s1;
				if(k==5){
					m_block3a.SetWindowText(show);
					show.Empty();
				}
				else if(k==9){
					m_block3b.SetWindowText(show);
					show.Empty();
				}
			}
			m_block3c.SetWindowText(show);
		}else{
			for(int k=0;k<des_len;k++){
				CString s1;
				s1.Format("%02x",des_data[k]);
				show+=s1;
			}
			if(BlockIndex == 0) m_block0.SetWindowText(show);
			if(BlockIndex == 1) m_block1.SetWindowText(show);
			if(BlockIndex == 2) m_block2.SetWindowText(show);
		}
	}
	return;
}

void CAppDev::OnButtonWriteBlock() 
{
	clearView();
	// TODO: Add your control notification handler code here
	if(pswtype!=0x0A && pswtype!=0x0B){
		AfxMessageBox(_T("请选择秘钥类型"));
		return;
	}

	CString psw;
	m_password.GetWindowText(psw);
	int len = psw.GetLength();
	if(len==0){//if empty
		AfxMessageBox(_T("密码不能为空"));
		return;
	} else if (len!=12){
		AfxMessageBox(_T("密码长度错误"));
		return;
	}
	
	int SectorIndex = m_SectorNum1.GetCurSel(),BlockIndex = m_BlockNum1.GetCurSel();
	if(SectorIndex == CB_ERR || BlockIndex == CB_ERR){
		AfxMessageBox(_T("请选择扇区以及块号"));
		return;
	}
	if(len%2==1){
		psw = "0"+psw;
		len++;
	}
	unsigned char HexPSW[2000];
	StringToHex(psw,HexPSW);

	unsigned char des_data[2000];
	int des_len, count=0;

	int flag = read_block(SectorIndex,BlockIndex,pswtype,HexPSW,des_data,&des_len);
	if (flag!=0){
		AfxMessageBox(_T("写块失败"));
		return;
	}

	CString msrc_data0, msrc_data1, msrc_data2;
	for (int k=0;k<des_len;k++){
		CString s1;
		s1.Format("%02x", des_data[k]);
		if (k<6) msrc_data0 += s1;
		else if (k<10) msrc_data1 += s1;
		else msrc_data2 += s1;
	}
	
	CString src_data0, src_data1, src_data2;
	unsigned char src_data[2000];
	int src_len;
	m_block3a.GetWindowText(src_data0);
	m_block3b.GetWindowText(src_data1);
	m_block3c.GetWindowText(src_data2);
	if (BlockIndex == 3){
		if (src_data0.IsEmpty()){
			src_data0 = msrc_data0;
		}
		if (src_data1.IsEmpty()){
			src_data1 = msrc_data1;
		}
		if (src_data2.IsEmpty()){
			src_data2 = msrc_data2;
		}
		CString data = src_data0 + src_data1 + src_data2;
		if (data.GetLength() %2 !=0){
			data.Insert(data.GetLength(), "0");
		}		
		StringToHex(data, src_data);
		src_len = data.GetLength()/2;
	} else {
		CString data = msrc_data0 + msrc_data1 + msrc_data2;
		StringToHex(data, src_data);
		src_len = data.GetLength()/2;
	}
	flag = write_block(BlockIndex, SectorIndex, pswtype, HexPSW, src_data, src_len);
	if (flag < 0){
		AfxMessageBox(_T("写入失败"));
		return;
	} else {
		AfxMessageBox(_T("写入成功"));
		return;
	}
	return;
}

void CAppDev::OnButtonInitializeWallet() 
{
	// TODO: Add your control notification handler code here
	clearView();
	// TODO: Add your control notification handler code here
	if(pswtype!=0x0A && pswtype!=0x0B){
		AfxMessageBox(_T("请选择秘钥类型"));
		return;
	}

	CString psw;
	m_password.GetWindowText(psw);
	int len = psw.GetLength();
	if(len==0){//if empty
		AfxMessageBox(_T("密码不能为空"));
		return;
	}
	int SectorIndex = m_SectorNum2.GetCurSel(),BlockIndex = m_BlockNum2.GetCurSel();
	if(SectorIndex == CB_ERR || BlockIndex == CB_ERR){
		AfxMessageBox(_T("请选择扇区以及块号"));
		return;
	}
	if(len%2==1){
		psw = "0"+psw;
		len++;
	}
	unsigned char HexPSW[2000];
	StringToHex(psw,HexPSW);
	long account=0;
	m_account.SetWindowText("dds");
	int des_len;
	int result = write_account(SectorIndex,BlockIndex,pswtype,HexPSW,account);
	if(result<0){
		AfxMessageBox(_T("执行失败"));
		walletinit=false;
		return;
	}
	walletinit=true;
	result = read_account(SectorIndex,BlockIndex,pswtype,HexPSW,&account);
	return;
}

void CAppDev::OnButtonCheckBalance() 
{
	// TODO: Add your control notification handler code here
	if(walletinit==false){
		AfxMessageBox(_T("钱包未初始化"));
		return;
	}
		CString psw;
	m_password.GetWindowText(psw);
	int len = psw.GetLength();
	if(len==0){//if empty
		AfxMessageBox(_T("密码不能为空"));
		return;
	}
	int SectorIndex = m_SectorNum2.GetCurSel(),BlockIndex = m_BlockNum2.GetCurSel();
	if(SectorIndex == CB_ERR || BlockIndex == CB_ERR){
		AfxMessageBox(_T("请选择扇区以及块号"));
		return;
	}
	if(len%2==1){
		psw = "0"+psw;
		len++;
	}
	unsigned char HexPSW[2000];
	StringToHex(psw,HexPSW);
	long balance=0;
	int result = read_account(SectorIndex,BlockIndex,pswtype,HexPSW,&balance);
	CString Balance=balance+'0';
	m_account.SetWindowText(Balance);
	return;
}

void CAppDev::OnButtonDeposit() 
{
	// TODO: Add your control notification handler code here
		if(walletinit==false){
		AfxMessageBox(_T("钱包未初始化"));
//		return;
	}
	CString psw;
	m_password.GetWindowText(psw);
	int len = psw.GetLength();
	if(len==0){//if empty
		AfxMessageBox(_T("密码不能为空"));
		return;
	}
	int SectorIndex = m_SectorNum2.GetCurSel(),BlockIndex = m_BlockNum2.GetCurSel();
	if(SectorIndex == CB_ERR || BlockIndex == CB_ERR){
		AfxMessageBox(_T("请选择扇区以及块号"));
		return;
	}
	if(len%2==1){
		psw = "0"+psw;
		len++;
	}
	unsigned char HexPSW[2000];
	StringToHex(psw,HexPSW);
	long balance=0;
	//int result = read_account(SectorIndex,BlockIndex,pswtype,HexPSW,&balance);
	CString Temp;
	m_deposit.GetWindowText(Temp);
	balance=balance+_ttol(Temp);
	CString Balance="";
	Balance.Format(_T("%ld"),balance);
	write_account(SectorIndex,BlockIndex,pswtype,HexPSW,balance);
	m_account.SetWindowText(Balance);
	return;
}

void CAppDev::OnButtonCharge() 
{
	// TODO: Add your control notification handler code here
		if(walletinit==false){
	//	AfxMessageBox(_T("钱包未初始化"));
	//	return;
	}
		CString psw;
	m_password.GetWindowText(psw);
	int len = psw.GetLength();
	if(len==0){//if empty
		AfxMessageBox(_T("密码不能为空"));
		return;
	}
	int SectorIndex = m_SectorNum2.GetCurSel(),BlockIndex = m_BlockNum2.GetCurSel();
	if(SectorIndex == CB_ERR || BlockIndex == CB_ERR){
		AfxMessageBox(_T("请选择扇区以及块号"));
		return;
	}
	if(len%2==1){
		psw = "0"+psw;
		len++;
	}
	unsigned char HexPSW[2000];
	StringToHex(psw,HexPSW);
	long balance=100;
	int result = read_account(SectorIndex,BlockIndex,pswtype,HexPSW,&balance);
	CString Balance="";
	CString Temp;
	m_charge.GetWindowText(Temp);
	if(balance<_ttol(Temp)){
		AfxMessageBox(_T("余额不足"));
		return;
	}
	balance=balance-_ttol(Temp);
	Balance.Format(_T("%ld"),balance);
	write_account(SectorIndex,BlockIndex,pswtype,HexPSW,balance);
	m_account.SetWindowText(Balance);
	return;
}
