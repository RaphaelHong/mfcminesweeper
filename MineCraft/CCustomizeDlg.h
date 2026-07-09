#pragma once


// CCustomizeDlg 对话框

class CCustomizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomizeDlg)

public:
	CCustomizeDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCustomizeDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CUSTOM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    int _rows;
    int _columns;
    int _mines;
};
