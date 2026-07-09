#pragma once



// CGameStatusView 窗体视图

class CGameStatusView : public CFormView
{
	DECLARE_DYNCREATE(CGameStatusView)

protected:
	CGameStatusView();           // 动态创建所使用的受保护的构造函数
	virtual ~CGameStatusView();

    Gdiplus::Image *_num_img[10]{};
    Gdiplus::Image *_emoji_img[5]{};

    int _led_time_index[3]{};
    int _led_mine_count[3]{};
    int _emo_index{};

    CRect _led_time_rect;
    CRect _led_mine_rect;
    CRect _emo_rect;

    int _mines_left{0}; //剩余雷数

    CMFCButton _emoBtn;

    void UpdateLED(int (&led_arr)[3], int num);
    void RecalcLayout();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GAME_STATUS };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT OnMessageGameStart(WPARAM wp, LPARAM lp);
    afx_msg LRESULT OnMessageGameReset(WPARAM wp, LPARAM lp);
    afx_msg LRESULT OnMessageGameWin(WPARAM wp, LPARAM lp);
    afx_msg LRESULT OnMessageGameOver(WPARAM wp, LPARAM lp);
    afx_msg LRESULT OnMessageMineCountUp(WPARAM wp, LPARAM lp);
    afx_msg LRESULT OnMessageMineCountDown(WPARAM wp, LPARAM lp);
    virtual void OnInitialUpdate();
};


