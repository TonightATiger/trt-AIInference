// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� SEGMENTIONDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// SEGMENTIONDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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
	char  name[256];//�Կ�����
	unsigned int nDeviceNum;//�Կ�����
	unsigned int nGPUutilization;//GPU������,���أ���5����Ϊ5%��
	unsigned int nCudaMemutilization;//�Դ�������,���أ���5����Ϊ5%��
	unsigned int nDeviceTemperature;//��ǰ�Կ��豸�¶�

	unsigned int nTotalGPUMemory;//�豸�ܹ��Դ�
	unsigned int nUsedGpuMemory;//�豸ʹ���Դ�
	unsigned int nFreeGpuMemory;//�豸�豸
};

typedef enum DPReturnModule
{
	DPMODULE_SUCCESS=0, //!< The operation was successful
	DPMODULE_ERROR_NOT_FINDGPU=1,//!< not find available device
	DPMODULE_ERROR_NOT_FINDMODEL=2,//!< not find available device



}DPReturnModStates;

// �����Ǵ� SegmentionDll.dll ������
class SEGMENTIONDLL_API CSegmentionDll {
public:
	CSegmentionDll();
	// TODO:  �ڴ�������ķ�����
	~CSegmentionDll();
public:
	int LoadModel(char* pModelName, char* pInputBlobName = "input", char* pOutPutBlobName = "output");
	int SetModelParam(int nMaxBatch = 16, int nDevice = 0,
		int nMaxInputImgSizeW = 1024, int nMaxInputSizeH = 1024, int nClassNums = 1);

	int SegmentInference(vector<ImgDbRGB>vecDbRGB, vector<Img>&vecImg);// by ls 2022.5.27 ��ʵ��

	int SegmentInference(ImgDbRGB &imgDbRGB, Img & img);//˫���� ����batch //20220603 ����Ӧ

	int GetDevicePropInfos(cudaDevicePropCuston &devicePropTmp);//��ȡ�Կ����������ơ�gpu�����ʡ��Դ�ʹ���ʡ��¶ȵ� by ls 20220620
	//void SetConfThreshold(float fConfThresh = 0.3);
private:
	bool m_bSetParm;
	void* m_pBuffers[2];
	float *m_pData;
	int *m_pProb;
	int m_nMaxBatch;//���batch �����Դ�ʹ��
	int m_nBatchCur;//ʵ��ʹ�õ�batch
	unsigned char*m_pPropMap;//by ls 0527
							 //unsigned char*m_pPropMapArr[16];

	unsigned char* img_host;//by ls 0610 ɾ��host������δ�õ�
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


