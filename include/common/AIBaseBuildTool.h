// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� DPLEARNINGV10_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// DPLEARNINGV10_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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
*brief deepLearning ͳһ���ࡢ��⡢�ָ�ӿ�
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
		nInDeviceIndex = 0;//Ĭ�ϰ���һ�ſ�����
		nInClassNum = 1;

		//����Ϊ����������������Ҫ��resize
		nInModelInputW = 512;
		nInModelInputH = 512;

		//���²���Ϊ��Ӧ��⡢�ָ̬�ߴ�
		nInInputImgMaxW = nInInputImgMaxH = 8192;
		nInInputImgMinW = nInInputImgMinH = 8192;

		nInInputMaxImgSize =nInInputImgMaxW*nInInputImgMaxH;

		fNmsThrd = 0.5;
		fProp = 0.5;

	}
	//����Ϊͨ�ò���
	char chInModelPathName[500];
	char chInInputNodeName[500];
	char chInOutputNodeName[500];
	char chConfigModelName[500];//����IPU������ģ������

	int nInMaxBatch;
	int nInDeviceIndex;//�豸(�Կ�)������
	int nInClassNum;
	
	//����Ϊ�������������Ҫ��resize
	int nInModelInputW;
	int nInModelInputH;

	//���²���Ϊ��Ӧ��⡢�ָ̬�ߴ�
	int nInInputImgMaxW;
	int nInInputImgMaxH;

	int nInInputImgMinW;
	int nInInputImgMinH;
	int nInInputMaxImgSize;

	//������
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
	AIInputChannelMode m_AITrainChannelMode;//by ls Ĭ��RGB��ͨ��
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

	int m_nBatchCur;//ʵ��ʹ�õ�batch
	int m_nImgWCur;//ʵ��ʹ�õ�ͼ���
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
