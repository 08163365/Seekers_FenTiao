#pragma once

#include "ChartCtrl.h"
#include "ColourPicker.h"
#include "ChartLineSerie.h"
#include "ChartPointsSerie.h"
// CDlgScatterPlot 对话框

class CDlgScatterPlot : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgScatterPlot)

public:
	CDlgScatterPlot(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgScatterPlot();

	// 对话框数据
	enum { IDD = IDD_DIALOG_SCATTERPLOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void  InsertData(C_ALLOCATORWORKSHOPOUTPUT& tOutput);
	void ResetSize();
	CChartCtrl m_ChartCtrl;
	CChartXYSerie* m_pSeries;
	CChartPointsSerie* m_pPointsSeries;
	virtual BOOL OnInitDialog();


};