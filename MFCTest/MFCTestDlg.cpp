
// MFCTestDlg.cpp : 实现文件
//
//#define _AFXDLL
#include "stdafx.h"
#include "MFCTest.h"
#include "MFCTestDlg.h"
#include "afxdialogex.h"
#include <chrono>

#include <io.h>
#include <fcntl.h>
#include <stdio.h> 


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMFCTestDlg 对话框



CMFCTestDlg::CMFCTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCTEST_DIALOG, pParent)
	, m_nnMinSize(0)
	, m_nnMaxSize(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bIsInitModel = false;
	m_bIsBatchProcess = false;
	m_strBtFilePath = L"";
	m_bIsCUDASuceed = false;

	

}
CMFCTestDlg::~CMFCTestDlg()
{
	delete m_pDPTool;
	m_pDPTool = NULL;
}

void CMFCTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_CtrlMessageList);
	DDX_Control(pDX, IDC_COMBO2, m_nMinSize);
	DDX_Control(pDX, IDC_COMBO3, m_nMaxSize);
	DDX_CBIndex(pDX, IDC_COMBO2, m_nnMinSize);
	DDV_MinMaxInt(pDX, m_nnMinSize, 32, 2048);
	DDX_CBIndex(pDX, IDC_COMBO3, m_nnMaxSize);
	DDV_MinMaxInt(pDX, m_nnMaxSize, 64, 2048);
	DDX_Control(pDX, IDC_COMBO4, m_nMaxBatch);
	DDX_Control(pDX, IDC_CHECK3, m_check);
	DDX_Control(pDX, IDC_COMBO5, m_nWorkSpace);
	DDX_Control(pDX, IDC_CHECK4, m_nSaveEngPath);
}

BEGIN_MESSAGE_MAP(CMFCTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCTestDlg::OnBnClickedExecute)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCTestDlg::OnBnClickedOneSample)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCTestDlg::OnBnClickedBatchRead)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCTestDlg::OnBnClickedOnnxPath)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CMFCTestDlg::OnCbnSelchangeCombo2_minSize)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCTestDlg::OnBnClickedButton_Execute)
END_MESSAGE_MAP()


// CMFCTestDlg 消息处理程序

BOOL CMFCTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_pDPTool = new DeepLearningTool;
	GetcudaName();
	//最小最大输入尺寸设定
	m_nMinSize.InsertString(0, L"64");
	m_nMinSize.InsertString(1, L"128");
	m_nMinSize.InsertString(2, L"256");
	m_nMinSize.InsertString(3, L"512");
	m_nMinSize.InsertString(4, L"1024");
	m_nMinSize.InsertString(5, L"2048");
	m_nMaxSize.InsertString(0, L"64");
	m_nMaxSize.InsertString(1, L"128");
	m_nMaxSize.InsertString(2, L"256");
	m_nMaxSize.InsertString(3, L"512");
	m_nMaxSize.InsertString(4, L"1024");
	m_nMaxSize.InsertString(5, L"2048");
	m_nMinSize.SetCurSel(0);
	m_nMaxSize.SetCurSel(4);
	m_nMaxBatch.InsertString(0, L"1");
	m_nMaxBatch.InsertString(1, L"4");
	m_nMaxBatch.InsertString(2, L"8");
	m_nMaxBatch.InsertString(3, L"16");
	m_nMaxBatch.SetCurSel(0);
	m_check.SetCheck(1);
	m_nWorkSpace.InsertString(0, L"1024");
	m_nWorkSpace.InsertString(1, L"2048");
	m_nWorkSpace.SetCurSel(1);
	m_nSaveEngPath.SetCheck(1);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCTestDlg::OnBnClickedExecute()
{
	USES_CONVERSION;
	if (!m_srcGray.data&&!m_bIsBatchProcess)
	{
		MessageOut(L"请先读图!");
		return;
	}
	if (!m_bIsCUDASuceed)
	{
		MessageOut(L"读取显卡设备失败!");
		return;
	}
	if (!m_bIsInitModel)
	{
		//m_pDPTool->CreateTool(dpGlassClassifierTool);
		m_pDPTool->CreateTool(dpClassifierTool);
		//pDPTool->CreateTool(dpClassifierTool);
		if (!m_pDPTool->Init(m_ptagAIBaseParam))
		{
			MessageOut(L"Init:模型初始化失败!");
			return;
		}
		else
		{
			MessageOut(L"Init:模型初始化成功!");
		}
		if (!m_pDPTool->SetParam(m_ptagAIBaseParam))
		{
			MessageOut(L"SetParam:设置参数失败!");
			return;
		}
		else
		{
			MessageOut(L"SetParam:设置参数成功!");
		}
		m_bIsInitModel = true;
	}
	
	vector<DEF>vecDefImg;
	vector<REF>vecRefImg;
	DEF myDef, myDef1;
	vecDefImg.clear();
	vecRefImg.clear();
	vector<cv::Mat>nChannels;

	//是否图像先缩放至256x256
	
	if (!m_bIsBatchProcess)
	{
		if (((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck())
		{
			cv::resize(m_srcGray, m_srcGray, cv::Size(256, 256));
		}

		cv::split(m_srcGray, nChannels);
		myDef.m_ImgRGB.nWidth = m_srcGray.cols;
		myDef.m_ImgRGB.nHeight = m_srcGray.rows;
		myDef.m_ImgRGB.pRGBData[0] = nChannels.at(2).data;
		myDef.m_ImgRGB.pRGBData[1] = nChannels.at(1).data;
		myDef.m_ImgRGB.pRGBData[2] = nChannels.at(0).data;
		vecDefImg.push_back(myDef);

		int nForState = ((CButton *)GetDlgItem(IDC_CHECK2))->GetCheck();
		int nNo = 1;
		nNo = nForState > 0 ? 10000 : 1;
		double fMeanCost = 0.0f;
		for (int i = 0; i < nNo; i++)
		{

			auto start = std::chrono::system_clock::now();
			
			for (int nI=0; nI<150;nI++)
			{
				//auto startPre = std::chrono::system_clock::now();
				m_pDPTool->PreProcess(m_ptagAIBaseParam, m_ptagAIBaseResultParam, vecDefImg, vecRefImg);
				//auto endPre = std::chrono::system_clock::now();
				m_pDPTool->Execute(m_ptagAIBaseParam, m_ptagAIBaseResultParam);
				m_pDPTool->GetResult(m_ptagAIBaseParam, m_ptagAIBaseResultParam);
				
			
			}
			tagAIClassifierResult *pClasRes = (tagAIClassifierResult*)m_ptagAIBaseResultParam;
			auto end = std::chrono::system_clock::now();
			fMeanCost += (float)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			CString strDebug = L"";
			//strDebug.Format(L"Total inference time:%d us,Pre:%d us,class:%d,probability:%.4f", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(), std::chrono::duration_cast<std::chrono::microseconds>(endPre - startPre).count(), pClasRes->m_vecClassifierResult[0].nClassNo, pClasRes->m_vecClassifierResult[0].fProbabity);
			strDebug.Format(L"Total inference time:%d us,class:%d,probability:%.4f", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(), pClasRes->m_vecClassifierResult[0].nClassNo, pClasRes->m_vecClassifierResult[0].fProbabity);
			MessageOut(strDebug);
			
		}

		fMeanCost = nNo > 1 ? fMeanCost / 100.0 : fMeanCost;
		CString strCostTime = L"";
		strCostTime.Format(L"---------Total average cost timte:%fus", fMeanCost);
		MessageOut(strCostTime);
	}
	else
	{
		vecDefImg.clear();
		if (m_strArray.IsEmpty()|| m_strArray.GetSize()<=0)
		{
			MessageOut(L"请先选择文件夹路径!");
			return;
		}
		//保存图像
		CString strTime, strSaveDebug;
		//获取系统时间
		CTime tm;
		tm = CTime::GetCurrentTime();//获取系统日期
		strTime = tm.Format("%Y%m%d%H%M%S");
		strSaveDebug = L"D:\\DPDebug\\" + strTime;
		if (!PathIsDirectory(strSaveDebug))
		{

			::CreateDirectory(strSaveDebug, NULL);

		}

		double fMeanCost = 0.0f;
		string strFilename;
		for (int i = 0; i < m_strArray.GetSize(); i++)
		{
			vecDefImg.clear();
			CString strTemp = m_strArray.GetAt(i);
			//m_imgDisplay.Load(strTemp);
			//UpdateDisplay();
			String strPathFile = W2A(strTemp);
			Mat srcTemp = imread(strPathFile, -1);
			//获取图像名称
			//1.获取不带路径的文件名

			string::size_type iPos = strPathFile.find_last_of('\\') + 1;

			strFilename = strPathFile.substr(iPos, strPathFile.length() - iPos);

			//cout << filename << endl;

			if (((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck())
			{
				cv::resize(srcTemp, srcTemp, cv::Size(256, 256));
			}
			unsigned char*m_pData = srcTemp.data;
			if (m_pData == nullptr)
			{
				MessageOut(L"读取图像失败，跳过!");
				return;
			}
			cv::split(srcTemp, nChannels);
			myDef.m_ImgRGB.nWidth = srcTemp.cols;
			myDef.m_ImgRGB.nHeight = srcTemp.rows;
			myDef.m_ImgRGB.pRGBData[0] = nChannels.at(2).data;
			myDef.m_ImgRGB.pRGBData[1] = nChannels.at(1).data;
			myDef.m_ImgRGB.pRGBData[2] = nChannels.at(0).data;
			vecDefImg.push_back(myDef);

			auto start = std::chrono::system_clock::now();
			auto startPre = std::chrono::system_clock::now();
			m_pDPTool->PreProcess(m_ptagAIBaseParam, m_ptagAIBaseResultParam, vecDefImg, vecRefImg);
			auto endPre = std::chrono::system_clock::now();
			m_pDPTool->Execute(m_ptagAIBaseParam, m_ptagAIBaseResultParam);
			m_pDPTool->GetResult(m_ptagAIBaseParam, m_ptagAIBaseResultParam);
			tagAIClassifierResult *pClasRes = (tagAIClassifierResult*)m_ptagAIBaseResultParam;
			auto end = std::chrono::system_clock::now();
			fMeanCost += (float)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			CString strDebug = L"";
			strDebug.Format(L"Total inference time:%d us,Pre:%d us,class:%d,probability:%.4f", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(), std::chrono::duration_cast<std::chrono::microseconds>(endPre - startPre).count(), pClasRes->m_vecClassifierResult[0].nClassNo, pClasRes->m_vecClassifierResult[0].fProbabity);
			MessageOut(strDebug);
			CString ss,strSaveImg = L"";
			ss.Format(L"\\%d",pClasRes->m_vecClassifierResult[0].nClassNo);
			strSaveImg=strSaveDebug +ss;
			if (!PathIsDirectory(strSaveImg))
			{

				::CreateDirectory(strSaveImg, NULL);

			}

			USES_CONVERSION;
			cv::imwrite(W2A(strSaveImg + L"\\"+strFilename.c_str()), srcTemp);

		}
		fMeanCost/=(m_strArray.GetSize());
		CString strCostTime = L"";
		strCostTime.Format(L"---------Total average cost timte:%fus", fMeanCost);
		MessageOut(strCostTime);
	}
}
void CMFCTestDlg::MessageOut(CString str)
{
	m_CtrlMessageList.AddString(str);
	int m_Count = m_CtrlMessageList.GetCount();
	if (m_Count >= 1000) {
		m_CtrlMessageList.ResetContent();
	}
	m_CtrlMessageList.SetCurSel(m_Count - 1);//显示到最后一行

}

void CMFCTestDlg::OnBnClickedOneSample()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("位图文件（*.bmp）| *.bmp||"));
	if (dlg.DoModal() == IDOK)
	{
		m_bIsBatchProcess = false;
		CString strFilePath = dlg.GetPathName();
		if (strFilePath.Right(4) != _T(".bmp"))
		{
			strFilePath += _T(".bmp");
		}
		String strPathFile = W2A(strFilePath);
		m_strOneFilePath = strFilePath;

		m_srcGray = imread(strPathFile, 1);
		if (m_srcGray.data == nullptr ||m_srcGray.rows<=0 ||m_srcGray.cols<=0)
		{
			MessageOut(L"-------读图失败--------");
			return;
		}
		MessageOut(L"-------读图成功--------");
		//鼠标事件
		//cv::setMouseCallback("LoadImg", on_MouseHandle, this);
		//waitKey(0);
		//destroyAllWindows();

	}
}


void CMFCTestDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	int Count = m_CtrlMessageList.GetCount();
	for (int i = Count; i >= 0; i--)
		m_CtrlMessageList.DeleteString(i);
}


void CMFCTestDlg::OnBnClickedBatchRead()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR     szFolderPath[MAX_PATH] = { 0 };
	CString   strFolderPath = TEXT("");
	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("请选择处理结果存储路径");
	sInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_DONTGOBELOWDOMAIN;
	sInfo.lpfn = NULL;
	//清空数据
	m_strArray.RemoveAll();
	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		//取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			strFolderPath = szFolderPath;
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}
	CFileFind find;
	CString lpsz = strFolderPath + L"\\*.*";
	CString strPathFinal = strFolderPath + L"\\";
	BOOL IsFind = find.FindFile(lpsz);
	while (IsFind)
	{
		IsFind = find.FindNextFile();
		if (find.IsDots())
		{
			continue;
		}
		//文件
		else
		{
			//获得文件的路径
			CString m_strFile = find.GetFileName();
			CString extend = m_strFile.Right(m_strFile.GetLength() - m_strFile.ReverseFind('.') - 1);//取得扩展名
			if (extend == "bmp")//设置文件扩展名
			{
				m_strArray.Add(strPathFinal + m_strFile);
			}
		}
	}
	m_strBtFilePath = strFolderPath;
	m_bIsBatchProcess = true;
	//UpdateData(FALSE);
	find.Close();
	MessageOut(L"批量读取路径成功!");
}

bool CMFCTestDlg::GetcudaName()
{
	cudaDevicePropCuston devicePropTmp;
	nvmlReturn_t result;
	unsigned int device_count = 0;
	memset(&devicePropTmp, 0, sizeof(cudaDevicePropCuston));
	// First initialize NVML library
	result = nvmlInit();
	//获取数量
	result = nvmlDeviceGetCount(&device_count);
	if (NVML_SUCCESS != result)
	{
		m_bIsCUDASuceed = false;
		return false;
	}
	if (device_count<=0)
	{
		m_bIsCUDASuceed = false;
		return false;
	}
	for (int i = 0; i < 1; i++)
	{
		nvmlDevice_t device;
		//char name[NVML_DEVICE_NAME_BUFFER_SIZE];
		nvmlPciInfo_t pci;
		result = nvmlDeviceGetHandleByIndex(i, &device);
		if (NVML_SUCCESS != result) {
			std::cout << "get device failed " << endl;
		}
		//获取名字
		result = nvmlDeviceGetName(device, devicePropTmp.name, NVML_DEVICE_NAME_BUFFER_SIZE);
		strcpy_s(m_charCudaName, devicePropTmp.name);
		m_bIsCUDASuceed = true;
		CString strTemp(m_charCudaName);
		LPCTSTR   lp = (LPCTSTR)strTemp;
		GetDlgItem(IDC_EDIT1)->SetWindowText(lp);
		MessageOut(L"获取显卡设备成功!");
	}

}

void CMFCTestDlg::OnBnClickedOnnxPath()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("模型文件（*.onnx）| *.onnx||"));
	if (dlg.DoModal() == IDOK)
	{
		m_bIsBatchProcess = false;
		CString strFilePath = dlg.GetPathName();
		if (strFilePath.Right(5) != _T(".onnx"))
		{
			strFilePath += _T(".onnx");
		}
		String strPathFile = W2A(strFilePath);
		m_strOnnxPath = strFilePath;
		LPCTSTR   lp = (LPCTSTR)m_strOnnxPath;
		GetDlgItem(IDC_EDIT2)->SetWindowText(lp);
		CString strTemp;
		strTemp.Format(L"成功读取onnx路径:");
		strTemp += m_strOnnxPath;
		MessageOut(strTemp);

	}
}


void CMFCTestDlg::OnCbnSelchangeCombo2_minSize()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex = m_nMinSize.GetCurSel();

}


void CMFCTestDlg::OnBnClickedButton_Execute()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bIsCUDASuceed)
	{
		MessageOut(L"获取显卡设备失败，请检查显卡驱动等!");
	}
	if (m_strOnnxPath.IsEmpty())
	{
		MessageOut(L"请先读取onnx模型！再转换");
		return;
	}
	int nMinSizeIndex = m_nMinSize.GetCurSel();
	CString strSize = L"";
	strSize.Format(L"最小输入尺寸:%d", m_nArrInputSize[nMinSizeIndex]);
	MessageOut(strSize);
	int nMaxSizeIndex = m_nMaxSize.GetCurSel();
	strSize = L"";
	strSize.Format(L"最优输入尺寸：%d", m_nArrInputSize[nMaxSizeIndex]);
	MessageOut(strSize);
	strSize = L"";
	strSize.Format(L"最大输入尺寸：%d", m_nArrInputSize[nMaxSizeIndex]);
	MessageOut(strSize);
	int nBatch = min(16,m_nMaxBatch.GetCurSel());
	strSize = L"";
	strSize.Format(L"最大Batch设定：%d", m_nArrBatchSize[nBatch]);
	MessageOut(strSize);
	MessageOut(L"------------------------------");
	MessageOut(L"开始转换模型，请稍等！");
	CString strSaveEngine = L"";
	if (m_strOnnxPath.ReverseFind('.'))
	{
		strSaveEngine = m_strOnnxPath.Left(m_strOnnxPath.ReverseFind('.'))+L".engine";
	}
	LPCTSTR   lp = (LPCTSTR)strSaveEngine;
	GetDlgItem(IDC_EDIT3)->SetWindowText(lp);
	CString strCMD = L"";
	strCMD.Format(L"/k trtexec --onnx=%s --saveEngine=%s --fp16  --workspace=2048 --minShapes=input:1x3x%dx%d --optShapes=input:%dx3x%dx%d --maxShapes=input:%dx3x%dx%d",m_strOnnxPath, strSaveEngine, m_nArrInputSize[nMinSizeIndex], m_nArrInputSize[nMinSizeIndex], m_nArrBatchSize[nBatch], m_nArrInputSize[nMaxSizeIndex], m_nArrInputSize[nMaxSizeIndex], m_nArrBatchSize[nBatch], m_nArrInputSize[nMaxSizeIndex], m_nArrInputSize[nMaxSizeIndex]);
	//ShellExecute(NULL, L"open", L"cmd", strCMD, NULL, SW_SHOWMINIMIZED);

	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = L"cmd";
	ShExecInfo.lpParameters = strCMD;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	if (WaitForSingleObject(ShExecInfo.hProcess, INFINITE)== WAIT_OBJECT_0)
	{
		MessageOut(L"转换成功，模型保存在:"+strSaveEngine);
	}
	
	
	
}
