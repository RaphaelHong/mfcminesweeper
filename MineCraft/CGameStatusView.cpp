// CGameStatusView.cpp: 实现文件
//

#include "pch.h"
#include "MineCraft.h"
#include "CGameStatusView.h"
#include "utils.h"
#include "GameCore.h"


using namespace Gdiplus;


#define LED_0 (0)
#define LED_1 (1)
#define LED_2 (2)
#define LED_3 (3)
#define LED_4 (4)
#define LED_5 (5)
#define LED_6 (6)
#define LED_7 (7)
#define LED_8 (8)
#define LED_9 (9)

#define EMO_COOL    (0)
#define EMO_SAD     (1)
#define EMO_SMILE_E (2) //emboss
#define EMO_SMILE_F (3) //flat
#define EMO_SURP    (4) //surprise

#define TIMER_ELAPLSE (1000)


// CGameStatusView

IMPLEMENT_DYNCREATE(CGameStatusView, CFormView)

CGameStatusView::CGameStatusView()
	: CFormView(IDD_DIALOG_GAME_STATUS)
{
    _num_img[0] = new Gdiplus::Image(L"res/LED_0.png");
    _num_img[1] = new Gdiplus::Image(L"res/LED_1.png");
    _num_img[2] = new Gdiplus::Image(L"res/LED_2.png");
    _num_img[3] = new Gdiplus::Image(L"res/LED_3.png");
    _num_img[4] = new Gdiplus::Image(L"res/LED_4.png");
    _num_img[5] = new Gdiplus::Image(L"res/LED_5.png");
    _num_img[6] = new Gdiplus::Image(L"res/LED_6.png");
    _num_img[7] = new Gdiplus::Image(L"res/LED_7.png");
    _num_img[8] = new Gdiplus::Image(L"res/LED_8.png");
    _num_img[9] = new Gdiplus::Image(L"res/LED_9.png");

    _emoji_img[0] = new Gdiplus::Image(L"res/cool_emboss.png");
    _emoji_img[1] = new Gdiplus::Image(L"res/sad_emboss.png");
    _emoji_img[2] = new Gdiplus::Image(L"res/smile_emboss.png");
    _emoji_img[3] = new Gdiplus::Image(L"res/smile_flat.png");
    _emoji_img[4] = new Gdiplus::Image(L"res/surprise_emboss.png");
    
}

CGameStatusView::~CGameStatusView()
{
    for (auto &i : _num_img) {
        SAFE_DEL(i);
    }

    for (auto &i : _emoji_img) {
        SAFE_DEL(i);
    }
}

void CGameStatusView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameStatusView, CFormView)
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_WM_CREATE()
    ON_MESSAGE(WM_USER_GAME_START, OnMessageGameStart)
    ON_MESSAGE(WM_USER_MINE_COUNT_UP, OnMessageMineCountUp)
    ON_MESSAGE(WM_USER_MINE_COUNT_DOWN, OnMessageMineCountDown)
    ON_MESSAGE(WM_USER_GAME_RESET, OnMessageGameReset)
	ON_MESSAGE(WM_USER_GAME_OVER, OnMessageGameOver)
	ON_MESSAGE(WM_USER_GAME_WIN, OnMessageGameWin)
END_MESSAGE_MAP()


// CGameStatusView 诊断

#ifdef _DEBUG
void CGameStatusView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CGameStatusView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGameStatusView 消息处理程序


void CGameStatusView::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: 在此处添加消息处理程序代码
                       // 不为绘图消息调用 CFormView::OnPaint()

    CRect rc;
    GetClientRect(&rc);
   
    Gdiplus::Graphics gp(dc);
    Gdiplus::SolidBrush br(Gdiplus::Color(255, 204, 204, 204));
    gp.FillRectangle(&br, Gdiplus::Rect(rc.left, rc.top, rc.Width(), rc.Height()));

    
    Gdiplus::Rect rcTime, rcMineNum, rcEmoji;
    int numW = (_num_img[0] != nullptr ? _num_img[0]->GetWidth() : 13);
    int numH = (_num_img[0] != nullptr ? _num_img[0]->GetHeight() : 23);
    rcTime.X = rc.left + 2;
    rcTime.Y = rc.top + (rc.Height() - numH)/2;
    rcTime.Width = numW;
    rcTime.Height = numH;

    rcMineNum.X = rc.right - 2 - 3 * numW;
    rcMineNum.Y = rc.top + (rc.Height() - numH)/2;
    rcMineNum.Width = numW;
    rcMineNum.Height = numH;

    int emoW = (_emoji_img[0] != nullptr ? _emoji_img[0]->GetWidth() : 26);
    int emoH = (_emoji_img[0] != nullptr ? _emoji_img[0]->GetHeight() : 26);
    rcEmoji.X = (rc.Width() - 4 - numW*6 - emoW) / 2 + 2 + numW*3;
    rcEmoji.Y = (rc.Height() - emoH) / 2;
    rcEmoji.Width = emoW;
    rcEmoji.Height = emoH;

    if (_num_img[0] != nullptr) {
        gp.DrawImage(_num_img[_led_time_index[2]], rcTime);
        rcTime.Offset(numW, 0);
        gp.DrawImage(_num_img[_led_time_index[1]], rcTime);
        rcTime.Offset(numW, 0);
        gp.DrawImage(_num_img[_led_time_index[0]], rcTime);

        gp.DrawImage(_num_img[_led_mine_count[2]], rcMineNum);
        rcMineNum.Offset(numW, 0);
        gp.DrawImage(_num_img[_led_mine_count[1]], rcMineNum);
        rcMineNum.Offset(numW, 0);
        gp.DrawImage(_num_img[_led_mine_count[0]], rcMineNum);
    }

    if (_emoji_img[0] != nullptr) {
        gp.DrawImage(_emoji_img[_emo_index], rcEmoji);
    }
}


void CGameStatusView::UpdateLED(int(&led_arr)[3],int num)
{
    if (num < 0)
    {
        led_arr[0] = led_arr[1] = led_arr[2] = LED_0;
        return;
    }

    if (num > 999)
    {
        led_arr[0] = led_arr[1] = led_arr[2] = LED_9;
        return;
    }

    int n = 0;
    for (int i = 0; i < 3; i++) {
        n = num % 10;
        num = num / 10;
        led_arr[i] = LED_0 + n;
    }
}


void CGameStatusView::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent) {
    case TIMER_ELAPLSE: {
        auto t = GameCore::GetInstance()->GameController().TimeElapsed();
        UpdateLED(_led_time_index, t);
        InvalidateRect(_led_time_rect);
        break;
    }
    default: break;
    }

    CFormView::OnTimer(nIDEvent);
}


int CGameStatusView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFormView::OnCreate(lpCreateStruct) == -1)
        return -1;

    //_emoBtn.Create(nullptr, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, 0, 26,26), this, IDC_BUTTON_EMOJ);
    //_emoBtn.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_COOL));

    return 0;
}

void CGameStatusView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    // TODO: 在此添加专用代码和/或调用基类

    RecalcLayout();
}


void CGameStatusView::RecalcLayout()
{
    CRect rcCli;
    GetClientRect(&rcCli);

    int numW = (_num_img[LED_0] != nullptr ? _num_img[0]->GetWidth() : 13);
    int numH = (_num_img[LED_0] != nullptr ? _num_img[0]->GetHeight() : 23);

    _led_time_rect.left = 2;
    _led_time_rect.top = 0;
    _led_time_rect.right = _led_time_rect.left + (_num_img[LED_0] != nullptr ? _num_img[LED_0]->GetWidth() : 13) * 3;
    _led_time_rect.bottom = rcCli.bottom;

    _led_mine_rect.left = rcCli.Width() - 2 - numW * 3;
    _led_mine_rect.top = 0;
    _led_mine_rect.right = _led_mine_rect.left + numW * 3;
    _led_mine_rect.bottom = rcCli.bottom;

    int emoW = (_emoji_img[0] != nullptr ? _emoji_img[0]->GetWidth() : 26);
    int emoH = (_emoji_img[0] != nullptr ? _emoji_img[0]->GetHeight() : 26);
    _emo_rect.left = (rcCli.Width() - 4 - numW * 6 - emoW) / 2 + 2 + numW * 3;
    _emo_rect.top = (rcCli.Height() - emoH) / 2;
    _emo_rect.right = _emo_rect.left + emoW;
    _emo_rect.bottom = _emo_rect.top + emoH;
}



LRESULT CGameStatusView::OnMessageGameStart(WPARAM wp, LPARAM lp)
{
    SetTimer(TIMER_ELAPLSE, 1000, NULL);

    return 0;
}

LRESULT CGameStatusView::OnMessageMineCountUp(WPARAM wp, LPARAM lp)
{
    _mines_left--;
    UpdateLED(_led_mine_count, _mines_left);
    InvalidateRect(_led_mine_rect);

    return 0;
}

LRESULT CGameStatusView::OnMessageMineCountDown(WPARAM wp, LPARAM lp)
{
    _mines_left++;
    UpdateLED(_led_mine_count, _mines_left);
    InvalidateRect(_led_mine_rect);

    return 0;
}


LRESULT CGameStatusView::OnMessageGameReset(WPARAM wp, LPARAM lp)
{
    RecalcLayout();

    _mines_left = GetGameCore->GetTotalMines();
    UpdateLED(_led_mine_count, _mines_left);
    InvalidateRect(_led_mine_rect);

    UpdateLED(_led_time_index, 0);
    InvalidateRect(_led_time_rect);

    _emo_index = EMO_SMILE_E;
	InvalidateRect(_emo_rect);

    KillTimer(TIMER_ELAPLSE);

    return 0;
}


LRESULT CGameStatusView::OnMessageGameWin(WPARAM wp, LPARAM lp)
{
    KillTimer(TIMER_ELAPLSE);
    _emo_index = EMO_COOL;
    InvalidateRect(_emo_rect);

    return 0;
}

LRESULT CGameStatusView::OnMessageGameOver(WPARAM wp, LPARAM lp)
{
	KillTimer(TIMER_ELAPLSE);
	_emo_index = EMO_SAD;
	InvalidateRect(_emo_rect);

    return  0;
}