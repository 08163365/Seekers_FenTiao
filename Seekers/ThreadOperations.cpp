#include "stdafx.h"
#include "ThreadOperations.h"
#include "SeekersDlg.h"
#include "FastMemcpy.h"
#include <iostream>
#include <fstream>
using namespace std;

BYTE* pBufferFirstImage = NULL;
BYTE* pBufferNextImage = NULL;
BYTE* pBufferTilePostion = NULL;

void GetImage(int nGrabberSN, HObject* hoGrabberImage);
bool RGBToGray(const char *srcRGB, char *dstR, char *dstG, char *dstB, int nWidth, int nHeight, const int nType);



namespace HDevExportCpp
{
	void* _hcppthread_pintu_thread(void *hcthread)//ƴͼ�߳�
	{
		HTuple hv_CurMessage;
		// +++ define thread context for this procedure
		HDevThread*  hcppthread = (HDevThread*)hcthread;
		try
		{
			const HTuple        &cbhv_ThreadIndex = hcppthread->GetInputCtrlParamTuple(0);
			Pintu_thread(cbhv_ThreadIndex, &hv_CurMessage);
			hcppthread->Exit();
			delete hcppthread;
		}
		catch (HException& e)
		{
			CString strException;
			strException.Format("Pintu_thread�����쳣\r\n%s  [ %s() ]", e.ErrorMessage().Text()
				, e.ProcName().Text());
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
			bool is_direct_call = hcppthread->IsDirectCall();
			// Attempt to clean up in error case, too
			hcppthread->Exit();
			delete hcppthread;
			// Propagate exception if called directly
			if (is_direct_call)
				throw e;
		}
		return NULL;
	}

	// Parallel execution wrapper for processing_thread(...) 
	void* _hcppthread_processing_thread(void *hcthread)
	{
		HTuple hv_CurMessage;
		// +++ define thread context for this procedure
		HDevThread*  hcppthread = (HDevThread*)hcthread;
		try
		{
			// Input parameters
			const HTuple        &cbhv_ThreadIndex = hcppthread->GetInputCtrlParamTuple(0);


			// Call processing_thread
			processing_thread(cbhv_ThreadIndex, &hv_CurMessage);

			// Reduce reference counter of thread object
			hcppthread->Exit();
			delete hcppthread;

		}
		catch (HException& e)
		{
			CString strException;
			strException.Format("processing_thread�����쳣\r\n%s  [ %s() ]", e.ErrorMessage().Text()
				, e.ProcName().Text());
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
			//�����쳣ͼƬ
			//��ȡA������Ϣ
			HObject ho_GrabImage;
			HTuple  hv_GrabberSN;
			GetMessageObj(&ho_GrabImage, hv_CurMessage, "A_IMAGE");//���ͼ����Ϣ
			GetMessageTuple(hv_CurMessage, "A_GRABBERSN", &hv_GrabberSN);//��õ�ǰ������
			CString strExceptionFile;
			SYSTEMTIME CurSysTime;
			GetLocalTime(&CurSysTime);
			strExceptionFile.Format("%s\\ExceptionFile\\%d��%02d��\\%02d��\\%s\\%s\\%d"
				, GetCurrentAppPath(), CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay,
				g_strBatch, g_strProductName, hv_GrabberSN.I());
			HTuple bExists;
			FileExists(HTuple(strExceptionFile), &bExists);
			if (0 == bExists)
			{
				CreateMultilevelPath(strExceptionFile);
			}
			CString strFileName;
			strFileName.Format("%s\\%d", strExceptionFile, g_nGrabberCount);
			ho_GrabImage.WriteObject(strFileName);
			HTuple hv_Exception;
			e.ToHTuple(&hv_Exception);
			WriteTuple(hv_Exception, HTuple(strFileName));

			// No exceptions may be raised from stub in parallel case,
			// so we need to store this information prior to cleanup
			bool is_direct_call = hcppthread->IsDirectCall();
			// Attempt to clean up in error case, too
			hcppthread->Exit();
			delete hcppthread;
			// Propagate exception if called directly
			if (is_direct_call)
				throw e;
		}
		return NULL;
	}


	void* _hcppthread_allocator_thread(void *hcthread)
	{
		// +++ define thread context for this procedure
		HDevThread*  hcppthread = (HDevThread*)hcthread;
		try
		{
			// Input parameters
			const HTuple        & hv_GrabberSN = hcppthread->GetInputCtrlParamTuple(0);
			// Call processing_thread
			allocator_thread(hv_GrabberSN);
			// Reduce reference counter of thread object
			hcppthread->Exit();
			delete hcppthread;
		}
		catch (HException& e)
		{

			CString strException;
			strException.Format("allocator_thread�����쳣\r\n�쳣��Ϣ%s  [ %s() ]", e.ErrorMessage().Text()
				, e.ProcName().Text());
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
			// No exceptions may be raised from stub in parallel case,
			// so we need to store this information prior to cleanup
			bool is_direct_call = hcppthread->IsDirectCall();
			// Attempt to clean up in error case, too
			hcppthread->Exit();
			delete hcppthread;
			// Propagate exception if called directly
			if (is_direct_call)
				throw e;
		}
		return NULL;
	}

	void* _hcppthread_Listen_thread(void *hcthread)
	{
		// +++ define thread context for this procedure
		HDevThread*  hcppthread = (HDevThread*)hcthread;
		try
		{
			// Input parameters
			const HTuple        & hv_GrabberSN = hcppthread->GetInputCtrlParamTuple(0);
			// Call processing_thread
			Listen_thread(hv_GrabberSN);
			// Reduce reference counter of thread object
			hcppthread->Exit();
			delete hcppthread;
		}
		catch (HException& e)
		{

			CString strException;
			strException.Format("�����̳߳����쳣\r\n�쳣��Ϣ%s  [ %s() ]", e.ErrorMessage().Text()
				, e.ProcName().Text());
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);

		}
		return NULL;
	}

	void WINAPI _hcppthread_acquisition_thread(const s_GBSIGNALINFO* SigInfo)//�ص�������������ȥд
	{
		acquisition_thread(SigInfo, g_hvec_QueueImages[SigInfo->nGrabberSN].T());
	}
}

BOOL HDevExportCpp::SaveJigsawImage(const int& nGrabberSN, const HObject& ho_ImageSave, int nImageSN)//����8.9����ͼ��ű���ǰ�����ԭͼ
{
	try
	{
		CString strObjectFilePATH, strFileName;
		SYSTEMTIME CurSysTime;
		GetLocalTime(&CurSysTime);
		strObjectFilePATH.Format("%s\\Map\\%d��%02d��%02d��\\%s-%s-%d"
			, GetCurrentAppPath(), CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay,
			g_strBatch, g_strProductName, nGrabberSN);
		CreateMultilevelPath(strObjectFilePATH);
		HTuple  hv_ImageFiles;
		ListFiles(HTuple(strObjectFilePATH),
			(HTuple("files").Append("follow_links")), &hv_ImageFiles);
		TupleRegexpSelect(hv_ImageFiles, (HTuple("\\.(tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima|hobj)$").Append("ignore_case")),
			&hv_ImageFiles);
		strFileName.Format("%s\\Image%04d_%d", strObjectFilePATH, hv_ImageFiles.Length(), nImageSN);
		WriteImage(ho_ImageSave, "jpeg", 0, HTuple(strFileName));


	}
	catch (HException e)
	{
		return FALSE;
	}
	return TRUE;

}
BOOL HDevExportCpp::AlignCameraImages(HObject& ho_GrabImage, HTuple&  hv_GrabberSN, HTuple& hv_Encoder)//ʹ��������ɼ���ͼƬ�ܹ�����
{

#pragma region    "�ڴ��λ"

	int  nGrabberSN = hv_GrabberSN;
	DHInterface* pGrabber = g_vec_Grabber[nGrabberSN];
	__int64 nAddr(0), nImageSize(0), nPixelSize(0), nImageWidth(0), nImageHeight(0), nGrabberType(0);
	GBParamID Param = GBImageBufferAddr;
	g_vec_Grabber[nGrabberSN]->DoSomething("GETPARAM", &Param, &nAddr);
	Param = GBImageBufferSize;
	g_vec_Grabber[nGrabberSN]->DoSomething("GETPARAM", &Param, &nImageSize);
	Param = GBImagePixelSize;
	g_vec_Grabber[nGrabberSN]->DoSomething("GETPARAM", &Param, &nPixelSize);
	Param = GBImageWidth;
	g_vec_Grabber[nGrabberSN]->DoSomething("GETPARAM", &Param, &nImageWidth);
	Param = GBImageHeight;
	g_vec_Grabber[nGrabberSN]->DoSomething("GETPARAM", &Param, &nImageHeight);
	Param = GBGrabberTypeSN;
	g_vec_Grabber[nGrabberSN]->DoSomething("GETPARAM", &Param, &nGrabberType);
	static int nStart = 0;
	if (0 == nStart)
	{
		pBufferFirstImage = new BYTE[2 * nImageSize];
		pBufferNextImage = pBufferFirstImage + nImageSize;
		memset(pBufferFirstImage, 0, 2 * nImageSize);
		memcpy(pBufferNextImage, (BYTE*)nAddr, nImageSize);
		nStart++;
	}
	else
	{
		memcpy(pBufferFirstImage, pBufferNextImage, nImageSize);
		memcpy(pBufferNextImage, (BYTE*)nAddr, nImageSize);
	}
	static int nThrowLine = 0;//�Ѿ�����������
	static int nCutLine = -1;//Ѱ�ҳ��Ĳ����еı�־��
	static HObject ho_Top, ho_Bottom;
	static int nDistanceCamera = g_tSystemSet.nDistanceCamera;
	if (nThrowLine + nImageHeight < g_tSystemSet.nDistanceCamera && nCutLine == -1)//���������������û�����õ��������
	{
		nThrowLine = nThrowLine + nImageHeight;
		return FALSE;
	}
	else//�Ѿ�����ƴ�ӵ���
	{
		if (-1 == nCutLine)//��û�еó�ƴ�ӵ�
		{
			nCutLine = g_tSystemSet.nDistanceCamera - nThrowLine;
			if (nCutLine == nImageHeight)//�����������,��ôΪ�˱�������������У�����-1
			{
				nCutLine -= 1;
			}
			pBufferTilePostion = pBufferFirstImage + nCutLine * nImageWidth;
			return FALSE;
		}
		else
		{
			GenImage1(&ho_GrabImage, "byte", (Hlong)nImageWidth, (Hlong)nImageHeight, (__int64)pBufferTilePostion);
			if (g_nGrabberType == 0)
			{

				MirrorImage(ho_GrabImage, &ho_GrabImage, "row");
			}


			hv_Encoder = hv_Encoder - ((nImageHeight - nCutLine) + g_tSystemSet.nDistanceCamera)*g_vec_RowResolution[1];
			return TRUE;
		}
	}

#pragma endregion

}
class CAlignedTwoCamera
{
public:
	CAlignedTwoCamera()
	{
		m_hvLastImageSN = 0;
	}
public:
	void Append(const HTuple& hv_FrameSN, const HTuple& hv_Row, HTuple& hv_TotalHeight)
	{
		HTuple hv_NewTuple;
		m_tLock.Lock();
		TupleGenConst(hv_Row.Length(), hv_FrameSN, &hv_NewTuple);
		m_hv_FrameSN.Append(hv_NewTuple);
		m_hv_Row.Append(hv_Row);
		m_tLock.Unlock();
	}
	BOOL Query(const HTuple& hv_FrameSN, const BOOL& bRemove, HTuple* hv_Row, HTuple& hv_TotalHeight)//��ѯ�Ƿ��֡����ƴͼ��������� ����Ϊhv_Row
	{
		BOOL bRet = FALSE;
		HTuple hv_TempRow;
		m_tLock.Lock();
		HTuple hv_Indices;
		BOOL bHaveRow = FALSE;//Ĭ�ϲ����ڲ��е�
		int nCurrentFrameSN = 0;//��ǰ���ڲ��е��֡��
		int k = 0;//��¼��ʼ���е�����ƴͼ�еľ��Ը߶�
		double dLastCropPosition;//��¼��ʼ���е�����ƴͼ�еľ��Ը߶�
		HTuple hv_Max;
		HTuple hv_CurrentSN;
		for (int j = m_hvLastImageSN + 1; j <= hv_FrameSN.I(); j++)
		{

			TupleFind(m_hv_FrameSN, j, &hv_Indices);

			if (hv_Indices.Length() > 0 && hv_Indices != -1)//������������Ԫ��
			{

				bHaveRow = TRUE;//���ڲ��е�
				TupleGenConst(hv_Indices.Length(), j, &hv_CurrentSN);
				TupleSelect(m_hv_Row, hv_Indices, &hv_TempRow);
				if (k == 0)
				{
					(*hv_Row).Append(hv_TempRow);
					TupleMax(hv_TempRow, &hv_Max);
					dLastCropPosition = hv_Max.D() + 1;

				}
				else if (k >= 1)
				{

					(*hv_Row).Append(hv_TempRow + dLastCropPosition);
					TupleMax(*hv_Row, &hv_Max);
					dLastCropPosition = hv_Max.D() + 1;

				}
				k++;
				nCurrentFrameSN = j;
				if (TRUE == bRemove)
				{
					TupleRemove(m_hv_FrameSN, hv_Indices, &m_hv_FrameSN);
					TupleRemove(m_hv_Row, hv_Indices, &m_hv_Row);
				}
			}


		}
		if (bHaveRow)//���ڲ��е�
		{
			bRet = TRUE;
			m_hvLastImageSN = nCurrentFrameSN;
		}
		else
		{
			*hv_Row = 0;
			bRet = FALSE;
		}

		m_tLock.Unlock();
		return bRet;
	}
private:
	HTuple m_hvLastImageSN;
	HTuple m_hv_FrameSN;
	HTuple m_hv_Row;
	CCriticalSection m_tLock;
};

//class CAlignedTwoCamera
//{
//public:	
//	CAlignedTwoCamera()
//	{
//		m_hvLastImageSN=0;
//	}
//public:
//	void Append(const HTuple& hv_FrameSN,const HTuple& hv_Row,HTuple& hv_TotalHeight)
//	{
//		HTuple hv_NewTuple;
//		m_tLock.Lock();
//		TupleGenConst(hv_Row.Length(), hv_FrameSN, &hv_NewTuple); 
//		m_hv_FrameSN.Append(hv_NewTuple);
//		m_hv_Row.Append(hv_Row);
//
//		m_tLock.Unlock();
//	}
//	BOOL Query(const HTuple& hv_FrameSN,const BOOL& bRemove,HTuple* hv_Row,HTuple& hv_TotalHeight)//��ѯ�Ƿ��֡����ƴͼ��������� ����Ϊhv_Row
//	{
//		BOOL bRet = FALSE;
//		//static int s_nLastFrameSN=0;//������ϴβ�ѯ����ͼ���
//		HTuple hv_TempRow;
//		m_tLock.Lock();
//		HTuple hv_Indices;
//		BOOL bHaveRow=FALSE;//Ĭ�ϲ����ڲ��е�
//		int nCurrentFrameSN=0;//��ǰ���ڲ��е��֡��
//		int k=0;//��¼��ʼ���е�����ƴͼ�еľ��Ը߶�
//		double dLastCropPosition;//��¼��ʼ���е�����ƴͼ�еľ��Ը߶�
//		HTuple hv_Max;
//		int nLastImageSN=m_hvLastImageSN;//��־ʹ��
//		HTuple hv_QueryedSN;//����־ʹ��
//		HTuple hv_CurrentSN;
//		hv_QueryedSN.Clear();
//		for (int j=m_hvLastImageSN+1;j<=hv_FrameSN.I();j++)
//		{
//
//			TupleFind(m_hv_FrameSN,j,&hv_Indices);
//
//			if (hv_Indices.Length()>0 && hv_Indices != -1)//������������Ԫ��
//			{
//
//				bHaveRow=TRUE;//���ڲ��е�
//				TupleGenConst(hv_Indices.Length(),j,&hv_CurrentSN);
//				hv_QueryedSN.Append(hv_CurrentSN);
//				TupleSelect(m_hv_Row, hv_Indices, &hv_TempRow);
//				if (k==0)
//				{
//					(* hv_Row).Append(hv_TempRow);
//					TupleMax(hv_TempRow,&hv_Max);
//					dLastCropPosition=hv_Max.D()+1;
//
//				}else if(k>=1)
//				{
//
//					(* hv_Row).Append(hv_TempRow+dLastCropPosition);
//					TupleMax(* hv_Row,&hv_Max);
//					dLastCropPosition=hv_Max.D()+1;
//
//				}
//				k++;
//				nCurrentFrameSN=j;
//				if (TRUE == bRemove)
//				{
//					TupleRemove(m_hv_FrameSN,hv_Indices,&m_hv_FrameSN);
//					TupleRemove(m_hv_Row,hv_Indices,&m_hv_Row);	
//				}
//			}
//
//
//		}
//		if (bHaveRow)//���ڲ��е�
//		{
//			bRet =  TRUE;
//			m_hvLastImageSN=nCurrentFrameSN;
//		}
//		else
//		{
//			*hv_Row = 0;
//			bRet = FALSE;
//		}
//
//		m_tLock.Unlock();
//		return bRet;
//	}
//private:
//	HTuple m_hvLastImageSN;
//	HTuple m_hv_FrameSN;
//	HTuple m_hv_Row;
//	CCriticalSection m_tLock;
//};


void HDevExportCpp::TempLog(int& nGrabberSN, A_GRABWORKSHOPOUTPUT& tOutPut)
{
	SYSTEMTIME tTime;
	GetLocalTime(&tTime);
	HTuple hvHeight;
	GetImageSize(tOutPut.hoGrabImage, NULL, &hvHeight);
	CString strFileDiretory, strFile;
	strFileDiretory.Format("%s\\Log\\ͼ��ż��\\%02d-%02d\\%02dʱ-ͼ�����Ϣ", GetCurrentAppPath(), tTime.wMonth, tTime.wDay, tTime.wHour);
	strFile.Format("%s\\���%d.csv", strFileDiretory, nGrabberSN);
	CString strTime;
	strTime.Format("%02d,%02d,%03d", tTime.wMinute, tTime.wSecond, tTime.wMilliseconds);
	static long long s_SumLength[2] = { 0,0 };
	s_SumLength[nGrabberSN] = s_SumLength[nGrabberSN] + hvHeight.I();
	if (!PathFileExists(strFile))
	{
		CreateMultilevelPath(strFileDiretory);
		CString strItem;
		strItem.Format("��,��,����,֡��,ͼ��߶�,�ۼ�ͼ��߶�");
		ofstream outFile;
		outFile.open(strFile, ios::out);
		outFile << strItem << endl;
		outFile.close();
	}
	else
	{
		CString strItem;
		ofstream outFile;
		outFile.open(strFile, ios::ate | ios::out | ios::app);
		strItem.Format("%s,%d,%d,%ld", strTime, tOutPut.hvImageSN.I(), hvHeight.I(), s_SumLength[nGrabberSN]);
		outFile << strItem << endl;
		outFile.close();
	}
}



void HDevExportCpp::WriteCropRowInfo(int nGrabberSN, CString& strInfo)
{
	HTuple FileHandle;
	SYSTEMTIME CurSysTime;
	GetLocalTime(&CurSysTime);
	CString strFilePath, strFileDirector;
	HTuple bExists;
	strFileDirector.Format("%s\\Log\\������Ϣ\\%02d-%02d-%02d\\%s", GetCurrentAppPath(), CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, g_strBatch);
	FileExists(HTuple(strFileDirector), &bExists);
	if (0 == bExists)
	{
		CreateMultilevelPath(strFileDirector);
	}
	strFilePath.Format("%s\\���%d�����ѯ��Ϣ-%s.txt", strFileDirector, nGrabberSN, g_strBatch);
	CString strTime, strWriteInfo;
	strTime.Format("%02d.%02d.%02d %03d", CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
	strWriteInfo.Format("%s:%s", strTime, strInfo);
	OpenFile((HTuple)strFilePath, "append", &FileHandle);
	FnewLine(FileHandle);
	FwriteString(FileHandle, HTuple(strWriteInfo));
	CloseFile(FileHandle);
}

void HDevExportCpp::WriteExceptImage(HObject &ho_ExceptionImage, HTuple hv_GrabberSN)
{
	static int nNum[4] = { 0 };
	CString strExceptionFile, strException;
	SYSTEMTIME CurSysTime;

	GetLocalTime(&CurSysTime);
	strExceptionFile.Format("%s\\ExceptionFile\\%s\\%d", GetCurrentAppPath(), g_strBatch, hv_GrabberSN);
	HTuple bExists;
	FileExists(HTuple(strExceptionFile), &bExists);
	if (0 == bExists)
	{
		CreateMultilevelPath(strExceptionFile);
	}

	CString strFileName;
	strFileName.Format("%s//%02d��%02dʱ%02d��%d%s", strExceptionFile, CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, ++nNum[hv_GrabberSN], ".jpg");
	WriteImage(ho_ExceptionImage, "jpeg", 0, (HTuple)strFileName);
	strException.Format("%d���,����û���ҵ�ƴͼ��λģ��", hv_GrabberSN);
	g_pLog->DoSomething("TEMPLOG", (void*)(LPCSTR)strException, NULL);

}

int HDevExportCpp::Jigsaw(A_GRABWORKSHOPOUTPUT &Output, const int& nGrabberSN, const int& nFrameSN, HObject& hoGrabImage, HObject* hoCompleteImage, HObject* hoRemainImage)
{


	try
	{

		static const int LIST_LENGTH = 4;//���֧����·ƴͼ 
		static HObject ho_Jigsawlist[LIST_LENGTH];//ƴͼ��ʱ����
		static CAlignedTwoCamera tAlignedTwoCamera[LIST_LENGTH / 2];//����ͨѶ �������ʹ��ǰ�����ƴͼ������ Ĭ��ʹ�ø�����
		HObject hoEmptyObj, ObjectsConcat, TiledImage;
		HTuple Height, Width, hv_ImageHeight;
		HTuple hvTotalWidth, hvTotalHeight;

		GenEmptyObj(&hoEmptyObj);
		GetImageSize(hoGrabImage, NULL, &hv_ImageHeight);//�õ��ɼ�ԭͼ�ĸ߶� 
		if (ho_Jigsawlist[nGrabberSN].IsInitialized())//�����ʱƴͼ�����Ѿ���ʼ����
		{
			ConcatObj(ho_Jigsawlist[nGrabberSN], hoGrabImage, &ho_Jigsawlist[nGrabberSN]);//������ͼ���ŵ���Ӧ����ʱƴͼ������
		}
		else
		{
			//**//�����δ��ʼ����������ǵ�һ��ƴͼ��ͼ�� Ӧ�ö���ʱ�������ɿն�����д��
			GenEmptyObj(&ho_Jigsawlist[nGrabberSN]);
			ConcatObj(ho_Jigsawlist[nGrabberSN], hoGrabImage, &ho_Jigsawlist[nGrabberSN]);
		}
		HTuple Number;
		CountObj(ho_Jigsawlist[nGrabberSN], &Number);//�õ���ʱ�����е�Ԫ����Ŀ
		if (Number >= 1)//�������һ��Ԫ��
		{

			TileImages(ho_Jigsawlist[nGrabberSN], &TiledImage);//��������ǵ���ͼ������Ҫ����ƴͼ
			GetImageSize(TiledImage, &hvTotalWidth, &hvTotalHeight);		//�õ�ƴͼ֮��ĸ߶ȺͿ��
			GetImageSize(TiledImage, &Width, &Height);		//�õ�ƴͼ֮��ĸ߶ȺͿ��
			HObject ho_ForFind;//Ѱ��ƴͼ��λ��ͼ������ 
			HObject ho_Rectangle;//Ѱ��ƴͼ��λ������
			HTuple  hv_StartRow = 0;//��ʼ��������
			GenRectangle1(&ho_Rectangle, hv_StartRow, 0, Height - 1, Width - 1);
			ReduceDomain(TiledImage, ho_Rectangle, &ho_ForFind);//�õ�Ѱ��ƴͼ��λ������
			int nAlignedTwoCameraIndex = 0;//�ж����Ҳ�



			NS_SCIFindMark::MARKRESULT tResultFind;//ƴͼ��λ����ṹ��
			//���������Ϊ0 ���� ���Ϊ1����������  �����������Ϊ1
			{
				if (g_tSystemSet.nCameraModel == nGrabberSN)
				{
					// "������������ǰ���Ѱ�Ҷ�λ��ǰ���Ѱ�ҵ���λ��ŵ���ѯ������"
					g_vec_FindMark[nGrabberSN]->DoSomething("FIND", &ho_ForFind, &tResultFind);
					if (TRUE == g_tSystemSet.bAlignedTwoCamera)//������ú����ʹ��ǰ����Ľ�������������������1
					{
						if (tResultFind.hv_Score.Length() > 0 && tResultFind.hv_Score != 0)
						{

							tAlignedTwoCamera[nAlignedTwoCameraIndex].Append(nFrameSN, tResultFind.hv_Row, hvTotalHeight);//�������0�ҵ��ĵ��ŵ�����ȥ

						}
						else//���ƥ���ʵ�Ԫ��ĳ���Ϊ0����ƥ���ʵ����ֵС�ڵ���0 ����Ѱ��ƴͼ��λʧ��1:����û�м��� 2:���������޼���
						{
							///����N���޼���
							double dMaxHeightDistance = g_Jugde[0].m_tStandardSize.dTotalLength*1.5;//��󼫶����

							if (hvTotalHeight > (dMaxHeightDistance / g_vec_RowResolution[0] + 10))//���ƴͼ�Ĳ�Ʒ�ĸ߶ȱ����õ���������ô��Ҫֱ�ӽ�ȡ��ͼ	
							{

								///99.9  99  200  101
								int nMaxDistance = floor(dMaxHeightDistance / g_vec_RowResolution[0]);
								HTuple CompleteRow, hv_RemainHeight;//ʣ�µİ���
								hv_RemainHeight = hvTotalHeight - nMaxDistance;
								CompleteRow = hvTotalHeight - hv_RemainHeight;
								GenEmptyObj(hoCompleteImage);

								try
								{
									CropRectangle1(TiledImage, hoCompleteImage, 0, 0, CompleteRow, Width - 1);
									CropRectangle1(TiledImage, hoRemainImage, CompleteRow + 1, 0, Height - 1, Width - 1);//ʣ���ͼ��߶�	

								}
								catch (HException& e)
								{
									CString strException;
									strException.Format("Jigsawǿ�Ƴ�ͼ�����쳣:%s:%s CompleteRow:%d hv_RemainHeight:%d Height:%d", e.ProcName().Text(), e.ErrorMessage().Text(), CompleteRow.I(), hv_RemainHeight.I(), Height.I());
									g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
								}


								HTuple hv_CropRow, hvImageSN;
								GenEmptyObj(&ho_Jigsawlist[nGrabberSN]);
								ConcatObj(ho_Jigsawlist[nGrabberSN], *hoRemainImage, &ho_Jigsawlist[nGrabberSN]);
								tAlignedTwoCamera[nAlignedTwoCameraIndex].Append(nFrameSN, CompleteRow, Height);//�������0ǿ�Ƴ�ͼ��Ž�ȥ
								return  1;//1����ƴ������ͼ����Ŀ
							}
							else//���δ�ҵ���λ
							{
								return FALSE;
							}
						}
					}
				}
				else
				{
					///"���ں�������������������Լ�Ѱ�Ҷ�λҲ���Ը���ǰ����Ķ�λȡͼ"
					if (TRUE == g_tSystemSet.bAlignedTwoCamera)
					{

						if (TRUE == tAlignedTwoCamera[nAlignedTwoCameraIndex].Query(nFrameSN, TRUE, &tResultFind.hv_Row, hvTotalHeight))
						{
							tResultFind.hv_Score = 1;
						}
						else
						{
							tResultFind.hv_Score.Clear();
						}
					}
					else
					{
						g_vec_FindMark[nGrabberSN]->DoSomething("FIND", &ho_ForFind, &tResultFind);
					}

				}

			}

			try
			{
				//���ƥ���ʵ�Ԫ��ĳ���Ϊ0����ƥ���ʵ����ֵС�ڵ���0 ����Ѱ��ƴͼ��λʧ��1:����û�м��� 2:���������޼���
				if (0 == tResultFind.hv_Score.Length())
				{
					return FALSE;
				}
				else//Ѱ�Ҷ�λ�ɹ� //Ѱ�Ҷ�λ�ɹ� 
				{
					///Ѱ�ҵ�������ĸ������ڷָ������ͼ�����Ŀ
					int nNumberComplete = tResultFind.hv_Row.Length();
					try
					{

						GenEmptyObj(hoCompleteImage);
						HObject ho_SingleImage;
						for (int i = 0; i < nNumberComplete; i++)//ƴ��N�ŵĻ���Ҫ�ֱ��г�N��ͼƬ����
						{
							if (0 == i)
							{
								CropRectangle1(TiledImage, &ho_SingleImage, 0, 0, MIN(tResultFind.hv_Row[i].D(), Height.I() - 1), Width - 1);

							}
							else
							{
								CropRectangle1(TiledImage, &ho_SingleImage, MIN(tResultFind.hv_Row[i - 1].D() + 1, Height.I() - 1), 0, MIN(tResultFind.hv_Row[i].D(), Height.I() - 1), Width - 1);

							}
							ConcatObj(*hoCompleteImage, ho_SingleImage, hoCompleteImage);
						}
						CropRectangle1(TiledImage, hoRemainImage, MIN(tResultFind.hv_Row[nNumberComplete - 1].D() + 1, Height.I() - 2), 0, Height - 1, Width - 1);
						GenEmptyObj(&ho_Jigsawlist[nGrabberSN]);
						ConcatObj(ho_Jigsawlist[nGrabberSN], *hoRemainImage, &ho_Jigsawlist[nGrabberSN]);
					}
					catch (HException& e)
					{
						CString strException;
						strException.Format("%d�����ȡͼƬʱ����%s ��������%s ͼ��߶�:%d ͼ���ȶ�:%d ", nGrabberSN, e.ProcName().Text(), e.ErrorMessage().Text(), Height.I(), Width.I());
						CString strImagePath, strTuplePath;
						strImagePath.Format("%s\\Log\\TiledImage.jpg", GetCurrentAppPath());
						WriteImage(TiledImage, "jpeg", 0, HTuple(strImagePath));
						strTuplePath.Format("%s\\Log\\tResultFindhv_Row.tup", GetCurrentAppPath());
						WriteTuple(tResultFind.hv_Row, (HTuple)strTuplePath);
						g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
					}
					return nNumberComplete;
				}
			}
			catch (HException& e)
			{
				CString strException;
				strException.Format("��������ƴͼ��λƥ��ʧ�ܳ����쳣:%s:%s", e.ProcName().Text(), e.ErrorMessage().Text());
				g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);

			}
		}
		else//�����ϴ˴���֧��Զ������ ��Ϊ����϶���������һ������
		{
			return FALSE;
		}
	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("Jigsaw�����쳣:%s:%s", e.ProcName().Text(), e.ErrorMessage().Text());
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}
}

int  HDevExportCpp::GetFrameSN(const int& nGrabberSN)
{
	static int nFrameSN[4] = { 0 };
	nFrameSN[nGrabberSN]++;
	return nFrameSN[nGrabberSN];
}







void HDevExportCpp::acquisition_thread(const s_GBSIGNALINFO* SigInfo, HTuple hv_QueueImage)
{
	HTuple hv_TimeStart, hv_TimeEnd;

	CountSeconds(&hv_TimeStart);//��ʼ��ʱ
	WPARAM wParam = MAKEWPARAM(SigInfo->nGrabberSN, SigInfo->nErrorCode);
	int nGrabberSN = SigInfo->nGrabberSN;
	int nImageSN = SigInfo->nFrameCount;
	if (SigInfo->nErrorCode != 0)
	{
		LPARAM lParam = MAKELPARAM(SigInfo->nFrameCount, 0);
		((CSeekersDlg*)g_pMainDialog)->PostMessageA(WM_GRABBER, wParam, lParam);//֪ͨ״̬���ɼ�����һ֡
		CString strException;
		strException.Format("���%d�ص��вɼ�����%d", nGrabberSN, SigInfo->nErrorCode);
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
		return;
	}
	A_GRABWORKSHOPOUTPUT tOutPut;
	/*
	HObject hoGrabImage;//�ɼ�����ͼ��
	HTuple  hvGrabberSN;//������
	HTuple  hvFrameSN; //�ڼ�֡
	HTuple  hvEncoder;//����������
	HTuple  hvImageSN;//ͼ���
	HTuple  hv_PinTuNormal;//ƴͼ�ɹ��ı�־ 0��δƴͼ��ʼֵ 1�������ɹ� 2��ǿ�Ƴ�ͼ
	HTuple  hvTimeStamp;//ʱ���
	*/
	///////
	if (0 == nGrabberSN)
	{
		tOutPut.hvGrabberSN = SigInfo->nGrabberSN;//ǰ��������
		tOutPut.hvEncoder = GetEncoder(nGrabberSN);//�õ���ǰ�ı�����λ��
		tOutPut.hv_PinTuNormal = 0;

		if (0 == SigInfo->iGrabberTypeSN)//�����ģ��ɼ�
		{
			tOutPut.hvImageSN = SigInfo->nFrameCount;
		}
		else if (1 == SigInfo->iGrabberTypeSN)
		{
			int nReadCount = 28;
			g_pCommunicate->DoSomething("READCOUNT", &nReadCount, &nImageSN);
			tOutPut.hvImageSN = nImageSN;

			static int S_nLastImageSN = -1;
			static int S_nContinuSnNum = 0;
			if (S_nLastImageSN == nImageSN)
			{
				S_nContinuSnNum++;
				if (S_nContinuSnNum > 5)//ͼ�����������5���ͱ���ͣ��
				{
					CString  strException;
					strException.Format("��ǰ���ͼ���%d�����ϴ�ͼ���%d,���Ų���·�Ӵ��Ƿ�����", nImageSN, S_nLastImageSN);
					g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
					S_nContinuSnNum = 0;
				}
			}
			else
			{
				S_nContinuSnNum = 0;
			}
			S_nLastImageSN = nImageSN;
		}
		GetImage(nGrabberSN, &tOutPut.hoGrabImage);//����ͼ��
	}
	else if (1 == nGrabberSN)
	{
		tOutPut.hvGrabberSN = SigInfo->nGrabberSN;
		tOutPut.hvEncoder = GetEncoder(nGrabberSN);
		tOutPut.hv_PinTuNormal = 0;
		if (0 == SigInfo->iGrabberTypeSN)//�����ģ��ɼ�
		{
			tOutPut.hvImageSN = SigInfo->nFrameCount;
		}
		else if (1 == SigInfo->iGrabberTypeSN)
		{
			int nReadCount = 28;
			g_pCommunicate->DoSomething("READCOUNT", &nReadCount, &nImageSN);
			tOutPut.hvImageSN = nImageSN;
		}
		if (FALSE == AlignCameraImages(tOutPut.hoGrabImage, tOutPut.hvGrabberSN, tOutPut.hvEncoder))//��δ��ƴ�ӵ㷵��
		{
			return;
		}
	}

	try
	{
		/////Ͷ����Ϣʱ 2��������ǰ������������Ͷ��
		{

			{
				HTuple hv_MessageSend;
				CreateMessage(&hv_MessageSend);
				tOutPut.hvGrabberSN = nGrabberSN;
				tOutPut.hvFrameSN = GetFrameSN(tOutPut.hvGrabberSN);
				tOutPut.hv_PinTuNormal = 0;
				CountSeconds(&hv_TimeEnd);//������ʱ
				tOutPut.hvTimeStamp = (hv_TimeEnd - hv_TimeStart) * 1000;
				tOutPut.WriteMessage(hv_MessageSend, tOutPut.hvGrabberSN);
				try
				{




					EnqueueMessage(g_hvec_QueuePintu[tOutPut.hvGrabberSN].T(), hv_MessageSend, HTuple(), HTuple());


				}
				catch (HException& e)
				{

					static BOOL  s_bFlag = FALSE;
					CString strException;
					strException.Format("���%dƴͼ��������,���߳����˳�,���Ų�ԭ��", nGrabberSN);
					if (s_bFlag == FALSE)
					{
						g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
						s_bFlag == TRUE;
					}
					g_CUpLoad.SendExceptionWarn(nGrabberSN, strException);

				}
				ClearMessage(hv_MessageSend);
				LPARAM lParam = MAKELPARAM(tOutPut.hvFrameSN[0].I(), nImageSN);
				((CSeekersDlg*)g_pMainDialog)->PostMessageA(WM_GRABBER, wParam, lParam);//֪ͨ״̬���ɼ�����һ֡
			}
		}

	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("acquisition_thread�����쳣\r\n�쳣��Ϣ%s  [ %s() ]", e.ErrorMessage().Text()
			, e.ProcName().Text());
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}


	CountSeconds(&hv_TimeEnd);//ִ�н���
	CString strTime;
	strTime.Format("%01d����ص���ʱ:%.2f", nGrabberSN, (hv_TimeEnd[0].D() - hv_TimeStart[0].D()) * 1000);
	g_pLog->DoSomething("LOG", (void*)(LPCSTR)(strTime), NULL);
}

void HDevExportCpp::Pintu_thread(HTuple hv_ThreadIndex, HTuple* hv_CurMessage)
{
	HTuple hv_MessageRecv, hv_StopProcInfo, hv_TimeStart, hv_TimeEnd;
	int nGrabberSN = hv_ThreadIndex.I();
	CString strException;
	while (0 != 1)
	{
		DequeueMessage(g_hvec_QueuePintu[nGrabberSN].T(), "timeout", "infinite", &hv_MessageRecv);
		CountSeconds(&hv_TimeStart);
		//��������ṹ��
		B_DETECTWORKSHOPOUTPUT tOutput;
		tOutput.A.ReadMessage(hv_MessageRecv, nGrabberSN);
		ClearMessage(hv_MessageRecv);

		/*****����ƴͼģ�飬����ܹ�ƴ��һ�������ļ�Ƭ������ȥ���/****/

		if (TRUE == g_tSystemSet.bTileImage && g_Jugde->m_tStandardSize.hv_CoatType == M_TwoCam_Intermission)//ֻ�м�Ъ���ϲſ���ѡ��ƴͼ
		{

			int nNumberComplete;//ƴͼ����ͼ��ĸ��� 0������δƴ���� -1������ǿ�Ƴ�ͼ  ��������������ͼ����Ŀ
			HObject ho_CompleteImage, ho_RemainImage;//����ͼ���ʣ��ͼ�񲿷�
			HTuple hv_Height;//�ɼ���ͼ��߶�
			GetImageSize(tOutput.A.hoGrabImage, NULL, &hv_Height);//5000
			nNumberComplete = Jigsaw(tOutput.A, nGrabberSN, tOutput.A.hvFrameSN, tOutput.A.hoGrabImage, &ho_CompleteImage, &ho_RemainImage);
			int n_ImageSN;
			n_ImageSN = tOutput.A.hvImageSN;//ǰ���ͼ��űȽ���Ҫ�漰�����꣬�������ͼ��Ż���ûɶ����
			if (nNumberComplete > 0)//�ж�ƴͼ�õ�����Ŀ
			{

				HTuple hv_CompleteImageHeight, hv_RemainHeight;
				GetImageSize(ho_CompleteImage, NULL, &hv_CompleteImageHeight);
				GetImageSize(ho_RemainImage, NULL, &hv_RemainHeight);//���ʱ��ʣ���ֵ��һ������Ҫ���ÿ��һ��ͼʣ���Height
				for (int i = 0; i < nNumberComplete; i++)
				{
					HTuple hv_CurFrameRemainHeight, hv_Sum;
					if (i + 1 > hv_CompleteImageHeight.Length() - 1)
					{
						hv_CurFrameRemainHeight = hv_RemainHeight;
					}
					else
					{
						HTuple hv_Select;
						TupleSelectRange(hv_CompleteImageHeight, i + 1, hv_CompleteImageHeight.Length() - 1, &hv_Select);//
						TupleSum(hv_Select, &hv_Sum);
						hv_CurFrameRemainHeight = hv_RemainHeight + hv_Sum;
					}

					if (hv_CurFrameRemainHeight <= hv_Height)//ʣ���С�ڵ��Ų�Ʒ�ĸ߶� 2996  С�ڲ�ͼ�߶�
					{
						tOutput.hv_CropRow = hv_Height - hv_CurFrameRemainHeight;
						tOutput.A.hvImageSN = n_ImageSN;

					}
					else
					{

						int nFrames = floor(hv_CurFrameRemainHeight.D() / hv_Height.D());
						if (hv_Height > hv_CurFrameRemainHeight.D() - nFrames*hv_Height.D())
						{
							if (hv_CurFrameRemainHeight.D() == nFrames*hv_Height.D())//�պ���0
							{
								tOutput.hv_CropRow = 0;//�߶ȵ�������Ҫ��ȥ1
								tOutput.A.hvImageSN = n_ImageSN - nFrames + 1;
							}
							else
							{
								tOutput.hv_CropRow = hv_Height - (hv_CurFrameRemainHeight.D() - nFrames*hv_Height.D());//
								tOutput.A.hvImageSN = n_ImageSN - nFrames;
							}
						}
					}
					CountSeconds(&hv_TimeEnd);
					tOutput.hvTimeStamp = (hv_TimeEnd.D() - hv_TimeStart.D()) * 1000;

					///ÿ�ζ�Ԥ��100�е�ͼ��
					HTuple hv_TempWidth, hv_TempHeight;
					static HObject S_hoLastImage[2];
					HObject hoSelectImage, hoConcatObj;
					hoConcatObj.GenEmptyObj();
					hoSelectImage = ho_CompleteImage.SelectObj(i + 1);
					GetImageSize(hoSelectImage, &hv_TempWidth, &hv_TempHeight);
					if (S_hoLastImage[nGrabberSN].IsInitialized())
					{
						ConcatObj(S_hoLastImage[nGrabberSN], hoSelectImage, &hoConcatObj);
						TileImages(hoConcatObj, &hoSelectImage);
					}
					///�̶�����199��
					CropRectangle1(ho_CompleteImage.SelectObj(i + 1), &S_hoLastImage[nGrabberSN], hv_TempHeight - 199, 0, hv_TempHeight - 1, hv_TempWidth - 1);

					tOutput.A.hoGrabImage = hoSelectImage;//�ɼ���ͼƬ����Ϊƴͼ֮���ͼƬ

					//tOutput.A.hoGrabImage = ho_CompleteImage.SelectObj(i+1);//�ɼ���ͼƬ����Ϊƴͼ֮���ͼƬ
					tOutput.A.hvEncoder = tOutput.A.hvEncoder - hv_CurFrameRemainHeight * g_vec_RowResolution[0];

					///������־
					/*if (nGrabberSN==0)
					{
					CString strlog;
					strlog.Format("���%dͼ���:%d ����λ��:%.1f", nGrabberSN, tOutput.A.hvImageSN.I(), tOutput.hv_CropRow.D());
					g_pLog->DoSomething("OPERATERLOG", (void*)(LPCSTR)strlog, NULL);
					}
					*/

					HTuple hvMessageSend;
					CreateMessage(&hvMessageSend);
					tOutput.WriteMessage(hvMessageSend, nGrabberSN);
					try
					{
						EnqueueMessage(g_hvec_QueueImages[nGrabberSN].T(), hvMessageSend, HTuple(), HTuple());
					}
					catch (HException& e)
					{
						static BOOL bFlag = TRUE;
						strException.Format("���%d�����������,���߳����˳�,���Ų�ԭ��", tOutput.A.hvGrabberSN.I());
						if (bFlag)
						{
							bFlag = FALSE;
							if (H_ERR_MQOVL == e.ErrorCode())
							{


								g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);

							}
						}
						g_CUpLoad.SendExceptionWarn(nGrabberSN, strException);
					}
					ClearMessage(hvMessageSend);
				}

			}
			else
			{
				continue;//���û��ƴ��ͼƬ����ֱ�Ӿͷ�����
			}
		}
		else
		{
			CountSeconds(&hv_TimeEnd);
			tOutput.hvTimeStamp = hv_TimeEnd - hv_TimeStart;
			tOutput.hv_CropRow = 0;
			HTuple hvMessageSend;
			CreateMessage(&hvMessageSend);
			tOutput.WriteMessage(hvMessageSend, nGrabberSN);
			try
			{
				EnqueueMessage(g_hvec_QueueImages[nGrabberSN].T(), hvMessageSend, HTuple(), HTuple());
			}
			catch (HException& e)
			{

				strException.Format("���%d�����������,���߳����˳�,���Ų�ԭ��", tOutput.A.hvGrabberSN.I());
				g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
				g_CUpLoad.SendExceptionWarn(nGrabberSN, strException);
			}
			ClearMessage(hvMessageSend);

		}

		CountSeconds(&hv_TimeEnd);
		CString strTime;
		strTime.Format("%01d���ƴͼ�̺߳�ʱ:%.01f", nGrabberSN, (hv_TimeEnd.D() - hv_TimeStart.D()) * 1000);
		g_pLog->DoSomething("LOG", (void*)(LPCSTR)(strTime), NULL);
	}
}

void HDevExportCpp::JudageMarkExist(C_ALLOCATORWORKSHOPOUTPUT& C_OutputInfo, int& nSide)
{
	try
	{
		C_OutputInfo.B[0].tDetectOutput.hv_JuanRaoMarkRow = -1;
		C_OutputInfo.B[1].tDetectOutput.hv_JuanRaoMarkRow = -1;

		static HTuple S_LastImageHeight[2] = { 0,0 };
		///ֻ�е�һ�βɼ���ʱ��Ż�ȡͼ��ĸ߶�
		static HTuple S_nSingleImageHeight = 1;
		if (S_nSingleImageHeight == 1)
		{
			__int64 nSingleImageHeight;
			GBParamID Param = GBImageHeight;
			g_vec_Grabber[0]->DoSomething("GETPARAM", &Param, &nSingleImageHeight);
			S_nSingleImageHeight = (int)nSingleImageHeight;

		}
		///��ȡ��ǰͼ��߶�
		HTuple hv_Width, hv_Height;
		GetImageSize(C_OutputInfo.B[0].A.hoGrabImage, &hv_Width, &hv_Height);
		double dCutPosition = g_Jugde[nSide].m_tStandardSize.hvCutPosition / g_vec_RowResolution[0];
		////��������EA �ڵ�ǰͼ���������Ϊ�����׼
		C_OutputInfo.B[0].tDetectOutput.hv_JuanRaoMarkRow = MIN(dCutPosition, hv_Height.I() - 1);
		C_OutputInfo.B[1].tDetectOutput.hv_JuanRaoMarkRow = MIN(dCutPosition, hv_Height.I() - 1);
	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("��%d����EA�ж�δ������������%s:%s", nSide, e.ProcName().Text(), e.ErrorMessage().Text());
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}
}

void HDevExportCpp::processing_thread(HTuple hv_ThreadIndex, HTuple* hv_CurMessage)
{
	HTuple hv_MessageRecv, hv_StopProcInfo;
	int nGrabberSN = hv_ThreadIndex.I();
	HTuple hv_TimeStart, hv_TimeEnd;
	CString strException;
	CString strTime;
	while (0 != 1)
	{
		DequeueMessage(g_hvec_QueueImages[nGrabberSN].T(), "timeout", "infinite", &hv_MessageRecv);
		CountSeconds(&hv_TimeStart);//��ʼ��ʱ
		//��������ṹ��
		B_DETECTWORKSHOPOUTPUT tOutput;

		tOutput.ReadMessage(hv_MessageRecv, nGrabberSN);
		tOutput.hv_PinTuHalfWay = g_dRealSpeed;

		ClearMessage(hv_MessageRecv);
		//Input��ֵ	
		DETECTOUTPUT tDetectOutput;
		DETECTINPUT  tDetectInput;
		tDetectInput.hoDetectImage = tOutput.A.hoGrabImage;
		tDetectInput.nBreakType = 0;
		//��ʼ���
		if (TRUE != g_vec_Algorithm[tOutput.A.hvGrabberSN.I()]->DoSomething("ACTION", &tDetectInput, &tDetectOutput))
		{
			strException.Format("�㷨%dδ��������,���߳����˳�,���Ų�ԭ��", tOutput.A.hvGrabberSN.I());
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
			break;
		}
		//�Խ�������ж�
		if (tDetectOutput.ho_ErrorRegion.CountObj() != tDetectOutput.hv_ErrorBelongStrip.Length()
			|| tDetectOutput.ho_ErrorRegion.CountObj() != tDetectOutput.hv_ErrorLevel.Length()
			|| tDetectOutput.ho_ErrorRegion.CountObj() != tDetectOutput.hv_ErrorType.Length())
		{
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)("processing_thread()��CountObj!=LEVEL!=TYPE!!"), NULL);
			strException.Format("���%d�㷨����쳣,���߳����˳�,���Ų�ԭ��", tOutput.A.hvGrabberSN.I());
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);

			break;
		}
		else
		{
			tOutput.tDetectOutput = tDetectOutput;

		}

		CountSeconds(&hv_TimeEnd);//��ʼ��ʱ
	/*	tOutput.hvTimeStamp=(hv_TimeEnd-hv_TimeStart)*1000;*/
		tOutput.hvTimeStamp.Append((hv_TimeEnd - hv_TimeStart) * 1000);
		/////Ͷ����Ϣ
		HTuple hvMessageSend;
		CreateMessage(&hvMessageSend);
		tOutput.WriteMessage(hvMessageSend, nGrabberSN);
		try
		{
			EnqueueMessage(g_hvec_QueueResults[nGrabberSN].T(), hvMessageSend, HTuple(), HTuple());
		}
		catch (HException& e)
		{

			static BOOL  s_bFlag = FALSE;
			strException.Format("���%d�����������,���߳����˳�,���Ų�ԭ��", tOutput.A.hvGrabberSN.I());
			if (s_bFlag == FALSE)
			{
				g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
				s_bFlag = TRUE;
			}
			g_CUpLoad.SendExceptionWarn(nGrabberSN, strException);
		}
		ClearMessage(hvMessageSend);
		CountSeconds(&hv_TimeEnd);//ִ�н���
		HTuple hv_Height;
		GetImageSize(tOutput.A.hoGrabImage, NULL, &hv_Height);
		strTime.Format("%01d��������̺߳�ʱ��%.2f ͼƬ�߶�:%d,", nGrabberSN, (hv_TimeEnd.D() - hv_TimeStart.D()) * 1000, hv_Height[0].I());
		g_pLog->DoSomething("LOG", (void*)(LPCSTR)(strTime), NULL);
	}
}





double HDevExportCpp::GetEncoder(const int& nGrabberSN)
{
	__int64 nImageHeight(0);
	GBParamID Param = GBImageHeight;
	g_vec_Grabber[nGrabberSN]->DoSomething("GETPARAM", &Param, &nImageHeight);
	static double dEnCoder[2] = { 0,0 };
	dEnCoder[nGrabberSN] = dEnCoder[nGrabberSN] + nImageHeight * g_vec_RowResolution[nGrabberSN];
	if (nGrabberSN == 0)
	{
		g_dwYield = g_dwYield + nImageHeight * g_vec_RowResolution[nGrabberSN];
	}
	g_dEncoderLocation[nGrabberSN] = dEnCoder[nGrabberSN];

	return dEnCoder[nGrabberSN];
}

void HDevExportCpp::CopyMessage(C_ALLOCATORWORKSHOPOUTPUT & tSourceOutPut, C_ALLOCATORWORKSHOPOUTPUT &tDestOutPut)
{
	HTuple hv_Message;
	CreateMessage(&hv_Message);
	tSourceOutPut.WriteMessage(hv_Message);
	tDestOutPut.ReadMessage(hv_Message);
	ClearMessage(hv_Message);
}

BOOL HDevExportCpp::DealData(C_ALLOCATORWORKSHOPOUTPUT &Output, int & nSide, CString & StrID)
{

	static int bIsFindMarkFirst[2] = { TRUE,TRUE };
	static int  snCellNumber[2] = { 0 };
	HTuple hv_StandarHeightTuple, hv_HeightTuple;
	hv_HeightTuple.Clear();
	hv_StandarHeightTuple.Clear();
	int  nStandardCellNumber = g_Jugde[nSide].m_tStandardSize.nCellNumber;
	HTuple hv_TimeStart, hv_TimeEnd;
	CountSeconds(&hv_TimeStart);



	if (Output.B[0].tDetectOutput.hv_JuanRaoMarkRow != -1 && Output.B[0].tDetectOutput.hv_JuanRaoMarkRow.Length() > 0)
	{

		///�״��ҵ�������Mark
		if (bIsFindMarkFirst[nSide] == TRUE)
		{
			{
				SYSTEMTIME CurSysTime;
				GetLocalTime(&CurSysTime);
				CString strTime;
				strTime.Format("%02d%02d%02d%02d%02d_%02d", CurSysTime.wMonth, CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
				StrID = strTime;
				Output.hvGroupID = (HTuple)strTime;
				Output.tFinalResult.hv_ProductID = (HTuple)strTime;
			}
			g_dwCorrectHeight[nSide] = Output.B[nSide].tDetectOutput.hv_CoatHeight;
			bIsFindMarkFirst[nSide] = FALSE;
			return 2;
		}
		else  //���״��ҵ�mark ��Ҫ�������У��
		{

			HTuple hv_MarkHeightLength;
			hv_HeightTuple.Clear();
			hv_StandarHeightTuple.Clear();
			hv_MarkHeightLength = g_Jugde[nSide].m_tStandardSize.hv_CoatHeight[0] + g_Jugde[nSide].m_tStandardSize.hv_CoatHeight[nStandardCellNumber];
			hv_HeightTuple.Append(g_dwCorrectHeight[nSide]);
			hv_HeightTuple.Append(Output.B[nSide].tDetectOutput.hv_CoatHeight);
			hv_StandarHeightTuple.Append(g_Jugde[nSide].m_tStandardSize.hv_CoatHeight[nStandardCellNumber - 1]);
			hv_StandarHeightTuple.Append(hv_MarkHeightLength);
		}
		snCellNumber[nSide] = 0;

	}
	else
	{
		if (bIsFindMarkFirst[nSide] == FALSE)//���״η���Mark�׵Ļ� �ж��Ƿ�֮ǰ�ҵ�mark�ˣ�����ֱ�ӷ��� �������ִ��
		{
			snCellNumber[nSide]++;
			HTuple hv_MarkHeightLength;
			int nCellNumber = MIN(nStandardCellNumber, snCellNumber[nSide]);
			hv_MarkHeightLength = g_Jugde[nSide].m_tStandardSize.hv_CoatHeight[0] + g_Jugde[nSide].m_tStandardSize.hv_CoatHeight[nStandardCellNumber];
			if (snCellNumber[nSide] == 1)
			{
				hv_StandarHeightTuple.Append(hv_MarkHeightLength);
				hv_StandarHeightTuple.Append(g_Jugde[nSide].m_tStandardSize.hv_CoatHeight[nCellNumber]);
			}
			else
			{
				hv_StandarHeightTuple.Append(g_Jugde[nSide].m_tStandardSize.hv_CoatHeight[MAX(0, nCellNumber - 1)]);
				hv_StandarHeightTuple.Append(g_Jugde[nSide].m_tStandardSize.hv_CoatHeight[nCellNumber]);
			}
			hv_HeightTuple.Append(g_dwCorrectHeight[nSide]);
			hv_HeightTuple.Append(Output.B[nSide].tDetectOutput.hv_CoatHeight);
		}
		else
		{
			return FALSE;
		}

	}

	HTuple hv_CorrectHeight;
	hv_CorrectHeight.Clear();
	g_Jugde[nSide].CorrectSizeResult(hv_HeightTuple, hv_StandarHeightTuple, g_Jugde[nSide].m_tStandardSize.dCorrectMin, g_Jugde[nSide].m_tStandardSize.dCorrectMax, &hv_CorrectHeight);
	g_dwCorrectHeight[nSide] = hv_CorrectHeight[1];
	Output.B[nSide].tDetectOutput.hv_CoatHeight = hv_CorrectHeight[1];
	return TRUE;
}


void HDevExportCpp::Listen_thread(HTuple hv_GrabberSN)///�����߳� ��ҪĿ���Ƕ�ȡ�ӿڿ����������
{
	int nSide = hv_GrabberSN.I();//0����ߵ�һ�� 1���ұߵ�1��
	int nIOChangeNum;
	while (!g_bExitThread)
	{
		nIOChangeNum = g_CInJetProcess[0].GetOutChaneNum();


		nIOChangeNum = g_CInJetProcess[1].GetOutChaneNum();

		Sleep(100);
	}
}


BOOL HDevExportCpp::ISZhiDai(C_ALLOCATORWORKSHOPOUTPUT & tOutPut)
{
	return FALSE;
	try
	{
		HObject hoRegion;
		HTuple  hvWidth, hvHeight;
		GetImageSize(tOutPut.B[0].A.hoGrabImage, &hvWidth, &hvHeight);
		Threshold(tOutPut.B[0].A.hoGrabImage, &hoRegion, g_Jugde[0].m_tStandardSize.hvZhiDaiGray - 20, g_Jugde[0].m_tStandardSize.hvZhiDaiGray + 20);
		FillUp(hoRegion, &hoRegion);
		Connection(hoRegion, &hoRegion);
		SelectShapeStd(hoRegion, &hoRegion, "max_area", 70);
		SelectShape(hoRegion, &hoRegion, (HTuple("width").Append("height")), "and", HTuple(g_Jugde[0].m_tStandardSize.dCoatWidth*g_Jugde[0].m_tStandardSize.hvTripNum / 2).TupleConcat(hvHeight / 2), HTuple(INT_MAX).TupleConcat(INT_MAX));
		if (hoRegion.CountObj() > 0)
		{
			CString strValue;
			strValue.Format("��ǰ��ֽ��");
			g_pLog->DoSomething("OPERATERLOG", (void*)(LPCSTR)strValue, NULL);
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strValue, NULL);
			return TRUE;

		}
	}
	catch (HException& e)
	{



	}

	return FALSE;



}

void HDevExportCpp::allocator_thread(HTuple hv_GrabberSN)
{

	HTuple hv_StopProcInfo, hvMessageSend, hv_TimeStart, hv_TimeEnd, hv_TimeEnd1;
	C_ALLOCATORWORKSHOPOUTPUT Output;
	int nSide = hv_GrabberSN.I();//0����ߵ�һ�� 1���ұߵ�1��
	const int STEP = 1;//����
	static HTuple S_nSingleImageHeight = 1;
	if (S_nSingleImageHeight == 1)
	{
		__int64 nSingleImageHeight;
		GBParamID Param = GBImageHeight;
		g_vec_Grabber[0]->DoSomething("GETPARAM", &Param, &nSingleImageHeight);
		S_nSingleImageHeight = (int)nSingleImageHeight;

	}
	while (0 != 1)
	{

		HTupleVector hv_Message(1);
		{
			DequeueMessage(g_hvec_QueueResults[nSide + STEP].T(), "timeout", "infinite", &hv_Message[1].T());//�ж�����Ƿ��Ѿ�������
			DequeueMessage(g_hvec_QueueResults[nSide].T(), "timeout", "infinite", &hv_Message[0].T());
			CountSeconds(&hv_TimeStart);
			Output.B[0].ReadMessage(hv_Message[0].T(), nSide);
			Output.B[1].ReadMessage(hv_Message[1].T(), nSide + STEP);

			Output.B[0].tDetectOutput.hv_JuanRaoMarkRow = S_nSingleImageHeight;
			Output.B[1].tDetectOutput.hv_JuanRaoMarkRow = S_nSingleImageHeight;			

			ClearMessage(hv_Message[0].T());
			ClearMessage(hv_Message[1].T());
		}
		Output.hvPackageType = nSide;//��־�����0�� �ұ���1��
		Output.hvPackageSN = 0;
		Output.hvDislocation = 0;
		Output.bIsFinalResult = FALSE;
		Output.tFinalResult.hv_SurfaceFlag = TRUE;
		Output.tFinalResult.hv_SizeFlag = TRUE;
		Output.tFinalResult.hv_TotalLength = 0;
		Output.tFinalResult.hv_SizeErrorIndex.Clear();
		Output.tFinalResult.hv_SurfaceErrorIndex.Clear();
		if (FALSE == ((CSeekersDlg*)g_pMainDialog)->m_bRunning)//����û�����ֹͣ��ť���Ͳ�����ת����Ϣ��
		{
			continue;
		}
		////��ȡEA���////////////////
		g_Jugde[nSide].Action(Output);

		if (Output.bIsFinalResult == TRUE)
		{
			SYSTEMTIME CurSysTime;
			CString strTime;
			GetLocalTime(&CurSysTime);
			strTime.Format("%dEA_%02d%02d%02d%02d%02d_%02d", g_Count.veCount[0][nSide].nTotal, CurSysTime.wMonth, CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
			Output.hvGroupID = (HTuple)g_ArrayStrLastGroupID[nSide];
			Output.tFinalResult.hv_ProductID = (HTuple)g_ArrayStrLastGroupID[nSide];
			g_ArrayStrLastGroupID[nSide] = strTime;
		}
		else
		{
			Output.hvGroupID = (HTuple)g_ArrayStrLastGroupID[nSide];
			Output.tFinalResult.hv_ProductID = (HTuple)g_ArrayStrLastGroupID[nSide];
		}


		CountSeconds(&hv_TimeEnd);//������ʱ
		Output.hvTimeStamp = (hv_TimeEnd - hv_TimeStart) * 1000;
		CreateMessage(&hvMessageSend);//����ҪͶ�ݵ���Ϣ
		Output.WriteMessage(hvMessageSend);

		if (ISZhiDai(Output) == FALSE)//��ֽ����ʱ�򲻴�겻��¼
		{
			if (Output.bIsFinalResult == TRUE)
			{
				g_pCommunicate->DoSomething("PUSH_MESSAGE", &hvMessageSend, NULL);
			}

			g_CUpLoad.Upload(Output);//���͸��������������
			g_pStorage->DoSomething("PUSH_MESSAGE", &hvMessageSend, NULL);//�洢���ݷ���
			try//����ϢͶ�ݵ���ʾ����
			{
				EnqueueMessage(g_hvec_QueueDisplay[0].T(), hvMessageSend, HTuple(), HTuple());//�����ʾ��������,�ͰѶ���ͷ����ȥ��
				((CSeekersDlg*)g_pMainDialog)->PostMessageA(WM_DISP, 0, 0);
			}
			catch (HException& e)
			{


			}

		}
		ClearMessage(hvMessageSend);//��ϢͶ�����,�����Ϣ
		CountSeconds(&hv_TimeEnd1);//ִ�н���
		{
			CString strTime;
			strTime.Format("%d�ߵ��ۺϽ���̻߳���ʱ��:%.01f", nSide, (hv_TimeEnd1.D() - hv_TimeStart.D()) * 1000);
			g_pLog->DoSomething("LOG", (void*)(LPCSTR)(strTime), NULL);
		}
	}

}


bool RGBToGray(const char *srcRGB, char *dstR, char *dstG,
	char *dstB, int nWidth, int nHeight, const int nType)
{
	//if(nType == RGB32)
	{
		try
		{
			int nTotalCellNum = nHeight / 400;
			int nMode = 400;
			BOOL bFlag = FALSE;
			if (nTotalCellNum > 4)
			{
				bFlag = TRUE;
			}
			else
			{
				nMode = 200;
				nTotalCellNum = nHeight / 200;
				if (nTotalCellNum > 4)
				{
					bFlag = TRUE;
				}
			}
			//#pragma omp parallel for if(bFlag)
			for (int Index = 0; Index < nTotalCellNum; Index++)
			{
				int h = Index*nMode;
				int dst = h*nWidth;
				int src = dst;
				char *p;
				p = (char*)srcRGB + src * 4;
				char *pR, *pG, *pB;
				pR = dstR + dst;
				pG = dstG + dst;
				pB = dstB + dst;
				for (int IndexT = 0; IndexT < nMode; IndexT++)
				{
					for (int w = 0; w < nWidth; w++)
					{

						*pR = *p;
						p++;
						*pG = *p;
						p++;
						*pB = *p;
						p++;
						p++;
						pR++;
						pG++;
						pB++;
					}
				}
			}

			for (int nIndex = nTotalCellNum*nMode; nIndex < nHeight; nIndex++)
			{
				int h = nIndex;
				int dst = h*nWidth;
				int src = dst;
				char *p;
				p = (char*)srcRGB + src * 4;
				char *pR, *pG, *pB;
				pR = dstR + dst;
				pG = dstG + dst;
				pB = dstB + dst;
				for (int w = 0; w < nWidth; w++)
				{
					*pR = *p;
					p++;
					*pG = *p;
					p++;
					*pB = *p;
					p++;
					p++;
					pR++;
					pG++;
					pB++;
				}
			}


		}
		catch (...)
		{
			CString strCount = "";
			strCount.Format("RGBToGray-��ͨ�쳣");
			return false;
		}
	}
	return true;
}
void GetImage(int nGrabberSN, HObject* hoGrabberImage)
{
	DHInterface* pGrabber = g_vec_Grabber[nGrabberSN];
	__int64 nAddr(0), nImageSize(0), nPixelSize(0), nImageWidth(0), nImageHeight(0), nGrabberType(0);
	GBParamID Param = GBImageBufferAddr;
	g_vec_Grabber[nGrabberSN]->DoSomething("GETPARAM", &Param, &nAddr);
	Param = GBImageBufferSize;
	g_vec_Grabber[nGrabberSN]->DoSomething("GETPARAM", &Param, &nImageSize);
	Param = GBImagePixelSize;
	g_vec_Grabber[nGrabberSN]->DoSomething("GETPARAM", &Param, &nPixelSize);
	Param = GBImageWidth;
	g_vec_Grabber[nGrabberSN]->DoSomething("GETPARAM", &Param, &nImageWidth);
	Param = GBImageHeight;
	g_vec_Grabber[nGrabberSN]->DoSomething("GETPARAM", &Param, &nImageHeight);
	Param = GBGrabberTypeSN;
	g_vec_Grabber[nGrabberSN]->DoSomething("GETPARAM", &Param, &nGrabberType);



	if (4 == nPixelSize)
	{
		char *RGB32;
		char *ptrR;
		char *ptrG;
		char *ptrB;

		RGB32 = (char*)nAddr;
		ptrR = new char[nImageWidth*nImageHeight];
		ptrG = new char[nImageWidth*nImageHeight];
		ptrB = new char[nImageWidth*nImageHeight];
		RGBToGray(RGB32, ptrR, ptrG, ptrB, nImageWidth, nImageHeight, 0);
		GenImage3(hoGrabberImage, "byte", nImageWidth, nImageHeight,
			ptrB, ptrG, ptrR);
		delete[]ptrR;
		delete[]ptrG;
		delete[]ptrB;
		ptrR = NULL;
		ptrG = NULL;
		ptrB = NULL;

	}
	else if (1 == nPixelSize)
	{
		GenImage1(hoGrabberImage, "byte", nImageWidth, nImageHeight, nAddr);
	}
	else if (3 == nPixelSize)
	{
		GenImage1(hoGrabberImage, "byte", nImageWidth, nImageHeight, nAddr);
		CfaToRgb(*hoGrabberImage, hoGrabberImage, "bayer_rg", "bilinear");
	}

	if (0 == nGrabberType)//�����ģ��
	{
		MirrorImage(*hoGrabberImage, hoGrabberImage, "row");

	}
	if (3 == nGrabberType && 0 == nGrabberSN)
	{
		MirrorImage(*hoGrabberImage, hoGrabberImage, "column");
	}

	if (1 == g_tSystemSet.bMirrorRow)
	{
		MirrorImage(*hoGrabberImage, hoGrabberImage, "row");
	}
}
