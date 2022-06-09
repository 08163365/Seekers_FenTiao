// DlgScatterPlot.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgScatterPlot.h"
#include "afxdialogex.h"

#include "ChartBarSerie.h"
#include "ChartLabel.h"
#include "ChartPointsSerie.h"
#include "ChartAxisLabel.h"
#include "ChartStandardAxis.h"
#include "ChartDateTimeAxis.h"

#include "ChartCrossHairCursor.h"
#include "ChartDragLineCursor.h"
#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
// CDlgScatterPlot 对话框

IMPLEMENT_DYNAMIC(CDlgScatterPlot, CDialogEx)

	CDlgScatterPlot::CDlgScatterPlot(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgScatterPlot::IDD, pParent)
{

}

CDlgScatterPlot::~CDlgScatterPlot()
{
}

void CDlgScatterPlot::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);
}


BEGIN_MESSAGE_MAP(CDlgScatterPlot, CDialogEx)
END_MESSAGE_MAP()


// CDlgScatterPlot 消息处理程序

void CDlgScatterPlot::ResetSize()
{
	CRect rcSize,rcPicture;
	this->GetWindowRect(&rcSize);
	ScreenToClient(rcSize);
	rcPicture=rcSize;
	GetDlgItem(IDC_CHARTCTRL)->MoveWindow(&rcPicture);

}
BOOL CDlgScatterPlot::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CChartStandardAxis* pBottomAxis = 
		m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pBottomAxis->SetMinMax(0, 10);
	CChartStandardAxis* pLeftAxis =m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);

	pLeftAxis->SetMinMax(-1, MIN(g_tSystemSet.nLaneCount+1,MAX_LANE+1));
	pLeftAxis->SetTickIncrement(false,1);
	pLeftAxis->SetPanZoomEnabled(false);
	pBottomAxis->SetPanZoomEnabled(false);
	m_ChartCtrl.GetLeftAxis()->GetGrid()->SetVisible(true);
	m_ChartCtrl.GetBottomAxis()->GetGrid()->SetVisible(false);
	COLORREF BackColor =RGB(255,255,255);
	m_ChartCtrl.SetBackColor(BackColor);
	COLORREF Color =255;
	m_pPointsSeries = m_ChartCtrl.CreatePointsSerie(0, 0);
	m_pSeries = m_pPointsSeries;
	m_pPointsSeries->SetPointType(CChartPointsSerie::ptEllipse);
	m_pPointsSeries->SetPointSize(10,10);
	m_pSeries->SetColor(Color);
	m_ChartCtrl.GetBottomAxis()->GetLabel()->SetText("运动方向米数(mm)");
	m_ChartCtrl.GetLeftAxis()->GetLabel()->SetText("条数分区");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void  CDlgScatterPlot::InsertData(C_ALLOCATORWORKSHOPOUTPUT& tOutput)
{


	CChartAxis* pAxis = m_ChartCtrl.GetBottomAxis();
	pAxis->SetMinMax(0,MAX(1,g_dwYield));

	HTuple hv_BelongTrip;
	hv_BelongTrip.Append(tOutput.B[0].tDetectOutput.hv_ErrorBelongStrip);
	hv_BelongTrip.Append(tOutput.B[1].tDetectOutput.hv_ErrorBelongStrip);
	HTuple hv_Index;

	for (int i=0;i<MIN(g_tSystemSet.nLaneCount,MAX_LANE);i++)
	{
		TupleFind(hv_BelongTrip,i,&hv_Index);//条号

		if (hv_Index.Length()>0 &&hv_Index!=-1)
		{
			g_hvXTuple.Append(g_dwYield);
			g_hvYTuple.Append(i);

		}

	}
	int NumberPoints=g_hvXTuple.Length();
	double* XValues = new double[NumberPoints];
	double* YValues = new double[NumberPoints];
	for (int i=0;i<NumberPoints;i++)
	{
		XValues[i] =g_hvXTuple[i].D();
		YValues[i] = g_hvYTuple[i].D();
	}
	m_pSeries->SetPoints(XValues,YValues,NumberPoints);
	delete[] XValues;
	delete[] YValues;
	m_ChartCtrl.RefreshCtrl();
}