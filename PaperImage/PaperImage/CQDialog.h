#pragma once

// CCQDialog 대화 상자

class CCQDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CCQDialog)

public:
	CCQDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCQDialog();

	CWnd* m_pWnd;

	PointF makeStartPoint(CCQArea* pArea, int w, int h);
	PointF makeStartPoint(vector<CCQArea*>& vec, int w, int h);
	CCQArea* findNearestArea(vector<CCQArea*>& vec, PointF& tp);

	BOOL isDrawObject(CCQObject* pObj);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCQDIALOG };
#endif

private:
	void setCountVariable();
	void setDrawOptions();

	void ClickedCheck1(int type);
	void ClickedCheck2(int type, int spd);

	void InvalidateView();

	BOOL* m_bool1[4];		// RAO, RSVO, RFVO, RSMO
	BOOL* m_bool2[4][3];	// RLSO, RMSO, RHSO

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_CQTYPE;
	int m_NO_COUNT;			// Number of moving objects

	int m_REO_PERCENT;		// 30 % The enemy proportion of the moving objects
	int m_REO_COUNT;

	int m_RFO_PERCENT;		// 70 % Moving object's friendly percentage
	int m_RFO_COUNT;

	int m_RLSO_PERCENT;		// 0.4	Slow speed ratio of moving object
	int m_RMSO_PERCENT;		// 0.59	Mid speed ratio of moving object
	int m_RHSO_PERCENT;		// 0.01	High speed ratio of moving object

	int m_REOCQ_PERCENT;	// 15 % CQ proximity movement rate of enemy moving objects
	int m_REOCQ_COUNT;
	int m_REONCQ_PERCENT;	// 85 % CQ Independent Movement Rate of Enemy Moving Objects
	int m_REONCQ_COUNT;

	int m_RFOCQ_PERCENT;	// 15 % CQ proximity movement rate of enemy moving objects
	int m_RFOCQ_COUNT;
	int m_RFONCQ_PERCENT;	// 85 % CQ Independent Movement Rate of Enemy Moving Objects
	int m_RFONCQ_COUNT;

	int m_RAO_PERCENT;		// 10 % Aerial proportion of moving objects
	int m_RAO_REO_COUNT;
	int m_RAO_RFO_COUNT;
	int m_RAO_REO_RLSO;
	int m_RAO_REO_RMSO;
	int m_RAO_REO_RHSO;
	int m_RAO_RFO_RLSO;
	int m_RAO_RFO_RMSO;
	int m_RAO_RFO_RHSO;
	int m_RSVO_PERCENT;		// 20 % Percentage of moving objects to surface naval vessels
	int m_RSVO_REO_COUNT;
	int m_RSVO_RFO_COUNT;
	int m_RSVO_REO_RLSO;
	int m_RSVO_REO_RMSO;
	int m_RSVO_REO_RHSO;
	int m_RSVO_RFO_RLSO;
	int m_RSVO_RFO_RMSO;
	int m_RSVO_RFO_RHSO;
	int m_RFVO_PERCENT;		// 65 % Fishing vessel ratio of moving objects
	int m_RFVO_REO_COUNT;
	int m_RFVO_RFO_COUNT;
	int m_RFVO_REO_RLSO;
	int m_RFVO_REO_RMSO;
	int m_RFVO_REO_RHSO;
	int m_RFVO_RFO_RLSO;
	int m_RFVO_RFO_RMSO;
	int m_RFVO_RFO_RHSO;
	int m_RSMO_PERCENT;		// 5 % Submarine percentage of moving objects
	int m_RSMO_REO_COUNT;
	int m_RSMO_RFO_COUNT;
	int m_RSMO_REO_RLSO;
	int m_RSMO_REO_RMSO;
	int m_RSMO_REO_RHSO;
	int m_RSMO_RFO_RLSO;
	int m_RSMO_RFO_RMSO;
	int m_RSMO_RFO_RHSO;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedClose();
protected:
	afx_msg LRESULT OnAddobject(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEndobject(WPARAM wParam, LPARAM lParam);
public:
	BOOL m_bALL;
	BOOL m_bREO;
	BOOL m_bRFO;
	BOOL m_bRAO;
	BOOL m_bRAO_RLSO;
	BOOL m_bRAO_RMSO;
	BOOL m_bRAO_RHSO;
	BOOL m_bRSVO;
	BOOL m_bRSVO_RLSO;
	BOOL m_bRSVO_RMSO;
	BOOL m_bRSVO_RHSO;
	BOOL m_bRFVO;
	BOOL m_bRFVO_RLSO;
	BOOL m_bRFVO_RMSO;
	BOOL m_bRFVO_RHSO;
	BOOL m_bRSMO;
	BOOL m_bRSMO_RLSO;
	BOOL m_bRSMO_RMSO;
	BOOL m_bRSMO_RHSO;
	BOOL m_bDrawPath;
	afx_msg void OnClickedCheckAll();
	afx_msg void OnClickedCheckRao();
	afx_msg void OnClickedCheckRsvo();
	afx_msg void OnClickedCheckRfvo();
	afx_msg void OnClickedCheckRsmo();
	afx_msg void OnClickedCheckRaoRlso();
	afx_msg void OnClickedCheckRaoRmso();
	afx_msg void OnClickedCheckRaoRhso();
	afx_msg void OnClickedCheckRsvoRlso();
	afx_msg void OnClickedCheckRsvoRmso();
	afx_msg void OnClickedCheckRsvoRhso();
	afx_msg void OnClickedCheckRfvoRlso();
	afx_msg void OnClickedCheckRfvoRmso();
	afx_msg void OnClickedCheckRfvoRhso();
	afx_msg void OnClickedCheckRsmoRlso();
	afx_msg void OnClickedCheckRsmoRmso();
	afx_msg void OnClickedCheckRsmoRhso();
	afx_msg void OnClickedCheckReo();
	afx_msg void OnClickedCheckRfo();
	afx_msg void OnClickedCheckPath();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
};
