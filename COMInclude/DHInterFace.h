#pragma once
class DHInterface
{
public:
	virtual int DoSomething(void* message,void* wParam,void* lParam) = 0; // 将调用方需要调用的成员函数声明成纯虚函数
	virtual ~DHInterface(){};// 抽象类的虚析构函数
};
