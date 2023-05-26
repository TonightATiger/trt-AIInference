// TestDPLearnigDemo.cpp : 定义控制台应用程序的入口点。
//
//注意：分类任务可支持单通道和彩色图，检测和分割必须输入的为RGB

#include "stdafx.h"
#include "DeepLearningTool.h"
#include <vector>
#include "opencv2/opencv.hpp"
#include <chrono>
#include "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.1\include\nvml.h"
using namespace cv;
using namespace std;

#ifdef _DEBUG
#pragma comment(lib,"DeepLearningTool15_x64ud.lib")
#else
#pragma comment(lib,"DeepLearningTool15_x64u.lib")
#endif
#pragma comment(lib,"D:\\hwl\\DeepLearnintV1.0\\DPLearningV1.0\\lib\\nvml.lib")

int main()
{
	/**************分割测试************/
	//分割测试
	//DeepLearningTool *pDPTool=new DeepLearningTool();
	//tagAIBaseParam *ptagAIBaseParam = NULL;
	//tagAIBaseResultParam *ptagAIBaseResultParam = NULL;

	//pDPTool->CreateTool(dpSegmentionTool);
	//if (!pDPTool->Init(ptagAIBaseParam))
	//{
	//	cout << "初始化失败，检查配置文件" << endl;
	//	return false;
	//}
	//pDPTool->SetParam(ptagAIBaseParam);
	//vector<DEF>vecDefImg;
	//vector<REF>vecRefImg;
	//DEF myDef,myDef1;
	//REF myRef,myRef1;
	//Mat cvImg,cvImg1,cvImg2,cvImg3;
	//std::vector<Mat> channels, channels1,channels2,channels3;
	//while (1)
	//{
	//	vecDefImg.clear();
	//	vecRefImg.clear();

	//	cvImg = cv::imread("D:\\hwl\\DeepLearnintV1.0\\test\\ttt.bmp", 1);
	//	split(cvImg, channels);
	//	myDef.m_ImgRGB.nWidth = cvImg.cols;
	//	myDef.m_ImgRGB.nHeight = cvImg.rows;
	//	myDef.m_ImgRGB.nChannles = cvImg.channels();
	//	myDef.m_ImgRGB.pRGBData[0] = channels[0].data;
	//	myDef.m_ImgRGB.pRGBData[1] = channels[1].data;
	//	myDef.m_ImgRGB.pRGBData[2] = channels[2].data;
	//	vecDefImg.push_back(myDef);

	//	auto start = std::chrono::system_clock::now();
	//	pDPTool->PreProcess(ptagAIBaseParam, ptagAIBaseResultParam, vecDefImg, vecRefImg);
	//	auto preEnd = std::chrono::system_clock::now();
	//	auto preT = std::chrono::duration_cast<std::chrono::microseconds>(preEnd - start).count() / 1000.0;
	//	pDPTool->Execute(ptagAIBaseParam, ptagAIBaseResultParam);
	//	auto ExeEnd = std::chrono::system_clock::now();
	//	auto ExeT = std::chrono::duration_cast<std::chrono::microseconds>(ExeEnd - preEnd).count() / 1000.0;
	//	pDPTool->GetResult(ptagAIBaseParam, ptagAIBaseResultParam);
	//	tagAISegRusult *pSegRes = (tagAISegRusult*)ptagAIBaseResultParam;
	//	auto GetREnd = std::chrono::system_clock::now();
	//	auto GetResT = std::chrono::duration_cast<std::chrono::microseconds>(GetREnd - ExeEnd).count() / 1000.0;
	//	std::cout << "Pre time = " << preT << "   ,Exe time = " << ExeT << "   ,Get Result time = " << GetResT << endl;
	//	/////std::cout << "inference time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us" << std::endl;

	//	if (pSegRes == NULL)
	//	{
	//		return false;
	//	}
	//	int nW = pSegRes->m_nImgWCur;
	//	int nH = pSegRes->m_nImgHCur;

	//	cv::Mat mImg(nH, nW, CV_8UC1, pSegRes->m_vecImg[0].pImgData);
	//	cv::imwrite("D:\\model\\test.bmp", mImg);

	//	/*cv::Mat mImg1(nW, nH, CV_8UC1, pSegRes->m_vecImg[1].pImgData);
	/*	cv::imwrite("D:\\model\\test1.bmp", mImg1);
	}*/
	/*	std::cout << "获取分类结果" << "CLass" << pClasRes->m_vecClassifierResult[0].nClassNo << "概率" << pClasRes->m_vecClassifierResult[0].fProbabity << endl;
		std::cout << "获取检测结果" << "CLass" << pDetectRes->m_vecDetBox[0][0].fClassID << endl;*/
	/////////////////分割结束/////////////////////////////////////
	
	//------------------------------------------------------------------------------------------------------
	//1-1原片分类测试
	//-------------------------------------------------------------------------------------------------------//

	//while (true)
	//{
	//	DeepLearningTool *pDPTool = new DeepLearningTool();
	//	tagAIBaseParam *ptagAIBaseParam = NULL;
	//	tagAIBaseResultParam *ptagAIBaseResultParam = NULL;
	//	pDPTool->CreateTool(dpGlassClassifierTool);
	//	//pDPTool->CreateTool(dpClassifierTool);
	//	//pDPTool->CreateTool(dpClassifierTool);
	//	if (!pDPTool->Init(ptagAIBaseParam))
	//	{
	//		return false;

	//	}

	//	pDPTool->SetParam(ptagAIBaseParam);

	//	vector<DEF>vecDefImg;
	//	vector<REF>vecRefImg;
	//	DEF myDef, myDef1;
	//	Mat cvImg, cvImg1;
	//	//while(1)
	//	{
	//		vecDefImg.clear();
	//		vecRefImg.clear();

	//		//cvImg = cv::imread("D:\\model\\Yichang.bmp",1);
	//		cvImg = cv::imread("D:\\Error\\OtherclassWenchongying1\\1654108_00018_1_001_4_3_0_020159494_T11_P[14][0.63996494].bmp", 1);
	//		vector<cv::Mat>nChannels;
	//		cv::split(cvImg, nChannels);
	//		myDef.m_ImgRGB.nWidth = cvImg.cols;
	//		myDef.m_ImgRGB.nHeight = cvImg.rows;
	//		myDef.m_ImgRGB.pRGBData[0] = nChannels.at(2).data;
	//		myDef.m_ImgRGB.pRGBData[1] = nChannels.at(1).data;
	//		myDef.m_ImgRGB.pRGBData[2] = nChannels.at(0).data;
	//		vecDefImg.push_back(myDef);

	/*auto start = std::chrono::system_clock::now();
	pDPTool->PreProcess(ptagAIBaseParam, ptagAIBaseResultParam, vecDefImg, vecRefImg);
	pDPTool->Execute(ptagAIBaseParam, ptagAIBaseResultParam);
	pDPTool->GetResult(ptagAIBaseParam, ptagAIBaseResultParam);
*/
	//		tagAIClassifierResult *pClasRes = (tagAIClassifierResult*)ptagAIBaseResultParam;
	//		auto end = std::chrono::system_clock::now();
	//		std::cout << "inference time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us" << std::endl;
	//		std::cout << "获取分类结果" << "CLass" << pClasRes->m_vecClassifierResult[0].nClassNo << "概率" << pClasRes->m_vecClassifierResult[0].fProbabity << endl;
	//	}

	//	delete pDPTool;
	//}
	

    //return 0;

	//絲印背板分类----------------------------
	
	//DeepLearningTool *pDPTool = new DeepLearningTool();
	//tagAIBaseParam *ptagAIBaseParam = NULL;
	//tagAIBaseResultParam *ptagAIBaseResultParam = NULL;
	//pDPTool->CreateTool(dpSilkGlassClassifierTool);
	////pDPTool->CreateTool(dpClassifierTool);
	////pDPTool->CreateTool(dpClassifierTool);
	//if (!pDPTool->Init(ptagAIBaseParam))
	//{
	//	return false;

	//}

	//pDPTool->SetParam(ptagAIBaseParam);

	//vector<DEF>vecDefImg;
	//vector<REF>vecRefImg;
	//DEF myDef, myDef1;
	//Mat cvImg, cvImg1;
	//while (1)
	//{
	//	vecDefImg.clear();
	//	vecRefImg.clear();

	//	cvImg = cv::imread("D:\\Error\\3JieShi&YM\\5_(x989y2640)6_T3_P[0][0.44888064].bmp", 0);
	//	//cvImg = cv::imread("D:\\model\\testG.bmp", 1);
	//	/*vector<cv::Mat>nChannels;
	//	cv::split(cvImg, nChannels);
	//	myDef.m_ImgRGB.nWidth = cvImg.cols;
	//	myDef.m_ImgRGB.nHeight = cvImg.rows;
	//	myDef.m_ImgRGB.pRGBData[0] = nChannels.at(2).data;
	//	myDef.m_ImgRGB.pRGBData[1] = nChannels.at(1).data;
	//	myDef.m_ImgRGB.pRGBData[2] = nChannels.at(0).data;
	//	vecDefImg.push_back(myDef);*/

	//	myDef.m_Img.nChannels = 1;
	//	myDef.m_Img.nWidth= cvImg.cols;
	//	myDef.m_Img.nHeight = cvImg.rows;
	//	myDef.m_Img.pImgData = cvImg.data;
	//	vecDefImg.push_back(myDef);


	//	auto start = std::chrono::system_clock::now();
	//	pDPTool->PreProcess(ptagAIBaseParam, ptagAIBaseResultParam, vecDefImg, vecRefImg);
	//	pDPTool->Execute(ptagAIBaseParam, ptagAIBaseResultParam);
	//	pDPTool->GetResult(ptagAIBaseParam, ptagAIBaseResultParam);

	//	tagAIClassifierResult *pClasRes = (tagAIClassifierResult*)ptagAIBaseResultParam;
	//	auto end = std::chrono::system_clock::now();
	//	std::cout << "inference time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us" << std::endl;
	//	std::cout << "获取分类结果" << "CLass" << pClasRes->m_vecClassifierResult[0].nClassNo << "概率" << pClasRes->m_vecClassifierResult[0].fProbabity << endl;
	//}
////////////////////////////////////////////////////////////////////////////////////////////////

//DeepLearningTool *pDPTool=new DeepLearningTool();
//tagAIBaseParam *ptagAIBaseParam = NULL;
//tagAIBaseResultParam *ptagAIBaseResultParam = NULL;

//pDPTool->CreateTool(dpSegmentionTool);


	//DeepLearningTool *pDPTool=new DeepLearningTool();
	//tagAIBaseParam *ptagAIBaseParam = NULL;
	//tagAIBaseResultParam *ptagAIBaseResultParam = NULL;
	////pDPTool->CreateTool(AIBuildToolType::dpClassifierTool);

	//pDPTool->CreateTool(dpClassifierTool);
	////pDPTool->CreateTool(dpClassifierTool);
	//if (!pDPTool->Init(ptagAIBaseParam))
	//{
	//	return false;

	//}

	//pDPTool->SetParam(ptagAIBaseParam);

	//vector<DEF>vecDefImg;
	//vector<REF>vecRefImg;
	//DEF myDef, myDef1;
	//Mat cvImg, cvImg1;
	//while (1)
	//{
	//	vecDefImg.clear();
	//	vecRefImg.clear();

	//	//cvImg = cv::imread("D:\\image\\w360h360.bmp", 1);
	//	cvImg = cv::imread("D:\\hwl\\DeepLearnintV1.0\\DPLearningV1.0\\x64\\Debug\\00-08-39-13_4.bmp", 0);
	//	//cvImg = cv::imread("D:\\model\\testG.bmp", 1);
	//	/*vector<cv::Mat>nChannels;
	//	cv::split(cvImg, nChannels);
	//	myDef.m_ImgRGB.nWidth = cvImg.cols;
	//	myDef.m_ImgRGB.nHeight = cvImg.rows;
	//	myDef.m_ImgRGB.pRGBData[0] = nChannels.at(2).data;
	//	myDef.m_ImgRGB.pRGBData[1] = nChannels.at(1).data;
	//	myDef.m_ImgRGB.pRGBData[2] = nChannels.at(0).data;
	//	vecDefImg.push_back(myDef);*/

	//	myDef.m_Img.nChannels = 3;
	//	myDef.m_Img.nWidth= cvImg.cols;
	//	myDef.m_Img.nHeight = cvImg.rows;
	//	myDef.m_Img.pImgData = cvImg.data;
	//	vecDefImg.push_back(myDef);

	//	int nNo = 1;
	//	double fMeanCost = 0.0f;
	//	for (int i = 0; i < nNo; i++)
	//	{

	//		auto start = std::chrono::system_clock::now();

	//		for (int nI = 0; nI < 1; nI++)
	//		{
	//			//auto startPre = std::chrono::system_clock::now();
	//			pDPTool->PreProcess(ptagAIBaseParam, ptagAIBaseResultParam, vecDefImg, vecRefImg);
	//			//auto endPre = std::chrono::system_clock::now();
	//			pDPTool->Execute(ptagAIBaseParam, ptagAIBaseResultParam);
	//			pDPTool->GetResult(ptagAIBaseParam, ptagAIBaseResultParam);


	//		}
	//		tagAIClassifierResult *pClasRes = (tagAIClassifierResult*)ptagAIBaseResultParam;
	//		auto end = std::chrono::system_clock::now();
			//fMeanCost += (float)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			//string strDebug;
			//strDebug.Format(L"Total inference time:%d us,Pre:%d us,class:%d,probability:%.4f", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(), std::chrono::duration_cast<std::chrono::microseconds>(endPre - startPre).count(), pClasRes->m_vecClassifierResult[0].nClassNo, pClasRes->m_vecClassifierResult[0].fProbabity);
			//strDebug=""
			//	(L"Total inference time:%d us,class:%d,probability:%.4f", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(), pClasRes->m_vecClassifierResult[0].nClassNo, pClasRes->m_vecClassifierResult[0].fProbabity);
			//cout << strDebug << endl;
			//cout << "Total inference time:" << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"<<endl;

	//	}
	//
	//
	//
	//}

		//DCP分类----------------------------

		DeepLearningTool *pDPTool = new DeepLearningTool();
		tagAIBaseParam *ptagAIBaseParam = NULL;
		tagAIBaseResultParam *ptagAIBaseResultParam = NULL;
		pDPTool->CreateTool(dpDCPClassifierTool);
		//pDPTool->CreateTool(dpClassifierTool);
		//pDPTool->CreateTool(dpClassifierTool);
		if (!pDPTool->Init(ptagAIBaseParam))
		{
			return false;

		}

		pDPTool->SetParam(ptagAIBaseParam);

		vector<DEF>vecDefImg;
		vector<REF>vecRefImg;
		DEF myDef, myDef1;
		Mat cvImg, cvImg1;
		while (1)
		{
			vecDefImg.clear();
			vecRefImg.clear();

			cvImg = cv::imread("D:\\test.bmp", 0);
			//cvImg = cv::imread("D:\\model\\testG.bmp", 1);
			/*vector<cv::Mat>nChannels;
			cv::split(cvImg, nChannels);
			myDef.m_ImgRGB.nWidth = cvImg.cols;
			myDef.m_ImgRGB.nHeight = cvImg.rows;
			myDef.m_ImgRGB.pRGBData[0] = nChannels.at(2).data;
			myDef.m_ImgRGB.pRGBData[1] = nChannels.at(1).data;
			myDef.m_ImgRGB.pRGBData[2] = nChannels.at(0).data;
			vecDefImg.push_back(myDef);*/

			myDef.m_Img.nChannels = 1;
			myDef.m_Img.nWidth= cvImg.cols;
			myDef.m_Img.nHeight = cvImg.rows;
			myDef.m_Img.pImgData = cvImg.data;
			vecDefImg.push_back(myDef);


			auto start = std::chrono::system_clock::now();
			pDPTool->PreProcess(ptagAIBaseParam, ptagAIBaseResultParam, vecDefImg, vecRefImg);
			pDPTool->Execute(ptagAIBaseParam, ptagAIBaseResultParam);
			pDPTool->GetResult(ptagAIBaseParam, ptagAIBaseResultParam);

			tagAIClassifierResult *pClasRes = (tagAIClassifierResult*)ptagAIBaseResultParam;
			auto end = std::chrono::system_clock::now();
			std::cout << "inference time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "us" << std::endl;
			std::cout << "获取分类结果" << "CLass" << pClasRes->m_vecClassifierResult[0].nClassNo << "概率" << pClasRes->m_vecClassifierResult[0].fProbabity << endl;
		}
	return 0;
	
}

