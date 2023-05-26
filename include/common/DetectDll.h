// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 DETECTDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// DETECTDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef DETECTDLL_EXPORTS
#define DETECTDLL_API __declspec(dllexport)
#else
#define DETECTDLL_API __declspec(dllimport)
#endif
#include <vector>
using namespace std;

struct DetBox
{
	float fBBox[4];
	float fConfScore;
	float fClassID;
};
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

class DETECTDLL_API CDetectDll
{
public:
	CDetectDll();
	~CDetectDll();
public:
	int LoadModel(char* pModelName, char* pInputBlobName = "data", char* pOutPutBlobName = "prob");
	int SetModelParam(int nBatch = 16, int nDevice = 0,
		int nInputModelW = 512, int nInputModelH = 512,
		int nClassNums = 1, int nMaxOutPutBoxCounts = 1000);
	//int Detect(vector<Img> &vecImg,float fNMSThresh = 0.4, float fConfThresh = 0.3);
	//int Detect(vector<ImgRGB> &vecImg, float fNMSThresh = 0.4, float fConfThresh = 0.3);
	int Detect(vector<ImgRGB> &vecImg, vector<vector<DetBox>> &vecDetBoxRes);

	void SetConfThreshold(float fConfThresh = 0.3);
	void SetNmsThreshold(float fNMSThresh = 0.4);
private:
	bool m_bSetParm;
	void* m_pBuffers[2];//2->5
	float *m_pData;
	float *m_pProb;
	int m_nBatch;
public:
	vector<vector<DetBox>> m_vecDetBox;
public:
	int m_nInputModelW;
	int m_nInputModelH;
	int m_nClassNums;
	int m_nMaxOutPutBoxCounts;
	int m_nOutputSize;
	char m_chInputBlobName[500];
	char m_chOutPutBlobName[500];
	float m_fConfThresh;
	float m_fNMSThresh;

private:
	void* m_pEngine;
	void* m_pRuntime;
	void* m_pContext;
	void* m_pStream;
	void* m_pLogger;
private:
	int Release();
	/*
	cudaStream_t stream;
	ICudaEngine* engine;
	IRuntime* runtime;
	IExecutionContext* context;
	*/
};