#pragma once
#include "AIBaseBuildTool.h"



//struct myClassifierResult
//{
//	myClassifierResult()
//	{
//		nClassNo = 0;
//		fProbabity = 0.f;
//	}
//	int nClassNo;
//	float fProbabity;
//};
//
//struct tagAIClassifierResult:tagAIBaseResultParam
//{
//	vector<myClassifierResult>m_vecClassifierResult;
//};

class DPLEARNINGV10_API AIDCPClassifierTool :
	public AIBaseBuildTool
{
public:
	AIDCPClassifierTool();
	virtual ~AIDCPClassifierTool();

public:
	virtual bool Init(tagAIBaseParam* pBaseParam);
	virtual bool SetParam(tagAIBaseParam*pBaseParam);

	virtual bool PreProcess(tagAIBaseParam*pBaseParam, tagAIBaseResultParam* pBaseResultParam, vector<DEF>vecInDefImg, vector<REF>vecInRefImg);

	virtual bool Execute(tagAIBaseParam*pBaseParam, tagAIBaseResultParam* pBaseResultParam);
	virtual bool GetResult(tagAIBaseParam*pBaseParam,tagAIBaseResultParam* pBaseResultParam);
	virtual AIBuildToolType GetInspectToolType() { return dpDCPClassifierTool; };

public:
	tagAIClassifierResult* m_pAIClassifierResult;
private:
	void* m_pEngine;
	void* m_pRuntime;
	void* m_pContext;
	void* m_pStream;
	void* m_pLogger;
private:
	bool m_bSetParm;
	bool m_bIsDynamicShape;
	bool m_bIsDdInput;
	void* m_pBuffers[2];
	float *m_pData;
	int m_nMaxBatch;//最大batch 分配显存使用
					//unsigned char*m_pPropMapArr[16];
	unsigned char* img_device;

private:
	int *m_pProb;
	unsigned char*m_pPropMap;//by ls 0527
	float *m_pProbFloat;
private:
	int Release();
	unsigned char GetScaleVal(Img &img, float fRawX, float fRawY);
};

