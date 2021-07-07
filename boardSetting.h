#pragma once


// boardSetting dialog

class boardSetting : public CDialogEx
{
	DECLARE_DYNAMIC(boardSetting)

public:
	boardSetting(CWnd* pParent = nullptr);   // standard constructor
	virtual ~boardSetting();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SZDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPathbtn();
	int m_nRowCnt;
	int m_nColCnt;

//	string m_sFldPath;
	CString m_pStringPath;
};
