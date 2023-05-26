#include "stdafx.h"
#include "DeepLearningTool.h"

bool DeepLearningTool::CreateTool(AIBuildToolType AIToolType)
{
	switch (AIToolType)
	{
	case  dpClassifierTool:
		m_pAIBaseBuildTool = new AIClassifierTool();
		m_ptagAIBaseResultParam = new tagAIClassifierResult();
		m_ptagAIBaseParam = new tagAIBaseParam();
		break;
	case dpDetectTool:
		m_pAIBaseBuildTool = new AIdetectBuildTool();
		m_ptagAIBaseResultParam = new tagAIDetectResult();
		m_ptagAIBaseParam = new tagAIBaseParam();
		break;
	case dpSegmentionTool:
		m_pAIBaseBuildTool = new AISegmentBuildTool();
		m_ptagAIBaseResultParam = new tagAISegRusult();
		m_ptagAIBaseParam = new tagAIBaseParam();
		break;
	case  dpGlassClassifierTool:
		m_pAIBaseBuildTool = new AIGlassClassifierTool();
		m_ptagAIBaseResultParam = new tagAIClassifierResult();
		m_ptagAIBaseParam = new tagAIBaseParam();
		break;
	case  dpSilkGlassClassifierTool:
		m_pAIBaseBuildTool = new AISilkGlassClassifierTool();
		m_ptagAIBaseResultParam = new tagAIClassifierResult();
		m_ptagAIBaseParam = new tagAIBaseParam();
		break;
	case  dpDCPClassifierTool:
		m_pAIBaseBuildTool = new AIDCPClassifierTool();
		m_ptagAIBaseResultParam = new tagAIClassifierResult();
		m_ptagAIBaseParam = new tagAIBaseParam();
		break;
	default:
		break;
	}
	return true;
}

DeepLearningTool::DeepLearningTool()
{
	m_pAIBaseBuildTool = NULL;
	m_ptagAIBaseParam = NULL;
	m_ptagAIBaseResultParam = NULL;
}

DeepLearningTool::~DeepLearningTool()
{
	if (m_pAIBaseBuildTool)
	{
		delete m_pAIBaseBuildTool;
		m_pAIBaseBuildTool = NULL;
	}
	if (m_ptagAIBaseParam)
	{
		delete m_ptagAIBaseParam;
		m_ptagAIBaseParam = NULL;
	}
	if (m_ptagAIBaseResultParam)
	{
		delete m_ptagAIBaseResultParam;
		m_ptagAIBaseResultParam = NULL;
	}
}

bool DeepLearningTool::Init(tagAIBaseParam* &pBaseParam)
{
	//参数合法化内部判断，不再此判断
	if (!m_pAIBaseBuildTool->Init(m_ptagAIBaseParam))
	{
		return false;
	}
	pBaseParam = m_ptagAIBaseParam;
	return true;
}

bool DeepLearningTool::SetParam(tagAIBaseParam* &pBaseParam)
{
	if (!m_pAIBaseBuildTool->SetParam(m_ptagAIBaseParam))
	{
		return false;
	}
	pBaseParam = m_ptagAIBaseParam;
	return true;
}

bool DeepLearningTool::PreProcess(tagAIBaseParam* &pBaseParam, tagAIBaseResultParam* &pBaseResultParam, vector<DEF>vecInDefImg, vector<REF>vecInRefImg)
{
	if (vecInDefImg.size()<1)
	{
		return false;
	}
	if (!m_pAIBaseBuildTool->PreProcess(m_ptagAIBaseParam,m_ptagAIBaseResultParam,vecInDefImg,vecInRefImg))
	{
		return false;
	}
	pBaseParam = m_ptagAIBaseParam;
	pBaseResultParam = m_ptagAIBaseResultParam;
	return true;
}

bool DeepLearningTool::Execute(tagAIBaseParam* &pBaseParam, tagAIBaseResultParam* &pBaseResultParam)
{
	if (!m_pAIBaseBuildTool->Execute(m_ptagAIBaseParam,m_ptagAIBaseResultParam))
	{
		return false;
	}
	pBaseParam = m_ptagAIBaseParam;
	pBaseResultParam = m_ptagAIBaseResultParam;
	return true;
}

bool DeepLearningTool::GetResult(tagAIBaseParam* &pBaseParam, tagAIBaseResultParam* &pBaseResultParam)
{
	if (!m_pAIBaseBuildTool->GetResult(m_ptagAIBaseParam,m_ptagAIBaseResultParam))
	{
		return false;
	}
	pBaseParam = m_ptagAIBaseParam;
	pBaseResultParam = m_ptagAIBaseResultParam;
	return true;
}