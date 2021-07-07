
// calibrationDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "calibration.h"
#include "calibrationDlg.h"
#include "afxdialogex.h"
#include "boardSetting.h"


#include <opencv2/opencv.hpp>
#include <iostream>  
#include <fstream>  
#include <stdio.h>
#define TEMP_BUFFER_SIZE 1005

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcalibrationDlg dialog



CcalibrationDlg::CcalibrationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CALIBRATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WORKPROGRESS, m_pWorkProgress);
}

BEGIN_MESSAGE_MAP(CcalibrationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_CNDTBTN, &CcalibrationDlg::OnBnClickedCndtbtn)
	ON_BN_CLICKED(IDC_EXPBTN, &CcalibrationDlg::OnBnClickedExpbtn)
	ON_BN_CLICKED(IDC_TESTBTN, &CcalibrationDlg::OnBnClickedTestbtn)
END_MESSAGE_MAP()


// CcalibrationDlg message handlers

BOOL CcalibrationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	image_count = 0;
	m_pWorkProgress.SetRange(0, 1000);
	m_pWorkProgress.SetPos(0);

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CcalibrationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CcalibrationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CcalibrationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CcalibrationDlg::CreateBitmapInfo(int w, int h, int bpp)
{
	if (m_pBitmapInfo != NULL)
	{
		delete[]m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}

	if (bpp == 8)
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24 or 32bit
		m_pBitmapInfo = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];

	m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biPlanes = 1;
	m_pBitmapInfo->bmiHeader.biBitCount = bpp;
	m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	m_pBitmapInfo->bmiHeader.biSizeImage = 0;
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biClrUsed = 0;
	m_pBitmapInfo->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	m_pBitmapInfo->bmiHeader.biWidth = w;
	m_pBitmapInfo->bmiHeader.biHeight = -h;
}

void CcalibrationDlg::DrawImage(Mat frame)
{
	CClientDC dc(GetDlgItem(IDC_PRVIEWCTL));

	CRect rect;
	GetDlgItem(IDC_PRVIEWCTL)->GetClientRect(&rect);

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0,frame.cols, frame.rows, frame.data, m_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}


void CcalibrationDlg::OnBnClickedCndtbtn()
{

	boardSetting dlg;
	
	if (dlg.DoModal() == IDOK) {

		if (dlg.m_pStringPath == "") {

			MessageBox(_T("Please choose folder path"));
			return;
		}
		
		
		
		// Convert a TCHAR string to a LPCSTR
		CT2CA pszConvertedAnsiString(dlg.m_pStringPath);
		// construct a std::string using the LPCSTR input
		std::string sPath(pszConvertedAnsiString);


		if (cornerDetect(sPath, dlg.m_nRowCnt, dlg.m_nColCnt) == false) {
			MessageBox(_T("Please choose correct folder again"));
		}

	}
	else {
		MessageBox(_T("Please choose folder path"));
		return;
	}
	// TODO: Add your control notification handler code here
}




bool CcalibrationDlg::cornerDetect(string path, int row_count, int col_count) {
	
	
	board_size = Size(row_count, col_count);
	vector<string> filename;


	cv::glob(path, filename);

	int curPos = 0;
	int step = 1000 / filename.size() - 1;

	for (int i = 0; i < filename.size(); i++) {
		
		Mat imageInput = imread(filename[i]);
		image_count++;
		curPos += step;
		m_pWorkProgress.SetPos(curPos);
		if (image_count == 1)  //
		{
			image_size.width = imageInput.cols;
			image_size.height = imageInput.rows;
		}

		if (0 == findChessboardCorners(imageInput, board_size, image_points_buf))
		{
			return false;

		}
		else
		{
			Mat view_gray;
			cvtColor(imageInput, view_gray, CV_RGB2GRAY);
			find4QuadCornerSubpix(view_gray, image_points_buf, Size(5, 5));
			image_points_seq.push_back(image_points_buf);    
			drawChessboardCorners(imageInput, board_size, image_points_buf, false);  
			char buff[200];
			sprintf_s(buff, "%02d", image_count);
			string m_int2str(buff);
			string filePath = m_int2str;
			string imageFileName = filePath + "_d.jpg";
			imwrite(imageFileName, imageInput);
			CreateBitmapInfo(imageInput.cols, imageInput.rows, imageInput.channels() * 8);
			DrawImage(imageInput);
			waitKey(1000);
       
		}

		
		
	}
	
	int total = image_points_seq.size();
	
	int CornerNum = board_size.width * board_size.height; 
	for (int ii = 0; ii < total; ii++)
	{
		if (0 == ii % CornerNum)   
		{
			int i = -1;
			i = ii / CornerNum;
			int j = i + 1;
		
		}
		if (0 == ii % 3)   
		{
			cout << endl;
		}
		else
		{
			cout.width(10);
		}
		
		
	}
	m_pWorkProgress.SetPos(1000);
	return true;
}

void CcalibrationDlg::OnBnClickedExpbtn()
{
	if (image_count <= 0) {
		MessageBox(_T("Please detect corner"));
		return;
	}

	CString str = _T("All files(*.*)|*.*|"); 
	
	CFileDialog dlg(FALSE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		CT2CA pszConvertedAnsiString(strPathName);
		// construct a std::string using the LPCSTR input
		std::string sPath(pszConvertedAnsiString);
		if (sPath == "") {
			MessageBox(_T("Please choose text file"));
			return;
		}
		if (calibration_work(sPath)) {
			MessageBox(_T("The camera calibration information file was successfully stored."));
		};
		
	}
	else {
		MessageBox(_T("Please choose text file"));
		return;

	}



	// TODO: Add your control notification handler code here
}


bool CcalibrationDlg::calibration_work(string path) {
	ofstream fout(path);
	Size square_size = Size(16.5, 16.5);  
	vector<vector<Point3f>> object_points; 
	
	cameraMatrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); 
	vector<int> point_counts;  
	distCoeffs = Mat(1, 5, CV_32FC1, Scalar::all(0));
	vector<Mat> tvecsMat; 
	vector<Mat> rvecsMat; 
	
	int i, j, t;
	m_pWorkProgress.SetPos(0);
	int step = 300 / image_count - 1;
	int curPos = 0;
	for (t = 0; t < image_count; t++)
	{
		curPos += step;
		m_pWorkProgress.SetPos(curPos);
		vector<Point3f> tempPointSet;
		for (i = 0; i < board_size.height; i++)
		{
			for (j = 0; j < board_size.width; j++)
			{
				Point3f realPoint;
				
				realPoint.x = (float)i * square_size.width;
				realPoint.y = (float)j * square_size.height;
				realPoint.z = (float)0;
				tempPointSet.push_back(realPoint);
			}
		}
		object_points.push_back(tempPointSet);
	}
	m_pWorkProgress.SetPos(300);
	step = 100 / image_count - 1;
	curPos = 300;
	for (i = 0; i < image_count; i++)
	{
		curPos += step;
		m_pWorkProgress.SetPos(curPos);
		point_counts.push_back(board_size.width * board_size.height);
	}
	
	calibrateCamera(object_points, image_points_seq, image_size, cameraMatrix, distCoeffs, rvecsMat, tvecsMat, 0);
	
	double total_err = 0.0; 
	double err = 0.0; 
	vector<Point2f> image_points2; 
	
	fout << "每幅图像的标定误差:\n";

	m_pWorkProgress.SetPos(400);
	step = 300 / image_count - 1;
	curPos = 400;

	for (i = 0; i < image_count; i++)
	{
		curPos += step;
		m_pWorkProgress.SetPos(curPos);
		vector<Point3f> tempPointSet = object_points[i];
		
		projectPoints(tempPointSet, rvecsMat[i], tvecsMat[i], cameraMatrix, distCoeffs, image_points2);
		
		vector<Point2f> tempImagePoint = image_points_seq[i];
		Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
		Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
		for (int j = 0; j < tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points2[j].x, image_points2[j].y);
			tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
		total_err += err /= point_counts[i];
		
		fout << "第" << i + 1 << "幅图像的平均误差:" << err << "像素" << endl;
	}
	
	fout << "总体平均误差:" << total_err / image_count << "像素" << endl << endl;
	
	
	Mat rotation_matrix = Mat(3, 3, CV_32FC1, Scalar::all(0)); 
	fout << "相机内参数矩阵:" << endl;
	fout << cameraMatrix << endl << endl;
	fout << "畸变系数:\n";
	fout << distCoeffs << endl << endl << endl;
	

	m_pWorkProgress.SetPos(700);
	step = 300 / image_count - 1;
	curPos = 700;
	for (int i = 0; i < image_count; i++)
	{
		curPos += step;
		m_pWorkProgress.SetPos(curPos);
		fout << "第" << i + 1 << "幅图像的旋转向量:" << endl;
		fout << tvecsMat[i] << endl;
		
		Rodrigues(tvecsMat[i], rotation_matrix);
		fout << "第" << i + 1 << "幅图像的旋转矩阵:" << endl;
		fout << rotation_matrix << endl;
		fout << "第" << i + 1 << "幅图像的平移向量:" << endl;
		fout << rvecsMat[i] << endl << endl;
	}
	fout << endl;
	m_pWorkProgress.SetPos(1000);
	return true;
}


void CcalibrationDlg::OnBnClickedTestbtn()
{
	if (image_count <= 0) {
		MessageBox(_T("Please detect corner"));
		return;
	}
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                     

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("Please choose Folder to Test");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               

	char szTemp[100];
	CString str;
	str.Format(_T("%s"), szBuffer);

	if (str == "") {

		MessageBox(_T("Please choose folder to test"));
		return;
	}
	if (test_work(str)) MessageBox(_T("The test was successful."));
}

bool CcalibrationDlg::test_work(CString path) {



	CT2CA pszConvertedAnsiString(path);
	// construct a std::string using the LPCSTR input
	std::string sPath(pszConvertedAnsiString);

	Mat mapx = Mat(image_size, CV_32FC1);
	Mat mapy = Mat(image_size, CV_32FC1);
	Mat R = Mat::eye(3, 3, CV_32F);
	
	string imageFileName;
	std::stringstream StrStm;

	m_pWorkProgress.SetPos(0);
	int step = 1000 / image_count;
	int curPos = 0;
	for (int i = 1; i <= image_count; i++)
	{

		curPos += step;
		m_pWorkProgress.SetPos(curPos);
		initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, image_size, CV_32FC1, mapx, mapy);
		StrStm.clear();
		imageFileName.clear();
		string stemp;
		char buff[200];
		sprintf_s(buff, "\\left%02d", i);
		string m_int2str(buff);
		stemp = sPath + m_int2str;
		Mat imageSource = imread(stemp + ".jpg");
		
		
		
		
		Mat newimage = imageSource.clone();  
		remap(imageSource, newimage, mapx, mapy, INTER_LINEAR);
		
		Mat temp = newimage.clone();
		putText(temp, "Calibration image", Point(0, 0), 1, 5, Scalar(255, 0, 0), 2);
		CreateBitmapInfo(temp.cols, temp.rows, temp.channels() * 8);
		DrawImage(temp);
		waitKey(2000);

		imageFileName = stemp + "_test_result.jpg";
		imwrite(imageFileName, newimage);
	}
	m_pWorkProgress.SetPos(1000);
	return true;
}
