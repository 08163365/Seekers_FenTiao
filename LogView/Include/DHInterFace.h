#pragma once
//typedef enum _SeekersAlgorithm//��̬������
//{
//	//5����������
//	INITIALIZE,//��ʼ��
//	ACTION,//�������
//	SHOW,//��ʾ�����Ի���
//	SETPARAM,//���ò���
//	GETPARAM,//��ò���
//};
class DHInterface
{

public:

	virtual int DoSomething(void* message,void* wParam,void* lParam) = 0;

	virtual ~DHInterface(){};// �����������������
};
