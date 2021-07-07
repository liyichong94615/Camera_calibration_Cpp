
// calibrationDlg.h : header file
//

#pragma once


#include <opencv2/opencv.hpp>
#include <iostream>  
#include <fstream>  
#include <stdio.h>

using namespace std;
using namespace cv;
// CcalibrationDlg dialog
class CcalibrationDlg : public CDialogEx
{
// Construction
public:
	CcalibrationDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALIBRATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCndtbtn();

	BITMAPINFO* m_pBitmapInfo;
	int image_count;
	Size image_size;  
	Size board_size;
	Mat cameraMatrix;
	Mat distCoeffs;
	vector<Point2f> image_points_buf;
	vector<vector<Point2f>> image_points_seq;
	bool cornerDetect(string path, int row_count, int col_count);
	void CreateBitmapInfo(int w, int h, int bpp);
	void DrawImage(Mat frame);

	bool calibration_work(string path);
	bool test_work(CString path);
	
	CProgressCtrl m_pWorkProgress;
	afx_msg void OnBnClickedExpbtn();
	afx_msg void OnBnClickedTestbtn();
};
