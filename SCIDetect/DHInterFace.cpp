#include "stdafx.h"
#include "Battery.h"
#include "DHInterFace.h"

// ͨ������������ʽ����÷��ṩָ�����������Ļ���ָ��
extern "C" __declspec(dllexport) DHInterface* Export(void)
{
	return (DHInterface*)new CBattery();
}