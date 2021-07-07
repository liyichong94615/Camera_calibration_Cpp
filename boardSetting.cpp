// boardSetting.cpp : implementation file
//

#include "pch.h"
#include "calibration.h"
#include "boardSetting.h"
#include "afxdialogex.h"


// boardSetting dialog

IMPLEMENT_DYNAMIC(boardSetting, CDialogEx)

boardSetting::boardSetting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SZDLG, pParent)
	, m_nRowCnt(9)
	, m_nColCnt(6)
	
	, m_pStringPath(_T(""))
{

}

boardSetting::~boardSetting()
{
}

void boardSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ROWCNCTEDT, m_nRowCnt);
	DDX_Text(pDX, IDC_COLCNCTEDT, m_nColCnt);
	//  DDX_Text(pDX, IDC_PATHEDIT, m_sFldPath);
	DDX_Text(pDX, IDC_PATHEDIT, m_pStringPath);
}


BEGIN_MESSAGE_MAP(boardSetting, CDialogEx)
	ON_BN_CLICKED(IDC_PATHBTN, &boardSetting::OnBnClickedPathbtn)
END_MESSAGE_MAP()


// boardSetting message handlers


void boardSetting::OnBnClickedPathbtn()
{
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // 경로저장 버퍼 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("Please choose Folder to detect");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               // 파일경로 읽어오기

		// 경로를 가져와 사용할 경우, Edit Control 에 값 저장
	CString str;
	str.Format(_T("%s"), szBuffer);
	SetDlgItemText(IDC_PATHEDIT, str);


	// TODO: Add your control notification handler code here
}
