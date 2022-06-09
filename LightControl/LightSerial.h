#pragma once
#include "DHInterFace.h"
#include "SerialPortExt.h"
#include "LightControl.in.h"
#include "TcpComm.h"
void T_LightConnect();
class CLightSerial : DHInterface
{
public:
	typedef struct tagTParam
	{
		eLightCommType  eCommType;   //ͨѶ����
		eLightControlType	eLightType; //��Դ����������
		CString				sComName;   //���ں�
		int					nComIndex;  //���������±�
		CString sIP;						//IP��
		int nTcpPort;                    //����˿�

		BOOL bUseLightAdjust;//�Ƿ����ù�Դ����Ӧ
		int  nSampleFrames;//�ջ�ȡ��֡��
		int  nGoalGray;//Ŀ��Ҷ�
		int  nGraySafeDev;//���Բ����й�Դ����Ӧ�ķ�Χ
		int  nLightUp;//��Դ��������
		int  nLightDown;//��Դ��������
	};
public:
	CLightSerial(void);
	~CLightSerial(void);
	virtual int DoSomething(void* message,void* wParam,void* lParam);

	int Init(LIGHTINIT &Init);

	BOOL IsOpen() { return m_SerialPort.IsOpen(); }
	BOOL Open(); //�򿪹�Դ����������
	BOOL Close(); //�رչ�Դ����������
	void OpenLight();//������Դ
	void CloseLight();//�رչ�Դ
	BOOL AdaptLightBright(int nGrayValue, int nChannelIndex);
	BOOL SetBrightness(const int& nChannel, const int& nBrightness);//���ù�Դ����������
	int  GetBrightnes(const int& nChannel);//��ȡ��Դ������������
	BOOL Log(const CString& strLog,  int nLevel = 0);
	BOOL SaveParams();//��������ӿ�
	BOOL ReadParams();//��ȡ�����ӿ�

	CLightSerial::tagTParam& GetLightParam() {return m_tLightParam; }
	
private:
	BOOL CheckChannel(const int& nChannel);
private:
	BOOL ShowParamDlg(BOOL bEnable);
	LIGHTINIT m_sInit;
	CString m_strConfigLightSerialFile;
	CString m_strConfigLightSerialFilePath;
	tagTParam m_tLightParam;
	CSerialPortExt m_SerialPort;	
	CTcpComm m_TcpComm;

public:
	int	 m_nCurrentStatus;//��ǰ״̬
	CString CurLightType;
	CString m_strCurStatus;//��ǰ״̬
	HTuple m_hvRealGrayValue[2];//ͼ��ʵʱ�Ҷ�ֵ
};

extern CLightSerial* g_This;

