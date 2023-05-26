// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 DPLEARNINGV10_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// DPLEARNINGV10_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef DPLEARNINGV10_EXPORTS
#define DPLEARNINGV10_API __declspec(dllexport)
#else
#define DPLEARNINGV10_API __declspec(dllimport)
#endif

#pragma once
#include<vector>
#include <iostream>
#include <chrono>
#include <cmath>
using namespace std;

/*
*brief deepLearning 统一分类、检测、分割接口
* author @ls
* time 20220708

*/


typedef enum tagAIBuildToolType
{
	dpClassifierTool=0,
	dpDetectTool=1,
	dpSegmentionTool=2,
	dpGlassClassifierTool=3,
	dpSilkGlassClassifierTool=4,
	dpDCPClassifierTool=5,
}AIBuildToolType;

typedef enum tagChannelModeTpye
{
	dpSingleChannelMode=0,
	dpRGBChannelMode
}AIInputChannelMode;

typedef struct tagAICommonParam
{
	tagAICommonParam()
	{
		strcpy_s(chInInputNodeName, "input");
		strcpy_s(chInOutputNodeName, "output");
		strcpy_s(chConfigModelName, "DL");
		//chInInputNodeName = "input";
		//chInOutputNodeName = "output";
		nInMaxBatch = 1;
		nInDeviceIndex = 0;//默认按第一张卡索引
		nInClassNum = 1;

		//以下为分类或检测参数，由于要做resize
		nInModelInputW = 512;
		nInModelInputH = 512;

		//以下参数为适应检测、分割动态尺寸
		nInInputImgMaxW = nInInputImgMaxH = 8192;
		nInInputImgMinW = nInInputImgMinH = 8192;

		nInInputMaxImgSize =nInInputImgMaxW*nInInputImgMaxH;

		fNmsThrd = 0.5;
		fProp = 0.5;

	}
	//以下为通用参数
	char chInModelPathName[500];
	char chInInputNodeName[500];
	char chInOutputNodeName[500];
	char chConfigModelName[500];//接收IPU穿来的模块名称

	int nInMaxBatch;
	int nInDeviceIndex;//设备(显卡)索引号
	int nInClassNum;
	
	//以下为分类参数，由于要做resize
	int nInModelInputW;
	int nInModelInputH;

	//以下参数为适应检测、分割动态尺寸
	int nInInputImgMaxW;
	int nInInputImgMaxH;

	int nInInputImgMinW;
	int nInInputImgMinH;
	int nInInputMaxImgSize;

	//检测参数
	float fNmsThrd;
	float fProp;
}AICommonParams;

typedef struct tagAIBaseParam
{
	tagAIBaseParam() 
	{ 
		m_AIBuildToolType = dpClassifierTool;
		m_AITrainChannelMode = dpRGBChannelMode;
	};
	virtual ~tagAIBaseParam() { };
	AIBuildToolType m_AIBuildToolType;
	AIInputChannelMode m_AITrainChannelMode;//by ls 默认RGB三通道
	AICommonParams m_AICommonParam;
};

struct Img
{
	Img()
	{
		nWidth = 0;
		nHeight = 0;
		nChannels = 0;
		pImgData = NULL;
	}
	int nWidth;
	int nHeight;
	int nChannels;
	unsigned char* pImgData;
};

struct ImgRGB
{
	ImgRGB()
	{
		nWidth = 0;
		nHeight = 0;
		nChannles = 0;
		for (int i=0;i<3;i++)
		{
			pRGBData[i] = NULL;
		}
	}
	int nWidth;
	int nHeight;
	int nChannles;
	unsigned char* pRGBData[3];

};
struct ImgDbRGB
{
	int nWidth;
	int nHeight;
	int nChannles;
	unsigned char* pRGB2Data[6];

};
struct DEF 
{
	Img m_Img;
	ImgRGB m_ImgRGB;
};

struct REF
{
	Img m_Img;
	ImgRGB m_ImgRGB;
};

struct tagAIBaseResultParam
{
	tagAIBaseResultParam()
	{
		//m_AIBuildToolType = dpClassifierTool;
		m_nBatchCur = 1;
		m_nImgHCur = 285;
		m_nImgWCur = 285;
		m_nOutputSize = m_nImgHCur * m_nImgWCur;
	};
	virtual ~tagAIBaseResultParam() {};
	//AIBuildToolType m_AIBuildToolType;

	int m_nBatchCur;//实际使用的batch
	int m_nImgWCur;//实际使用的图像宽
	int m_nImgHCur;//
	int m_nOutputSize;

};
struct myClassifierResult
{
	myClassifierResult()
	{
		nClassNo = 0;
		fProbabity = 0.f;
	}
	int nClassNo;
	float fProbabity;
};

struct tagAIClassifierResult :tagAIBaseResultParam
{
	vector<myClassifierResult>m_vecClassifierResult;
};
class DPLEARNINGV10_API AIBaseBuildTool
{
public:
	AIBaseBuildTool() {};
	virtual ~AIBaseBuildTool() {};

public:
	virtual bool Init(tagAIBaseParam* pBaseParam)=0;
	virtual bool SetParam(tagAIBaseParam*pBaseParam)=0;

	virtual bool PreProcess(tagAIBaseParam*pBaseParam,tagAIBaseResultParam* pBaseResultParam,vector<DEF>vecInDefImg,vector<REF>vecInRefImg)=0;

	virtual bool Execute(tagAIBaseParam*pBaseParam, tagAIBaseResultParam* pBaseResultParam)=0;
	virtual bool GetResult(tagAIBaseParam*pBaseParam,tagAIBaseResultParam* pBaseResultParam)=0;
	virtual AIBuildToolType GetInspectToolType() = 0;

public:
	tagAIBaseParam m_tagAIBaseParams;
	tagAIBaseResultParam m_tagAIBaseResult;

};
