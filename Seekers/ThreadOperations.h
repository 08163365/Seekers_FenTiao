#pragma once
#include "stdafx.h"

namespace HDevExportCpp
{
	 using namespace HalconCpp;
	 void* _hcppthread_processing_thread(void *hcthread);//�����߳�
	 void* _hcppthread_pintu_thread(void *hcthread);//ƴͼ�߳�

	 
	 void* _hcppthread_allocator_thread(void *hcthread);//�������߳�
	 void* _hcppthread_Listen_thread(void *hcthread);//�����߳�

	 void WINAPI _hcppthread_acquisition_thread(const s_GBSIGNALINFO* SigInfo);

	 void Pintu_thread(HTuple hv_ThreadIndex,HTuple* hv_CurMessage);
	 BOOL DealData(C_ALLOCATORWORKSHOPOUTPUT &Output,int & nSide,CString & StrID);//�����ݽ���ƽ������

	 void processing_thread (HTuple hv_ThreadIndex,HTuple* hv_CurMessage);
	 void allocator_thread(HTuple hv_GrabberSN);//�������߳�
	 void Listen_thread(HTuple hv_GrabberSN);
	 
	 void JudageMarkExist(C_ALLOCATORWORKSHOPOUTPUT& C_OutputInfo,int& nSide);
	 void acquisition_thread(const s_GBSIGNALINFO* SigInfo,HTuple hv_QueueImage);
	 int Jigsaw(A_GRABWORKSHOPOUTPUT &Output,const int& nGrabberSN,const int & nFrameSN,HObject& hoGrabImage,HObject* hoCompleteImage,HObject* hoRemainImage);
	 void WriteExceptImage(HObject &ho_ExceptionImage,HTuple hv_GrabberSN);
	 BOOL AlignCameraImages( HObject& ho_GrabImage,HTuple&  hv_GrabberSN,HTuple& hv_Encoder);//ʹ��������ɼ���ͼƬ�ܹ�����
	 BOOL SaveJigsawImage(const int& nGrabberSN,const HObject& ho_ImageSave,int nImageSN);
	 double GetEncoder(const int& nGrabberSN);
	 int    GetFrameSN(const int& nGrabberSN);
	 void   CopyMessage(C_ALLOCATORWORKSHOPOUTPUT & tSourceOutPut,C_ALLOCATORWORKSHOPOUTPUT &tDestOutPut);
	 void  TempLog(int& nGrabberSN,A_GRABWORKSHOPOUTPUT& tOutPut);//��ʱ��־
	 void  WriteCropRowInfo(int nGrabberSN,CString& strInfo);//��¼������Ϣ
	 BOOL  ISZhiDai(C_ALLOCATORWORKSHOPOUTPUT & tOutPut);//�����Ƿ���ֽ��

}

