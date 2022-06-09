// DlgProductManager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgProductManager.h"
#include "afxdialogex.h"
#include "DlgNewProductName.h"
#include "SeekersDlg.h"
#include "DlgInput.h"
// CDlgProductManager �Ի���

IMPLEMENT_DYNAMIC(CDlgProductManager, CDialogEx)

CDlgProductManager::CDlgProductManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgProductManager::IDD, pParent)
{

}

CDlgProductManager::~CDlgProductManager()
{
}

void CDlgProductManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_cstaticTitle);
	DDX_Control(pDX, IDC_BUTTON_CREATE, m_btnCreate);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_COPY, m_btnCopy);
	DDX_Control(pDX, IDC_BUTTON_LOAD, m_btnLoad);
	DDX_Control(pDX, IDC_BUTTON_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_listProduct);
	DDX_Control(pDX, IDC_BUTTON_NOTE, m_btnNote);
	DDX_Control(pDX, IDC_STATIC_CURRENT_PRODUCT, m_STATIC_chCurrentProduct);
}


BEGIN_MESSAGE_MAP(CDlgProductManager, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CDlgProductManager::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CDlgProductManager::OnBnClickedButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgProductManager::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CDlgProductManager::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CDlgProductManager::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_NOTE, &CDlgProductManager::OnBnClickedButtonNote)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgProductManager ��Ϣ�������


BOOL CDlgProductManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitStatic();

	InitListCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgProductManager::InitListCtrl()
{
	m_Listfont.CreatePointFont(90,"Microsoft YaHei");

	while(m_listProduct.GetHeaderCtrl()->GetItemCount())
	{
		m_listProduct.DeleteColumn(0);//ɾ��������
	}
	m_listProduct.DeleteAllItems();//ɾ��������
	m_listProduct.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EDITLABELS|LVS_EX_SUBITEMIMAGES);
	m_listProduct.InsertColumn(0,"���",LVCFMT_LEFT,80);
	m_listProduct.InsertColumn(1,"��Ʒ��",LVCFMT_LEFT,180);
	m_listProduct.InsertColumn(2,"ע��",LVCFMT_LEFT,250);

	m_listProduct.SetColorHeader(TRUE);
	m_listProduct.SetHeaderBKColor(255,255,255,0);
	m_listProduct.SetFont(&m_Listfont);
	
	int nCurrentCount = GetPrivateProfileInt("PRODUCT","COUNT",0,g_strConfigSystemFile);//��ȡ��ǰ����
	CString strKey,strValue,strSN;
	CString strNote;
	for (int i=0;i<nCurrentCount;i++)
	{
		strKey.Format("PRODUCT%d",i);
		GetPrivateProfileString("PRODUCT",strKey,0,strValue.GetBuffer(MAX_PATH),MAX_PATH,g_strConfigSystemFile);
		strValue.ReleaseBuffer();
		strSN.Format("%d",i+1);
		int nItemCount = m_listProduct.GetItemCount();
		m_listProduct.InsertItem(nItemCount,0);
		m_listProduct.SetItemText(nItemCount,0,strSN);
		m_listProduct.SetItemText(nItemCount,1,strValue);
		GetPrivateProfileString("PRODUCTNOTE",strValue,"",strNote.GetBuffer(MAX_PATH),MAX_PATH,g_strConfigSystemFile);
		strNote.ReleaseBuffer();
		m_listProduct.SetItemText(nItemCount,2,strNote);
	}
}


void CDlgProductManager::InitStatic()
{
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;

	m_ftButton.CreatePointFont(100,"Microsoft YaHei");

	m_cstaticTitle.SetTextColor(RGB(255,255,255));
	m_cstaticTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticTitle.SetBkColor(MainColor,0,CxStatic::Normal);

	m_STATIC_chCurrentProduct.SetTextColor(RGB(0,0,0));
	m_STATIC_chCurrentProduct.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STATIC_chCurrentProduct.Format("��ǰƷ��:%s",g_strProductName);
	m_STATIC_chCurrentProduct.SetTransparent(TRUE);

	//short	shBtnColor = 0;
	//COLORREF BKOUT(RGB(129,197,81));
	//COLORREF BKIN(RGB(114,182,66));//124,192,76
	//COLORREF BKFOCUS(RGB(104,172,56));
	//COLORREF FG(RGB(255,255,255));

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	//���ñ���ɫ ��7����ť
	m_btnCreate.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnDelete.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnCopy.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnLoad.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnExit.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnNote.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	
	m_btnCreate.SetColor(CButtonST::BTNST_COLOR_BK_IN, BKIN);
	m_btnDelete.SetColor(CButtonST::BTNST_COLOR_BK_IN, BKIN);
	m_btnCopy.SetColor(CButtonST::BTNST_COLOR_BK_IN, BKIN);
	m_btnLoad.SetColor(CButtonST::BTNST_COLOR_BK_IN, BKIN);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_IN, BKIN);
	m_btnNote.SetColor(CButtonST::BTNST_COLOR_BK_IN, BKIN);

	m_btnCreate.SetColor(CButtonST::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnDelete.SetColor(CButtonST::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnCopy.SetColor(CButtonST::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnLoad.SetColor(CButtonST::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnNote.SetColor(CButtonST::BTNST_COLOR_BK_OUT, BKOUT);

	m_btnCreate.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnDelete.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnCopy.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnLoad.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnNote.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, BKOUT);


	m_btnCreate.SetColor(CButtonST::BTNST_COLOR_FG_IN, FG);
	m_btnDelete.SetColor(CButtonST::BTNST_COLOR_FG_IN, FG);
	m_btnCopy.SetColor(CButtonST::BTNST_COLOR_FG_IN, FG);
	m_btnLoad.SetColor(CButtonST::BTNST_COLOR_FG_IN, FG);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_IN, FG);
	m_btnNote.SetColor(CButtonST::BTNST_COLOR_FG_IN, FG);

	m_btnCreate.SetColor(CButtonST::BTNST_COLOR_FG_OUT, FG);
	m_btnDelete.SetColor(CButtonST::BTNST_COLOR_FG_OUT, FG);
	m_btnCopy.SetColor(CButtonST::BTNST_COLOR_FG_OUT, FG);
	m_btnLoad.SetColor(CButtonST::BTNST_COLOR_FG_OUT, FG);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_OUT, FG);
	m_btnNote.SetColor(CButtonST::BTNST_COLOR_FG_OUT, FG);

	m_btnCreate.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, FG);
	m_btnDelete.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, FG);
	m_btnCopy.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, FG);
	m_btnLoad.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, FG);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, FG);
	m_btnNote.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, FG);

	m_btnCreate.SetFont(&m_ftButton);
	m_btnDelete.SetFont(&m_ftButton);
	m_btnCopy.SetFont(&m_ftButton);
	m_btnLoad.SetFont(&m_ftButton);
	m_btnExit.SetFont(&m_ftButton);
	m_btnNote.SetFont(&m_ftButton);



	/*m_btnCreate.SetIcon(IDI_ICON_PRODUCTMANAGER_CREATE);
	m_btnDelete.SetIcon(IDI_ICON_PRODUCTMANAGER_DELETE);
	m_btnCopy.SetIcon(IDI_ICON_PRODUCTMANAGER_COPY);
	m_btnLoad.SetIcon(IDI_ICON_PRODUCTMANAGER_LOAD);
	m_btnExit.SetIcon(IDI_ICON_PRODUCTMANAGER_EXIT);
	m_btnNote.SetIcon(IDI_ICON_PRODUCTMANAGER_NOTE);*/

}

void CDlgProductManager::OnBnClickedButtonExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

void CDlgProductManager::CopyProduct(CString strSrc,CString strDst)
{
	CString strDstFolder,strSrcFolder;
	strSrcFolder.Format("%s//ModelInfo//%s",GetCurrentAppPath(),strSrc);
	strDstFolder.Format("%s//ModelInfo//%s",GetCurrentAppPath(),strDst);
	CopyFolder(strSrcFolder,strDstFolder);

	strSrcFolder.Format("%s//Hole//%s",GetCurrentAppPath(),strSrc);
	strDstFolder.Format("%s//Hole//%s",GetCurrentAppPath(),strDst);
	CopyFolder(strSrcFolder,strDstFolder);

	strSrcFolder.Format("%s//FindMark//%s",GetCurrentAppPath(),strSrc);
	strDstFolder.Format("%s//FindMark//%s",GetCurrentAppPath(),strDst);
	CopyFolder(strSrcFolder,strDstFolder);

	TCHAR chAllKeysAndValues[1000];
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//���������ļ�Ϊ��д��ģʽ
	GetPrivateProfileSection(strSrc,chAllKeysAndValues,300,g_strConfigSystemFile);
	WritePrivateProfileSection(strDst,chAllKeysAndValues,g_strConfigSystemFile);
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_READONLY);//���������ļ�Ϊֻ��ģʽ

}
void CDlgProductManager::DeleteProduct(CString strName)
{
	//д���ļ�
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//���������ļ�Ϊ��д��ģʽ
	WritePrivateProfileString("PRODUCTNOTE",strName,NULL,g_strConfigSystemFile);//ɾ����ע
	WritePrivateProfileSection("PRODUCT",0,g_strConfigSystemFile);//����ü�ֵ
	WritePrivateProfileSection(strName,0,g_strConfigSystemFile);
	int nCurrentCount = m_listProduct.GetItemCount();
	WritePrivateProfileInt("PRODUCT","COUNT",nCurrentCount,g_strConfigSystemFile);//��ȡ��ǰ����
	for(int i=0;i<nCurrentCount;i++)
	{
		CString strProductName = m_listProduct.GetItemText(i,1);
		CString strKey;
		strKey.Format("PRODUCT%d",i);
		WritePrivateProfileString("PRODUCT",strKey,strProductName,g_strConfigSystemFile);
	}
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_READONLY);//���������ļ�Ϊֻ��ģʽ

	CString strFolder;
	strFolder.Format("%s//ModelInfo//%s",GetCurrentAppPath(),strName);
	DeleteFolder(strFolder);
	strFolder.Format("%s//Hole//%s",GetCurrentAppPath(),strName);
	DeleteFolder(strFolder);
	strFolder.Format("%s//FindMark//%s",GetCurrentAppPath(),strName);
	DeleteFolder(strFolder);
}

void CDlgProductManager::CreateNewProduct(CString strNewProductName)
{
	int nCurrentCount = GetPrivateProfileInt("PRODUCT","COUNT",0,g_strConfigSystemFile);//��ȡ��ǰ����
	CString strKey;
	strKey.Format("PRODUCT%d",nCurrentCount);
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//���������ļ�Ϊ��д��ģʽ
	WritePrivateProfileString("PRODUCT",strKey,strNewProductName,g_strConfigSystemFile);
	nCurrentCount++;
	WritePrivateProfileInt("PRODUCT","COUNT",nCurrentCount,g_strConfigSystemFile);
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_READONLY);//���������ļ�Ϊֻ��ģʽ
}
void CDlgProductManager::OnBnClickedButtonCreate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgNewProductName dlg;
	if (IDOK == dlg.DoModal())
	{
		CString strNew;
		strNew.Format("%s",dlg.m_strNewProduct);
		CreateNewProduct(strNew);
		CopyProduct(g_strProductName,strNew);
		InitListCtrl();
	}
}


void CDlgProductManager::OnBnClickedButtonDelete()
{

	if(0 == m_listProduct.GetFirstSelectedItemPosition())//���ûѡ���κ�Ʒ��,����
	{
		return;
	}
	CString strDeleteProName = "";
	while(m_listProduct.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED) != -1)
	{
		int nItem = m_listProduct.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
		strDeleteProName = m_listProduct.GetItemText(nItem,1);
		if (g_strProductName == strDeleteProName)
		{
			return;
		}
		m_listProduct.DeleteItem(nItem);
	}
	DeleteProduct(strDeleteProName);
	InitListCtrl();
}


void CDlgProductManager::OnBnClickedButtonCopy()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_listProduct.GetFirstSelectedItemPosition();
	if(0 ==pos)//���ûѡ���κ�Ʒ��,����
	{
		return;
	}
	CString strSelProductName,strNew;
	strSelProductName = m_listProduct.GetItemText(m_listProduct.GetNextSelectedItem(pos),1);
	strNew = strSelProductName + "����";
	CreateNewProduct(strNew);
	CopyProduct(g_strProductName,strNew);
	InitListCtrl();
}


void CDlgProductManager::OnBnClickedButtonLoad()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_listProduct.GetFirstSelectedItemPosition();
	if(0 ==pos)//���ûѡ���κ�Ʒ��,����
	{
		return;
	}
	CString strNewProduct = m_listProduct.GetItemText(m_listProduct.GetNextSelectedItem(pos),1);
	CString strMessage;
	strMessage.Format("ȷ���л�Ʒ�ֵ�:%s  Ӧ�ó��򽫻�����",strNewProduct);
	if (IDYES == AfxMessageBox(strMessage,MB_YESNO))
	{
		g_strProductName = strNewProduct;
		((CSeekersDlg*)g_pMainDialog)->PostMessageA(WM_RESTARTPROCESS,0,0);
	}

}


void CDlgProductManager::OnBnClickedButtonNote()
{
	
	POSITION pos = m_listProduct.GetFirstSelectedItemPosition();
	if(0 ==pos)//���ûѡ���κ�Ʒ��,����
	{
		return;
	}
	int nSelItem = m_listProduct.GetNextSelectedItem(pos);
	if (-1!=nSelItem)
	{
		CString strSelProductName;
		m_listProduct.GetItemText(nSelItem,1,strSelProductName.GetBuffer(MAX_PATH),MAX_PATH);
		CDlgInput dlg;
		dlg.m_strTitle.Format("������%s�ı�ע",strSelProductName);
		if (IDOK == dlg.DoModal())
		{
			SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//���������ļ�Ϊ��д��ģʽ
			if (""== dlg.m_strInput)
			{
				WritePrivateProfileString("PRODUCTNOTE",strSelProductName,NULL,g_strConfigSystemFile);
			}else
			{
				WritePrivateProfileString("PRODUCTNOTE",strSelProductName,dlg.m_strInput,g_strConfigSystemFile);
			}
			InitListCtrl();
			SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_READONLY);//���������ļ�Ϊֻ��ģʽ
		}
	}
	


}


void CDlgProductManager::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect   rcDlg,rcTitle,rcClient;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	GetDlgItem(IDC_STATIC_TITLE)->GetClientRect(rcTitle); 
	rcClient = rcDlg;
	rcClient.top = rcTitle.bottom;
	dc.FillSolidRect(rcClient,RGB(255,255,255));

	//���߿���
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(200,200,200));//154,222,106
	dc.SelectObject(pen);
	dc.MoveTo(rcDlg.left,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.top);
}


HBRUSH CDlgProductManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


BOOL CDlgProductManager::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_MOUSEMOVE && pMsg->wParam & MK_LBUTTON)
	{
		CRect rcTitle;
		GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
		if (rcTitle.PtInRect(pMsg->pt))
		{
			PostMessage(WM_NCLBUTTONDOWN, HTCAPTION,  pMsg->lParam);
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgProductManager::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}
