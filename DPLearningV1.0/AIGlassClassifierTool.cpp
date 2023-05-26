#pragma once
#include "stdafx.h"
#include "AIGlassClassifierTool.h"

#include "cuda_utils.h"
#include "logging.h"
#include "commonCls.hpp"
#include "nvml.h"

#include<xmmintrin.h>
#include <immintrin.h>
#include <string.h>

string Tchar2StringGLC(TCHAR *pStr)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pStr, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[nLen * sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, pStr, -1, chRtn, nLen, NULL, NULL);
	std::string str(chRtn);
	return str;
}

LPCWSTR String2LpcwstrGLC(std::string strOrig)
{
	size_t nSize = strOrig.length() + 1;
	size_t nConvertedChars = 0;
	wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (strOrig.length() - 1));
	mbstowcs_s(&nConvertedChars, wcstring, nSize, strOrig.c_str(), _TRUNCATE);

	return wcstring;
}

void Tchar2CharGLC(const TCHAR * tchar, char * _char)
{
	int iLength;
	//获取字节长度  
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char   
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}
void doInferenceGlassClassification(IExecutionContext& context, cudaStream_t& stream, float*input, void **buffers, float* output, int batchSize, int nInPutModelW, int nInPutModelH, int nOutPutSize, bool bIsDbInput)
{
	if (bIsDbInput)
	{
		CUDA_CHECK(cudaMemcpyAsync(buffers[0], input, batchSize * 3 * 2 * nInPutModelW * nInPutModelH * sizeof(float), cudaMemcpyHostToDevice, stream));
	}
	else
	{
		CUDA_CHECK(cudaMemcpyAsync(buffers[0], input, batchSize * 3 * 1 * nInPutModelW * nInPutModelH * sizeof(float), cudaMemcpyHostToDevice, stream));
	}
	//context.enqueue(batchSize, buffers, stream, nullptr);
	context.enqueueV2(buffers, stream, nullptr);//by ls 20220605 为适应动态batch explicit batch 

	CUDA_CHECK(cudaMemcpyAsync(output, buffers[1], batchSize * nOutPutSize * sizeof(float), cudaMemcpyDeviceToHost, stream));

	//context.enqueue(batchSize, buffers, streamTest1, nullptr);
	//CUDA_CHECK(cudaMemcpyAsync(output, buffers[1], batchSize * nOutPutSize * sizeof(float), cudaMemcpyDeviceToHost, streamTest1));

	cudaStreamSynchronize(stream);
}
AIGlassClassifierTool::AIGlassClassifierTool()
{
	m_pAIClassifierResult =NULL;
}


AIGlassClassifierTool::~AIGlassClassifierTool()
{
	//delete m_pAIClassifierResult;
	Release();
}

int AIGlassClassifierTool::Release()
{

	ICudaEngine* engine = (ICudaEngine*)m_pEngine;
	IRuntime* runtime = (IRuntime*)m_pRuntime;
	IExecutionContext*context = (IExecutionContext*)m_pContext;
	if (m_bSetParm)
	{
		const int inputIndex = engine->getBindingIndex(m_tagAIBaseParams.m_AICommonParam.chInInputNodeName);
		const int outputIndex = engine->getBindingIndex(m_tagAIBaseParams.m_AICommonParam.chInOutputNodeName);
		assert(inputIndex == 0);
		assert(outputIndex == 1);

		CUDA_CHECK(cudaFree(m_pBuffers[inputIndex]));
		CUDA_CHECK(cudaFree(m_pBuffers[outputIndex]));
		// Release stream and buffers
		cudaStreamDestroy((__device_builtin__ struct CUstream_st *)m_pStream);

		CUDA_CHECK(cudaFree(img_device));
		//delete[] m_pData;// 2022.2.7 by ls into pinned memory
		CUDA_CHECK(cudaFreeHost(m_pData));
		delete[] m_pProb;
		delete[]m_pPropMap;
		delete[]m_pProbFloat;
		/*	for (int i=0;i<16;i++)
		{
		delete[]m_pPropMapArr[i];
		m_pPropMapArr[i] = NULL;
		}*/

	}

	if (context)
	{
		context->destroy();
		//delete context;
	}
	if (engine) {
		engine->destroy();
	}

	if (runtime)
	{
		runtime->destroy();
	}
	if (m_pLogger)
	{
		delete m_pLogger;
	}

	m_pLogger = NULL;
	engine = NULL;
	runtime = NULL;
	context = NULL;
	m_pData = NULL;
	m_pProb = NULL;
	m_pPropMap = NULL;
	m_bSetParm = false;

	return 1;
}
bool AIGlassClassifierTool::Init(tagAIBaseParam* pBaseParam)
{
	//1.1读取配置文件 待添加
	TCHAR exeFullPath[MAX_PATH]; // Full path
	std::string strPath = "";
	GetModuleFileName(NULL, exeFullPath, MAX_PATH); //获取带有可执行文件名路径
	strPath = Tchar2StringGLC(exeFullPath);
	int pos = strPath.find_last_of("\\", strPath.length());
	std::string strIniPath = strPath.substr(0, pos) + "\\DLConfig.ini";  // 返回不带有可执行文件名的路径
	if (pBaseParam == NULL || pBaseParam->m_AICommonParam.chInInputNodeName == NULL)
	{
		return false;
	}
	//char* chmodelname = pBaseParam->m_AICommonParam.chConfigModelName;
	char*chmodelname = "DL";//test
	WCHAR cls_modelName[256];
	memset(cls_modelName, 0, sizeof(cls_modelName));
	MultiByteToWideChar(CP_ACP, 0, chmodelname, strlen(chmodelname) + 1, cls_modelName, sizeof(cls_modelName) / sizeof(cls_modelName[0]));

	int nDLTemp = GetPrivateProfileInt(L"DL", L"DLTpye", 3, String2LpcwstrGLC(strIniPath));
	//test
	pBaseParam->m_AIBuildToolType = AIBuildToolType(nDLTemp); //dpClassifierTool;

	TCHAR szValue[MAX_PATH] = L"";
	char chInputName[MAX_PATH] = { 0 };
	GetPrivateProfileString(L"DL", L"InputName", L"input", szValue, MAX_PATH, String2LpcwstrGLC(strIniPath));
	wcstombs(chInputName, szValue, MAX_PATH);
	strcpy(pBaseParam->m_AICommonParam.chInInputNodeName, chInputName);
	//memcpy(pBaseParam->m_AICommonParam.chInInputNodeName, chInputName, strlen(chInputName));
	//pBaseParam->m_AICommonParam.chInInputNodeName = "input";

	char chOutputName[MAX_PATH] = { 0 };
	GetPrivateProfileString(L"DL", L"OutputName", L"output", szValue, MAX_PATH, String2LpcwstrGLC(strIniPath));
	wcstombs(chOutputName, szValue, MAX_PATH);
	//pBaseParam->m_AICommonParam.chInOutputNodeName = chOutputName;
	//memcpy(pBaseParam->m_AICommonParam.chInOutputNodeName, chOutputName, strlen(chOutputName));
	//pBaseParam->m_AICommonParam.chInOutputNodeName = "output";
	strcpy(pBaseParam->m_AICommonParam.chInOutputNodeName, chOutputName);
	char chModelPath[MAX_PATH] = { 0 };
	GetPrivateProfileString(L"DL", L"ModelPath", L"D:\\model\\moqie0712_s.engine", szValue, MAX_PATH, String2LpcwstrGLC(strIniPath));
	//Tchar2Char(szValue, chModelPath);
	wcstombs(chModelPath, szValue, MAX_PATH);
	//pBaseParam->m_AICommonParam.chInModelPathName = chModelPath;
	strcpy(pBaseParam->m_AICommonParam.chInModelPathName, chModelPath);
	nDLTemp = GetPrivateProfileInt(L"DL", L"ClassNum", 0, String2LpcwstrGLC(strIniPath));
	pBaseParam->m_AICommonParam.nInClassNum = nDLTemp;   //11

	int nModelW = GetPrivateProfileInt(L"DL", L"ModelW", 285, String2LpcwstrGLC(strIniPath));
	pBaseParam->m_AICommonParam.nInModelInputH = nModelW;           // 285;

	int nModelH = GetPrivateProfileInt(L"DL", L"ModelH", 285, String2LpcwstrGLC(strIniPath));
	pBaseParam->m_AICommonParam.nInModelInputW = nModelH;        // 285;

	Release();
	if (pBaseParam == NULL)
	{
		return false;
	}

	//1.2加载模型
	//std::string wts_name = pwts_name;
	std::string engine_name = pBaseParam->m_AICommonParam.chInModelPathName;
	// deserialize the .engine and run inference
	std::ifstream file(engine_name, std::ios::binary);
	if (!file.good()) {
		std::cerr << "read " << engine_name << " error!" << std::endl;
		return 0;
	}
	char *trtModelStream = nullptr;
	size_t size = 0;
	file.seekg(0, file.end);
	size = file.tellg();
	file.seekg(0, file.beg);
	trtModelStream = new char[size];
	assert(trtModelStream);
	file.read(trtModelStream, size);
	file.close();

	m_pLogger = new Logger;
	m_pRuntime = createInferRuntime(*(Logger*)m_pLogger);
	IRuntime* runtime = (IRuntime*)m_pRuntime;
	assert(runtime != nullptr);
	m_pEngine = runtime->deserializeCudaEngine(trtModelStream, size);
	ICudaEngine* engine = (ICudaEngine*)m_pEngine;
	assert(engine != nullptr);

	//test by ls 20220602 增加解析engine信息的功能

	nvinfer1::Dims dims = engine->getBindingDimensions(0);
	int nFlag = dims.d[1];
	if (nFlag == 3)
	{
		pBaseParam->m_AITrainChannelMode = dpRGBChannelMode;
	}
	else if (nFlag == 1)
	{
		pBaseParam->m_AITrainChannelMode = dpSingleChannelMode;
	}

	for (int i = 0; i < dims.nbDims; i++)
	{
		if (dims.d[i]/*>10 && dims.d[2]>0 && dims.d[3]>0*/ == -1)
		{
			m_bIsDynamicShape = true;
			break;
		}

	}
	memcpy(&m_tagAIBaseParams, pBaseParam, sizeof(tagAIBaseParam));

	//by ls 20220602 增加解析engine Max batch 信息
	//int32_t nBatch = engine->getMaxBatchSize();

	//
	IExecutionContext* context = (IExecutionContext*)m_pContext;
	if (context)
	{
		context->destroy();
		context = nullptr;
	}
	m_pContext = engine->createExecutionContext();

	context = (IExecutionContext*)m_pContext;
	assert(context != nullptr);
	delete[] trtModelStream;
	assert(engine->getNbBindings() == 2);


	return TRUE;
}

bool AIGlassClassifierTool::SetParam(tagAIBaseParam*pBaseParam)
{
	//2.1设置内存、显存分配
	//pBaseParam = &m_tagAIBaseParams;
	if (pBaseParam == NULL)
	{
		return false;
	}
	ICudaEngine* engine = (ICudaEngine*)m_pEngine;
	IRuntime* runtime = (IRuntime*)m_pRuntime;
	IExecutionContext* context = (IExecutionContext*)m_pContext;
	cudaSetDevice(pBaseParam->m_AICommonParam.nInDeviceIndex);
	const int inputIndex = engine->getBindingIndex(pBaseParam->m_AICommonParam.chInInputNodeName);
	//const int inputIndex = engine->getBindingIndex(chInputBlobName);
	const int outputIndex = engine->getBindingIndex(pBaseParam->m_AICommonParam.chInOutputNodeName);
	assert(inputIndex == 0);
	assert(outputIndex == 1);
	//cudaStream_t stream = (cudaStream_t)pstream;
	if (m_bSetParm)
	{
		CUDA_CHECK(cudaFree(m_pBuffers[inputIndex]));
		CUDA_CHECK(cudaFree(m_pBuffers[outputIndex]));
		// Release stream and buffers

		cudaStreamDestroy((cudaStream_t)m_pStream);
		//delete[] m_pData; // 2022.2.7 by ls into pinned memory
		CUDA_CHECK(cudaFreeHost(m_pData));
		delete[] m_pProb;
		delete[]m_pProbFloat;
	}
	//by ls 20220603 为适应动态尺寸 注释，改为解析模型获取输入模型尺寸
	/*this->m_nInputModelW = nInputModelW;
	this->m_nInputModelH = nInputModelH;*/
	//this->m_nClassNums = nClassNums;
	//this->m_nMaxOutPutBoxCounts = nMaxOutPutBoxCounts;
	//we assume the yololayer outputs no more than MAX_OUTPUT_BBOX_COUNT boxes that conf >= 0.1;
	//this->m_nOutputSize = nMaxOutPutBoxCounts * sizeof(Yolo::Detection) / sizeof(float) + 1;
	int nOutputSize = 1 * pBaseParam->m_AICommonParam.nInInputImgMaxW*pBaseParam->m_AICommonParam.nInInputImgMaxH /** sizeof(float)*/;//output 1x2x285x285
	CUDA_CHECK(cudaMalloc((void**)&img_device, pBaseParam->m_AICommonParam.nInMaxBatch * 6 * pBaseParam->m_AICommonParam.nInInputImgMaxW*pBaseParam->m_AICommonParam.nInInputImgMaxH));

	// Create GPU buffers on device
	CUDA_CHECK(cudaMalloc(&m_pBuffers[inputIndex], pBaseParam->m_AICommonParam.nInMaxBatch * 6 * pBaseParam->m_AICommonParam.nInInputImgMaxW*pBaseParam->m_AICommonParam.nInInputImgMaxH * sizeof(float)));
	CUDA_CHECK(cudaMalloc(&m_pBuffers[outputIndex], pBaseParam->m_AICommonParam.nInMaxBatch * nOutputSize * sizeof(float)));
	CUDA_CHECK(cudaStreamCreate((cudaStream_t *)&m_pStream));

	//m_pData = new float[nBatch * 3 * nInputModelH * nInputModelW]; //2022.2.7 by ls into pinned memory
	CUDA_CHECK(cudaMallocHost((void**)&m_pData, pBaseParam->m_AICommonParam.nInMaxBatch * 6 * pBaseParam->m_AICommonParam.nInInputImgMaxW*pBaseParam->m_AICommonParam.nInInputImgMaxH * sizeof(float)));
	//for (int i = 0; i < 3 * INPUT_H * INPUT_W; i++)
	//    data[i] = 1.0;
	m_pProb = new int[pBaseParam->m_AICommonParam.nInMaxBatch * nOutputSize];//by ls 2022.5.12 为适应唐博双输入分割模型
	m_pProbFloat = new float[pBaseParam->m_AICommonParam.nInMaxBatch * nOutputSize];
	m_pPropMap = new unsigned char[pBaseParam->m_AICommonParam.nInMaxBatch*nOutputSize];
	//for (int i=0;i<16;i++)
	//{
	//	m_pPropMapArr[i] = new unsigned char[m_nOutputSize];
	//}

	m_bSetParm = true;
	this->m_nMaxBatch = pBaseParam->m_AICommonParam.nInMaxBatch;

	return TRUE;
}


bool AIGlassClassifierTool::PreProcess(tagAIBaseParam*pBaseParam, tagAIBaseResultParam* pBaseResultParam, vector<DEF>vecInDefImg, vector<REF>vecInRefImg)
{
	pBaseParam = &m_tagAIBaseParams;
	if (pBaseParam == NULL)
	{
		return false;
	}
	int nDefNum = vecInDefImg.size();
	int nRefNum = vecInRefImg.size();

	//m_tagAIBaseResult.m_nBatchCur = nDefNum;//n c h w ,此为n
	pBaseResultParam->m_nBatchCur = nDefNum;
	if (vecInDefImg[0].m_Img.nWidth>0)
	{
		m_tagAIBaseResult.m_nImgWCur = vecInDefImg[0].m_Img.nWidth;
		m_tagAIBaseResult.m_nImgHCur = vecInDefImg[0].m_Img.nHeight;
	}
	else
	{
		m_tagAIBaseResult.m_nImgWCur = vecInDefImg[0].m_ImgRGB.nWidth;
		m_tagAIBaseResult.m_nImgHCur = vecInDefImg[0].m_ImgRGB.nHeight;
	}
	pBaseResultParam->m_nImgWCur = m_tagAIBaseResult.m_nImgWCur;
	pBaseResultParam->m_nImgHCur = m_tagAIBaseResult.m_nImgHCur;
	pBaseResultParam->m_nOutputSize = m_tagAIBaseResult.m_nImgWCur* m_tagAIBaseResult.m_nImgHCur;

	if (pBaseParam->m_AIBuildToolType ==3)
	{
		//玻璃分类by ls 2022-10-4
		cv::Mat srcMatArrray[3],srcMerge,srcResize,srcMakePad,srcBitnot;
		for (int b = 0; b<nDefNum; b++)
		{
			int nSrcCurWidth = vecInDefImg[b].m_ImgRGB.nWidth;
			int nSrcCurHeight = vecInDefImg[b].m_ImgRGB.nHeight;
			cv::Mat srcMatR(nSrcCurHeight, nSrcCurWidth, CV_8UC1, vecInDefImg[b].m_ImgRGB.pRGBData[0]);
			cv::Mat srcMatG(nSrcCurHeight, nSrcCurWidth, CV_8UC1, vecInDefImg[b].m_ImgRGB.pRGBData[1]);
			cv::Mat srcMatB(nSrcCurHeight, nSrcCurWidth, CV_8UC1, vecInDefImg[b].m_ImgRGB.pRGBData[2]);
			if (!srcMatR.data || !srcMatG.data || !srcMatB.data)
			{
				return false;
			}
			//cv::bitwise_not(srcMatG, srcMatG);
			srcMatArrray[0] = srcMatR;
			srcMatArrray[1] = srcMatG;
			srcMatArrray[2] = srcMatB;
			cv::merge(srcMatArrray, 3,srcMerge);
			//cv::imwrite("D:\\model\\123.bmp", srcMerge);
			int nDstResH = 1;
			int nDstResW =1;
			float fRatio = pBaseParam->m_AICommonParam.nInModelInputW*1.0 / (max(nSrcCurWidth, nSrcCurHeight));
			if (nSrcCurHeight<nSrcCurWidth)
			{
				nDstResH = fRatio*nSrcCurHeight;
				nDstResW = pBaseParam->m_AICommonParam.nInModelInputW;
			}
			else
			{
				nDstResH = pBaseParam->m_AICommonParam.nInModelInputH;
				nDstResW = fRatio*nSrcCurWidth;
			}
			cv::resize(srcMerge, srcResize, cv::Size(nDstResW, nDstResH));
			//cv::imwrite("D:\\model\\1234.bmp", srcResize);
			if (nSrcCurHeight<nSrcCurWidth)
			{
				cv::copyMakeBorder(srcResize, srcMakePad, (nDstResW - nDstResH) / 2, nDstResW-(nDstResW - nDstResH) / 2-nDstResH, 0, 0, cv::BORDER_CONSTANT,cv::Scalar(0, 0, 0));
			}
			else
			{
				cv::copyMakeBorder(srcResize, srcMakePad,0, 0, (nDstResH - nDstResW) / 2,nDstResH-(nDstResH - nDstResW) / 2-nDstResW, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
			}
			//cv::imwrite("D:\\model\\12345.bmp", srcMakePad);
			cv::split(srcMakePad, srcMatArrray);		
			cv::bitwise_not(srcMatArrray[1], srcMatArrray[1]);
			cv::merge(srcMatArrray,3, srcBitnot);
			cv::imwrite("D:\\model\\123456.bmp", srcBitnot);


			float fMS[3][2] = { { 0.485,0.229 },{ 0.456,0.224 },{ 0.406,0.225 }};
			//float fMS[3][2] = { { 0.0,1.0 },{ 0.0,1.0 },{ 0.0,1.0 } };
			__m128 e_Scale = _mm_set_ps1(1 / 255.0);

			//int nSize = vecDbRGB[0].nWidth*vecDbRGB[0].nHeight;
			int nSize = pBaseParam->m_AICommonParam.nInModelInputW*pBaseParam->m_AICommonParam.nInModelInputH;
			DWORD dwSize = nSize / 4;
			__m128 result;//临时变量
			float *pfTrans = new float[nSize];

			for (int j = 0; j < 3; j++)
			{
				unsigned char* uc_pixel = srcMatArrray[j].data;

				__m128 u = _mm_set_ps1(-fMS[j][0]);
				__m128 st = _mm_set_ps1(1.0 / fMS[j][1]);
				auto start = std::chrono::system_clock::now();
				for (int i = 0; i < nSize; i++)
				{
					*(pfTrans + i) = (float)*(uc_pixel + i);
				}
				auto end = std::chrono::system_clock::now();
				//std::cout << "类型转换: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us" << std::endl;

				for (DWORD i = 0; i<dwSize; i++)
				{
					//*(__m128*)((float*)uc_pixel + i * 4) = _mm_mul_ps(*(__m128*)((float*)uc_pixel + i * 4), e_Scale);
					result = _mm_load_ps(pfTrans + i * 4);
					//__m128 result = _mm_loadu_ps((float*)uc_pixel + i * 4);
					result = _mm_mul_ps(result, e_Scale);
					result = _mm_add_ps(result, u);

					//pfInputDevBuffer[j*imgDbRGB.nWidth*imgDbRGB.nHeight +i*4]= _mm_mul_ps(*(__m128*)((float*)uc_pixel + i * 4), st);
					result = _mm_mul_ps(result, st);
					_mm_storeu_ps(&m_pData[b * 3 * pBaseParam->m_AICommonParam.nInModelInputW*pBaseParam->m_AICommonParam.nInModelInputH + j*pBaseParam->m_AICommonParam.nInModelInputW*pBaseParam->m_AICommonParam.nInModelInputH + i * 4], result);
				}
			}
			delete[]pfTrans;
			pfTrans = NULL;

		}

	}
	else if (pBaseParam->m_AIBuildToolType == 1 || pBaseParam->m_AIBuildToolType == 2)
	{
		//配置错误,检查配置
		return false;
	}

	return TRUE;
}

bool AIGlassClassifierTool::Execute(tagAIBaseParam*pBaseParam, tagAIBaseResultParam* pBaseResultParam)
{
	auto  ExePreStart = std::chrono::system_clock::now();
	pBaseParam = &m_tagAIBaseParams;
	if (pBaseParam == NULL)
	{
		return false;
	}
	cudaStream_t stream = (cudaStream_t)m_pStream;
	//cudaStream_t stream1 = (cudaStream_t)m_pStream;
	ICudaEngine* engine = (ICudaEngine*)m_pEngine;
	IRuntime* runtime = (IRuntime*)m_pRuntime;
	IExecutionContext* context = (IExecutionContext*)m_pContext;

	//int nCountImg = m_tagAIBaseResult.m_nBatchCur;
	int nCountImg = pBaseResultParam->m_nBatchCur;
	if (nCountImg > m_nMaxBatch || nCountImg <= 0) {
		return -1;
	}
	int nImgWCur = m_tagAIBaseResult.m_nImgHCur;
	int nImgHCur = m_tagAIBaseResult.m_nImgHCur;

	//context->setBindingDimensions(0, Dims4(1, 6, 384, 384));
	//preprocess_img(vecDbRGB, m_pData);
	if (m_bIsDynamicShape)
	{
		if (m_bIsDdInput)
		{
			context->setBindingDimensions(0, Dims4(nCountImg, 6, nImgHCur, nImgWCur));//by ls 20220602 适应动态尺寸
		}
		else
		{
			//context->setBindingDimensions(0, Dims4(nCountImg, 3, nImgHCur, nImgWCur));//by ls 20220602 适应动态尺寸
			context->setBindingDimensions(0, Dims4(nCountImg, 3, pBaseParam->m_AICommonParam.nInModelInputW, pBaseParam->m_AICommonParam.nInModelInputH));//by ls 20220602 适应动态尺寸
		}
	}
	auto ExePreEnd = std::chrono::system_clock::now();
	auto ExePreT = std::chrono::duration_cast<std::chrono::microseconds>(ExePreEnd - ExePreStart).count() / 1000.0;
	std::cout << "Exe Pre time = " << ExePreT << endl;
	if (pBaseParam->m_AIBuildToolType == dpGlassClassifierTool)
	{
		doInferenceGlassClassification(*context, stream, m_pData, m_pBuffers, m_pProbFloat, nCountImg, pBaseParam->m_AICommonParam.nInModelInputW, pBaseParam->m_AICommonParam.nInModelInputH, max(0, pBaseParam->m_AICommonParam.nInClassNum), m_bIsDdInput);
	}


	return TRUE;
}
bool AIGlassClassifierTool::GetResult(tagAIBaseParam*pBaseParam, tagAIBaseResultParam* pBaseResultParam)
{
	if (pBaseResultParam == NULL || pBaseParam == NULL)
	{
		return false;
	}
	if (pBaseParam->m_AIBuildToolType != GetInspectToolType())
	{
		return false;
	}

	m_pAIClassifierResult = (tagAIClassifierResult*)pBaseResultParam;

	myClassifierResult myclassiferResultTemp;
	memset(&myclassiferResultTemp, 0, sizeof(myClassifierResult));
	m_pAIClassifierResult->m_vecClassifierResult.clear();
	for (int i = 0; i < pBaseResultParam->m_nBatchCur; i++)
	{
		int nClassIndex = 0;
		float fProb = 0.0;
		for (int j = 0; j < pBaseParam->m_AICommonParam.nInClassNum; j++)
		{
			//cout << "概率"<<m_pProb[j]<<endl;
			if (*(m_pProbFloat + i*pBaseParam->m_AICommonParam.nInClassNum + j) >= fProb)
			{
				nClassIndex = j;
				fProb = *(m_pProbFloat + i*pBaseParam->m_AICommonParam.nInClassNum + j);
				//myClassiferResult.nClassNo = nClassIndex;
				//myClassiferResult.fProbabity = fProb;
			}
		}
		myclassiferResultTemp.nClassNo = nClassIndex;
		myclassiferResultTemp.fProbabity = fProb;

		m_pAIClassifierResult->m_vecClassifierResult.push_back(myclassiferResultTemp);
	}
	return TRUE;
}

unsigned char AIGlassClassifierTool::GetScaleVal(Img &img, float fRawX, float fRawY)
{
	int nIVal = fRawX;
	int nJVal = fRawY;
	float fUVal = fRawX - nIVal;
	float fVVal = fRawY - nJVal;
	if (nJVal + 1 < img.nWidth && nIVal + 1 < img.nHeight)
	{
		unsigned char ucXY = img.pImgData[nIVal*img.nWidth + nJVal];
		unsigned char ucXY1 = img.pImgData[nIVal*img.nWidth + nJVal + 1];
		unsigned char ucX1Y = img.pImgData[(nIVal + 1)*img.nWidth + nJVal];
		unsigned char ucX1Y1 = img.pImgData[(nIVal + 1)*img.nWidth + nJVal + 1];
		return ((1 - fUVal)*(1 - fVVal)*ucXY + (1 - fUVal)*fVVal*ucXY1 + fUVal*(1 - fVVal)*ucX1Y + fUVal*fVVal*ucX1Y1);
	}
	else
	{
		return 120;
	}
}