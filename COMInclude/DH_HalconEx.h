/*
   作者：李博
   主要记录一些常用的函数
   版本号：13.0.0.502
*/
#include "HalconCpp.h"
#include "HDevThread.h"
using namespace HalconCpp;
void dev_open_window_fit_image (HObject ho_Image, HTuple hv_Row, HTuple hv_Column, 
    HTuple hv_WidthLimit, HTuple hv_HeightLimit, HTuple *hv_WindowHandle);
void dev_open_window_fit_image (HObject ho_Image,const CStatic &IpWinStatic, HTuple *hv_WindowHandle);
void disp_continue_message (HTuple hv_WindowHandle, HTuple hv_Color, HTuple hv_Box);
void disp_message (HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem, 
    HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box);
void set_display_font (HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold, 
    HTuple hv_Slant);
void TileImages (HObject ho_Images, HObject *ho_TiledImage);
void list_image_files (HTuple hv_ImageDirectory, HTuple hv_Extensions, HTuple hv_Options, 
	HTuple *hv_ImageFiles);
void FastSortFileNames (HTuple hv_DirName, HTuple *hv_SortedFileNames);
void TupleExceptRepeat(const HTuple& src,HTuple* dst);
void DeleteOldFileFolder(CString& strPath,int nSaveDays);
void parse_filename (HTuple hv_FileName, HTuple *hv_BaseName, HTuple *hv_Extension, 
	HTuple *hv_Directory);
void TupleCountExceptRepeat(const HTuple& srcErrorType,const HTuple& srcBelongStrip,HTuple* dstErrorType,HTuple* dstBelongStrip);
void AdaptiveSize(HWND hWnd);//自适应UI
///////////////////////////////////////配置文件读写///////////////////////////////////
CString GetPrivateProfileCString(CString lpAppName,LPCTSTR lpKeyName,CString lpDefault,LPCTSTR lpFileName);
void WritePrivateProfileInt(CString lpAppName,CString lpKeyName,int Value, CString lpFileName);
int GetPrivateProfileIntH(CString lpAppName,CString lpKeyName,double Value, CString lpFileName);
void WritePrivateProfileDouble(CString lpAppName,CString lpKeyName,double Value, CString lpFileName);
double GetPrivateProfileDouble(LPCTSTR lpAppName, LPCTSTR lpKeyName, double nDefault, LPCTSTR lpFileName);
CString GetCurrentAppPath();
BOOL IsDirectory(CString pstrPath);
BOOL CopyFolder(CString pstrSrcFolder, CString pstrDstFolder);
BOOL DeleteFolder(CString pstrFolder);
BOOL CreateMultilevelPath(CString pstrFolder);
int GetKeyValues(CStringArray& arrKey,CStringArray& arrValue, LPCTSTR lpSection,LPCTSTR lpfileName);
void TileImages (HObject ho_Images, HObject *ho_TiledImage,HTuple TileOrder);
void DeleteOldCache(CString strDir,int nSaveDays);//缓存天数
CString  GetGongWeiName(CString& FalgStr,int nSide);//获取工位名
/////////////////////////////////////加载动态库/////////////////////////
DHInterface* LoadDHInterFaceDll(CString strDllName);
//////////////////////////////////////////////////////////////////////////
void FullScreenAdaptive(HWND hWnd);
void AdaptiveSize(HWND hWnd, BOOL bAdaptive);
void FullScreen(HWND hWnd);
BOOL DirectoryExist(CString& Path);