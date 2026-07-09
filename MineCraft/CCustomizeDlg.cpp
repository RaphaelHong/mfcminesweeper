// CCustomizeDlg.cpp: 实现文件
//

#include "pch.h"
#include "MineCraft.h"
#include "CCustomizeDlg.h"
#include "afxdialogex.h"


// CCustomizeDlg 对话框

IMPLEMENT_DYNAMIC(CCustomizeDlg, CDialogEx)

CCustomizeDlg::CCustomizeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CUSTOM, pParent)
    , _rows(0)
    , _columns(0)
    , _mines(0)
{

}

CCustomizeDlg::~CCustomizeDlg()
{
}

void CCustomizeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_ROW, _rows);
    DDX_Text(pDX, IDC_EDIT_COLUMN, _columns);
    DDX_Text(pDX, IDC_EDIT_MINE_NUM, _mines);
}


BEGIN_MESSAGE_MAP(CCustomizeDlg, CDialogEx)
END_MESSAGE_MAP()


// CCustomizeDlg 消息处理程序
