#pragma once
#include "stdafx.h"

namespace HDevExportCpp
{
	 using namespace HalconCpp;
	 void* _hcppthread_processing_thread(void *hcthread);//处理线程
	 void* _hcppthread_pintu_thread(void *hcthread);//拼图线程

	 
	 void* _hcppthread_allocator_thread(void *hcthread);//分配者线程
	 void* _hcppthread_Listen_thread(void *hcthread);//监听线程

	 void WINAPI _hcppthread_acquisition_thread(const s_GBSIGNALINFO* SigInfo);

	 void Pintu_thread(HTuple hv_ThreadIndex,HTuple* hv_CurMessage);
	 BOOL DealData(C_ALLOCATORWORKSHOPOUTPUT &Output,int & nSide,CString & StrID);//对数据进行平滑处理

	 void processing_thread (HTuple hv_ThreadIndex,HTuple* hv_CurMessage);
	 void allocator_thread(HTuple hv_GrabberSN);//分配者线程
	 void Listen_thread(HTuple hv_GrabberSN);
	 
	 void JudageMarkExist(C_ALLOCATORWORKSHOPOUTPUT& C_OutputInfo,int& nSide);
	 void acquisition_thread(const s_GBSIGNALINFO* SigInfo,HTuple hv_QueueImage);
	 int Jigsaw(A_GRABWORKSHOPOUTPUT &Output,const int& nGrabberSN,const int & nFrameSN,HObject& hoGrabImage,HObject* hoCompleteImage,HObject* hoRemainImage);
	 void WriteExceptImage(HObject &ho_ExceptionImage,HTuple hv_GrabberSN);
	 BOOL AlignCameraImages( HObject& ho_GrabImage,HTuple&  hv_GrabberSN,HTuple& hv_Encoder);//使正反相机采集的图片能够对上
	 BOOL SaveJigsawImage(const int& nGrabberSN,const HObject& ho_ImageSave,int nImageSN);
	 double GetEncoder(const int& nGrabberSN);
	 int    GetFrameSN(const int& nGrabberSN);
	 void   CopyMessage(C_ALLOCATORWORKSHOPOUTPUT & tSourceOutPut,C_ALLOCATORWORKSHOPOUTPUT &tDestOutPut);
	 void  TempLog(int& nGrabberSN,A_GRABWORKSHOPOUTPUT& tOutPut);//临时日志
	 void  WriteCropRowInfo(int nGrabberSN,CString& strInfo);//记录裁切信息
	 BOOL  ISZhiDai(C_ALLOCATORWORKSHOPOUTPUT & tOutPut);//返回是否是纸带

}

