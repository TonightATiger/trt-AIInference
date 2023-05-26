// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SEGMENTIONDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SEGMENTIONDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef SEGMENTIONDLL_EXPORTS
#define SEGMENTIONDLL_API __declspec(dllexport)
#else
#define SEGMENTIONDLL_API __declspec(dllimport)
#endif

#include <vector>
using namespace std;

struct Img
{
	int nWidth;
	int nHeight;
	int nChannel;
	unsigned char* pData;
};

struct ImgRGB
{
	int nWidth;
	int nHeight;
	int nChannel;
	unsigned char *pRData;
	unsigned char *pGData;
	unsigned char *pBData;
};

struct ImgDbRGB
{
	ImgDbRGB()
	{
		nChannel = 6;
	}
	int nWidth;
	int nHeight;
	int nChannel;
	unsigned char *pRGBData[6];
};

struct cudaDevicePropCuston
{
	char  name[256];//显卡名称
	unsigned int nDeviceNum;//显卡数量
	unsigned int nGPUutilization;//GPU利用率,返回：如5，意为5%；
	unsigned int nCudaMemutilization;//显存利用率,返回：如5，意为5%；
	unsigned int nDeviceTemperature;//当前显卡设备温度

	unsigned int nTotalGPUMemory;//设备总共显存
	unsigned int nUsedGpuMemory;//设备使用显存
	unsigned int nFreeGpuMemory;//设备设备
};

typedef enum DPReturnModule
{
	DPMODULE_SUCCESS=0, //!< The operation was successful
	DPMODULE_ERROR_NOT_FINDGPU=1,//!< not find available device
	DPMODULE_ERROR_NOT_FINDMODEL=2,//!< not find available device



}DPReturnModStates;

// 此类是从 SegmentionDll.dll 导出的
class SEGMENTIONDLL_API CSegmentionDll {
public:
	CSegmentionDll();
	// TODO:  在此添加您的方法。
	~CSegmentionDll();
public:
	int LoadModel(char* pModelName, char* pInputBlobName = "input", char* pOutPutBlobName = "output");
	int SetModelParam(int nMaxBatch = 16, int nDevice = 0,
		int nMaxInputImgSizeW = 1024, int nMaxInputSizeH = 1024, int nClassNums = 1);

	int SegmentInference(vector<ImgDbRGB>vecDbRGB, vector<Img>&vecImg);// by ls 2022.5.27 已实现

	int SegmentInference(ImgDbRGB &imgDbRGB, Img & img);//双输入 单个batch //20220603 可适应

	int GetDevicePropInfos(cudaDevicePropCuston &devicePropTmp);//获取显卡数量、名称、gpu利用率、显存使用率、温度等 by ls 20220620
	//void SetConfThreshold(float fConfThresh = 0.3);
private:
	bool m_bSetParm;
	void* m_pBuffers[2];
	float *m_pData;
	int *m_pProb;
	int m_nMaxBatch;//最大batch 分配显存使用
	int m_nBatchCur;//实际使用的batch
	unsigned char*m_pPropMap;//by ls 0527
							 //unsigned char*m_pPropMapArr[16];

	unsigned char* img_host;//by ls 0610 删除host，代码未用到
	unsigned char* img_device;
	Img m_imgTemp[8];
public:
	int m_nInputModelW;
	int m_nInputModelH;
	int m_nClassNums;
	int m_nMaxOutPutBoxCounts;
	int m_nOutputSize;
	char m_chInputBlobName[500];
	char m_chOutPutBlobName[500];
	float m_fConfThresh;
	bool m_bIsDynamicShape;

private:
	void* m_pEngine;
	void* m_pRuntime;
	void* m_pContext;
	void* m_pStream;
	void* m_pLogger;
private:
	int Release();
	int preprocess_img(vector<ImgDbRGB>&vecDbRGB, float* pfInputDevBuffer);
	int preprocess_imgSSE(vector<ImgDbRGB>&vecDbRGB, float* pfInputDevBuffer);

	int preprocess_img(ImgDbRGB&imgDbRGB, float* pfInputDevBuffer);
	int preprocess_imgSSE(ImgDbRGB&imgDbRGB, float* pfInputDevBuffer);
	int preprocess_imgAVX(ImgDbRGB&imgDbRGB, float* pfInputDevBuffer);

};


