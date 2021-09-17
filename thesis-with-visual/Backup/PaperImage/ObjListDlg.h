#pragma once


// ObjListDlg dialog

class ObjListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ObjListDlg)

public:
	ObjListDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ObjListDlg();

	CWnd* m_pWnd;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJLISTDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listCtrl;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	void initObjects();

	int currObjFocused = -1;
};
