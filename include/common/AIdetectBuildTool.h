#pragma once
#include "AIBaseBuildTool.h"
#include <vector>

struct DetBox
{
	float fBBox[4];
	float fConfScore;
	float fClassID;
};

struct tagAIDetectResult :tagAIBaseResultParam
{
	vector<vector<DetBox>> m_vecDetBox;
};

class DPLEARNINGV10_API AIdetectBuildTool :
	public AIBaseBuildTool
{
public:
	AIdetectBuildTool();
	virtual ~AIdetectBuildTool();
public:
	virtual bool Init(tagAIBaseParam* pBaseParam);
	virtual bool SetParam(tagAIBaseParam*pBaseParam);
	virtual bool PreProcess(tagAIBaseParam*pBaseParam, tagAIBaseResultParam* pBaseResultParam, vector<DEF>vecInDefImg, vector<REF>vecInRefImg);
	virtual bool Execute(tagAIBaseParam*pBaseParam, tagAIBaseResultParam* pBaseResultParam);
	virtual bool GetResult(tagAIBaseParam*pBaseParam,tagAIBaseResultParam* pBaseResultParam);
	virtual AIBuildToolType GetInspectToolType() { return dpDetectTool; };

public:
	tagAIDetectResult *m_pAIDetectResult;
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
public:
	int *m_pProb;
	unsigned char*m_pPropMap;//by ls 0527
	float *m_pProbFloat;
private:
	int Release();
	//unsigned char GetScaleVal(Img &img, float fRawX, float fRawY);
};

