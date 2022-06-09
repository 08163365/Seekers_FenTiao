#pragma once
//typedef enum _SeekersAlgorithm//动态库名称
//{
//	//5个基本操作
//	INITIALIZE,//初始化
//	ACTION,//启动检测
//	SHOW,//显示制作对话框
//	SETPARAM,//设置参数
//	GETPARAM,//获得参数
//};
class DHInterface
{

public:

	virtual int DoSomething(void* message,void* wParam,void* lParam) = 0;

	virtual ~DHInterface(){};// 抽象类的虚析构函数
};
