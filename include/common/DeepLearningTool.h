#include "stdafx.h"
#include "AIClassifierTool.h"
#include "AIdetectBuildTool.h"
#include "AISegmentBuildTool.h"
#include "AIGlassClassifierTool.h"
#include "AISilkGlassClassifierTool.h"
#include "AIDCPClassifierTool.h"
/*
*brief deepLearning 统一分类、检测、分割接口
* time 20220708
步骤：
	1、CreateTool()//显式指定分类、检测、分割任务类型
	2、Init()//加载模型及配置文件
	3、SetParam()//内存、显存分配
	4、PreProcess()//图像预处理
	5、Execute()//执行函数
	6、GetResult()//虚函数，子类重写

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