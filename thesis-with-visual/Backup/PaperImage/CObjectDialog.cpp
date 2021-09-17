// CObjectDialog.cpp : implementation file
//

#include "pch.h"
#include "PaperImage.h"
#include "CObjectDialog.h"
#include "afxdialogex.h"


// CObjectDialog dialog

IMPLEMENT_DYNAMIC(CObjectDialog, CDialogEx)

CObjectDialog::CObjectDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CCQDIALOG1, pParent)
{

}

CObjectDialog::~CObjectDialog()
{
}

void CObjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CObjectDialog, CDialogEx)
END_MESSAGE_MAP()


// CObjectDialog message handlers
