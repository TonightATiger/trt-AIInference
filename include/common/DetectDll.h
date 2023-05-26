// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� DETECTDLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// DETECTDLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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