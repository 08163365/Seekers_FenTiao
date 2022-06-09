#pragma once

#include "ChartCtrl.h"
#include "ColourPicker.h"
#include "ChartLineSerie.h"
#include "ChartPointsSerie.h"
// CDlgScatterPlot �Ի���

class CDlgScatterPlot : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgScatterPlot)

public:
	CDlgScatterPlot(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgScatterPlot();

	// �Ի�������
	enum { IDD = IDD_DIALOG_SCATTERPLOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void  InsertData(C_ALLOCATORWORKSHOPOUTPUT& tOutput);
	void ResetSize();
	CChartCtrl m_ChartCtrl;
	CChartXYSerie* m_pSeries;
	CChartPointsSerie* m_pPointsSeries;
	virtual BOOL OnInitDialog();


};