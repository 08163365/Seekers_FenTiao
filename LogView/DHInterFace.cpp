#include "stdafx.h"
#include "Log.h"
#include "DHInterFace.h"

// ͨ������������ʽ����÷��ṩָ�����������Ļ���ָ��
extern "C" __declspec(dllexport) DHInterface* Export(void)
{
	return (DHInterface*)new CLog();
}