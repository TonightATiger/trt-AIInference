
// MFCTestDlg.h : 头文件
//

#pragma once
#include "DeepLearningTool.h"
#include <vector>
#include "opencv2/opencv.hpp"
#include <chrono>
#include "afxwin.h"
#include "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\include\nvml.h"

using namespace cv;
using namespace std;

#ifdef _DEBUG
#pragma comment(lib,"DeepLearningTool15_x64ud.lib")
#define new DEBUG_NEW
#else
#pragma comment(lib,"DeepLearningTool15_x64u.lib")
#endif

#pragma comment(lib,"D:\\hwl\\DeepLearnintV1.0\\DPLearningV1.0\\lib\\nvml.lib")
// CMFCTestDlg 对话框

struct cudaDevicePropCuston
{
	char  name[256];//显卡名称
	unsigned int nDeviceNum;//显卡数量
	unsigned int nGPUutilization;//GPU利用率,返回：如5，意为5%；
	unsigned int nCudaMemutilization;//显存利用率,返回：如5，意为5%
	unsigned int nDeviceTemperature;//当前显卡设备温度

	unsigned int nTotalGPUMemory;//设备总共显存
	unsigned int nUsedGpuMemory;//设备使用显存
	unsigned int nFreeGpuMemory;//设备设备
};

class CMFCTestDlg : public CDialogEx
{
// 构造
public:
	CMFCTestDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMFCTestDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedExecute();
	afx_msg void OnBnClickedOneSample();

private:
	CString m_strOneFilePath;
	CString m_strBtFilePath;
	CString m_strOnnxPath;
	CStringArray            m_strArray;
	Mat m_srcGray;
	DeepLearningTool *m_pDPTool = NULL;
	tagAIBaseParam *m_ptagAIBaseParam = NULL;
	tagAIBaseResultParam *m_ptagAIBaseResultParam = NULL;
	bool m_bIsInitModel;
	bool m_bIsBatchProcess;
	char m_charCudaName[500];
	bool m_bIsCUDASuceed;
public:
	CListBox m_CtrlMessageList;
	void MessageOut(CString str);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedBatchRead();
private:
	bool GetcudaName();
	int m_nArrInputSize[6] = { 64,128,256,512,1024,2048 };
	int m_nArrBatchSize[4] = { 1,4,8,16 };
public:
	afx_msg void OnBnClickedOnnxPath();
	CComboBox m_nMinSize;
	CComboBox m_nMaxSize;
	int m_nnMinSize;
	int m_nnMaxSize;
	CComboBox m_nMaxBatch;
	CButton m_check;
	afx_msg void OnCbnSelchangeCombo2_minSize();
	CComboBox m_nWorkSpace;
	CButton m_nSaveEngPath;
	afx_msg void OnBnClickedButton_Execute();
};
