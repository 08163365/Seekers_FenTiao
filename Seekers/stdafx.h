
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��
#include <vector>
#include <list>
#include <queue>
#include <exception>
#include <afxdlgs.h>


//excel
#include "CApplication.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "CShapes.h"
/////////////////////////////////



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


/////////�Զ���ͷ�ļ�
#include <iostream>  
#include <string>  
#include <vector>  
#include <fstream>  
#include <sstream> 
using namespace std;
#include "DHWidgets.h"
#include "ADOConnect.h"
#include "DHInterFace.h"
#include "Include/Define.h"
#include "SCISeparate.in.h"
#include "DH_HalconEx.h"
#include "Judge.in.h"
#include "PowerLimit.h"

#include "SCIDetect.in.h"
#include "SCIDataFlow.h"
#include "Upload.h"
#include "InkjetProcess.h"
#include "SpotSizeCheck.h"
#include "AutoGRR.h"
#include "SCICommunicate.in.h"
#include "SCILabeller.in.h"
#include "SCIStorage.in.h"
#include "SCIFindMark.in.h"
#include "Judge.h"
#include "Find.h"
//#include "DHWidgets.h"
#include "FinsTCPExt.h"
#include "SCIGrabber.in.h"
#include "GlobalVariables.h"
#include "LightControl.in.h"
#include "DlgCreateBaseLine.h"

