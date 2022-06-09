#pragma once

/********************************************************************
    ("INITIALIZE" == strMessage)//初始化
	("ACTION" == strMessage)//检测
	("SHOW" == strMessage)//调用对话框
	和Seekers系统共用的头文件，作用是声明结构体变量
/***********************************************************************/
typedef struct tagTDETECTINIT//SCIDetect算法动态库初始化结构体
{
	CString strProductName;//品种名称
	int     nGrabberSN;//相机编号 0表示前面相机，1表示后面相机
	PVOID   pLogView;
	int     nProductType;//产品类型
	int     nCoatType;//涂布类型

}DETECTINIT,*PDETECTINIT;
typedef struct tagTDETECTINPUT//SCIDetect算法动态库输入结构体
{
	int     nBreakType;
	HObject hoDetectImage;//采集到的图片
}DETECTINPUT,*PDETECTINPUT;
typedef struct tagTDETECTOUTPUT//SCIDetect算法动态库输出结构体
{
	HTuple  hv_Flag;//个数等于条数 TRUE为ok FALSE是NG 2是不检测 每片结果
	HTuple  hv_ErrorType;//错误类型         5
	HTuple  hv_ErrorLevel;//错误等级        0
	HTuple  hv_ErrorBelongStrip;//隶属于哪一条，从0开始

	HTuple  hv_CoatWidth; //涂布宽  
	HTuple  hv_CoatHeight;//涂布高


	HTuple  hv_FoilWidth;    //箔材宽
	HTuple  hv_FoilHeight;   //箔材高
	HTuple  hv_LocateMissing;//检测定位不齐值
	HTuple  hv_BaiBian;//正极存在的陶瓷边
	HTuple  hv_JuanRaoMarkRow;//Mark点的行坐标 如果是-1 代表没有Mark点 2017/7/1

	HTuple  hv_SortWidth;//排序后显示的宽度顺序
	HTuple  hv_Name;//依据宽度排序后的名字
	HTuple  hv_AT9EdageToProEdage;//AT9错位尺寸
	HTuple  hv_CeramicWidth;//AT9尺寸
	HTuple hv_CpkEdageToProEdage;	//连续涂布CPK计算,记录每条涂布的相邻边


	//间歇涂布
	HTuple  hv_LeftEadgePixel;      //左侧像素
	HTuple  hv_RightEadgePixel;     //右侧像素
	HTuple  hv_TopEdagePixel;       //顶部像素
	HTuple  hv_BotEdagePixel;       //底部像素
	HTuple  hv_HeadCuoWei;          //头部错位
	HTuple  hv_TailCuoWei;          //尾部错位

	HTuple  hv_TripSNTuple;         //槽位隶属于条数
	HTuple  hv_CaoWeiWidthTuple;    //槽位宽度
	HTuple  hv_CaoWeiHeightTuple;   //槽位高度
	HTuple  hv_CaoWeiXPositionTuple;//槽位X坐标
	HTuple  hv_CaoWeiYpositionTuple;//槽位Y坐标
	
	HTuple  hv_JXHeight;            //间歇高度
	HTuple  hv_MPLength;            //膜长
	HTuple  hv_SumLength;           //总长
	HObject ho_JXRegion;            //间歇区域
	HObject ho_ExtractCaoWeiRegions;//提取的槽位区域

	//HTuple  hv_Reserve1;//预留字段1   不含间歇总膜长
	//HTuple  hv_Reserve2;//预留字段2   间歇高度 
	//HTuple  hv_Reserve3;//预留字段3   首极耳间距
	//HTuple  hv_Reserve4;//预留字段4   极耳数目
	//HTuple  hv_Reserve5;//预留字段5   上圆弧高度
	//HTuple  hv_Reserve6;//预留字段6   下圆弧高度
	//HTuple  hv_Reserve7;//预留字段6    凹漏箔高度




	HObject ho_ErrorRegion;//错误区域 个数   5
	HObject ho_Coat;//涂布
	HObject ho_Foil;//箔材
	void InitVariable()
	{
		TupleGenConst(2,TRUE,&hv_Flag);//默认1是OK 0是NG
		hv_ErrorType.Clear();
		hv_ErrorLevel.Clear();
		hv_ErrorBelongStrip.Clear();
		ho_ErrorRegion.GenEmptyObj();
		ho_ExtractCaoWeiRegions.GenEmptyObj();
		ho_JXRegion.GenEmptyObj();

	}
}DETECTOUTPUT,*PDETECTOUTPUT;

typedef struct tagTSPOTCHECKOUTPUT//点检算法输出结构体
{

	HTuple hv_LeftSpotArea;//左侧
	HTuple hv_LeftSpotWidth;//左侧
	HTuple hv_LeftSpotHeight;//左侧

	HTuple hv_RightSpotArea;//右侧点检面积
	HTuple hv_RightSpotWidth;//右侧点检面积
	HTuple hv_RightSpotHeight;//右侧点检面积

}SPOTCHECKOUTPUT,*PSPOTCHECKOUTPUT;