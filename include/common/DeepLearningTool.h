#include "stdafx.h"
#include "AIClassifierTool.h"
#include "AIdetectBuildTool.h"
#include "AISegmentBuildTool.h"
#include "AIGlassClassifierTool.h"
#include "AISilkGlassClassifierTool.h"
#include "AIDCPClassifierTool.h"
/*
*brief deepLearning ͳһ���ࡢ��⡢�ָ�ӿ�
* time 20220708
���裺
	1��CreateTool()//��ʽָ�����ࡢ��⡢�ָ���������
	2��Init()//����ģ�ͼ������ļ�
	3��SetParam()//�ڴ桢�Դ����
	4��PreProcess()//ͼ��Ԥ����
	5��Execute()//ִ�к���
	6��GetResult()//�麯����������д

*/

class DPLEARNINGV10_API DeepLearningTool
{
public:
	DeepLearningTool();
	~DeepLearningTool();

public:
	bool CreateTool(AIBuildToolType AIToolType);
	bool Init(tagAIBaseParam* &pBaseParam);
	bool SetParam(tagAIBaseParam* &pBaseParam);
	bool PreProcess(tagAIBaseParam* &pBaseParam, tagAIBaseResultParam* &pBaseResultParam, vector<DEF>vecInDefImg, vector<REF>vecInRefImg);
	bool Execute(tagAIBaseParam* &pBaseParam, tagAIBaseResultParam* &pBaseResultParam);
	bool GetResult(tagAIBaseParam* &pBaseParam, tagAIBaseResultParam* &pBaseResultParam);

private:
	AIBaseBuildTool *m_pAIBaseBuildTool;
	tagAIBaseParam *m_ptagAIBaseParam;
	tagAIBaseResultParam *m_ptagAIBaseResultParam;
};