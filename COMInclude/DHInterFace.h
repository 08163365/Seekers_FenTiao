#pragma once
class DHInterface
{
public:
	virtual int DoSomething(void* message,void* wParam,void* lParam) = 0; // �����÷���Ҫ���õĳ�Ա���������ɴ��麯��
	virtual ~DHInterface(){};// �����������������
};
