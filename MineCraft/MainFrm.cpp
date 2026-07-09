
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "MineCraft.h"

#include "MainFrm.h"
#include "MineCraftDoc.h"
#include "GameCore.h"
#include "CGameStatusView.h"
#include "MineCraftView.h"
#include "CCustomizeDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int FRAME_BORD_H = 19;
const int FRAME_BORD_V = 81;
const int STATUS_VIEW_HEI = 38;

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
    ON_COMMAND(ID_DIFFICULTY_EASY, &CMainFrame::OnDifficultyEasy)
    ON_COMMAND(ID_DIFFICULTY_MIDDLE, &CMainFrame::OnDifficultyMiddle)
    ON_COMMAND(ID_DIFFICULTY_EXPERT, &CMainFrame::OnDifficultyExpert)
    ON_COMMAND(ID_DIFFICULTY_CUSTOMIZE, &CMainFrame::OnDifficultyCustomize)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	SetWindowText(_T("扫雷"));

    CenterWindow();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{	
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式 

	cs.style = WS_OVERLAPPED | WS_CAPTION  | WS_SYSMENU;

	cs.style &= ~WS_SIZEBOX;

	m_strTitle = _T("扫雷");

#ifndef DEBUG_MEM

    GameCore::GetInstance()->Init();
    CRect fieldPos = GameCore::GetInstance()->GetMineFieldPos();
    cs.cx = fieldPos.Width() > 0 ? fieldPos.Width() + FRAME_BORD_H : 800;
    cs.cy = fieldPos.Height() > 0 ? fieldPos.Height() + FRAME_BORD_V + STATUS_VIEW_HEI + 7 : 600;
#else
    cs.cx = 800;
    cs.cy = 600;
#endif

	return TRUE;
}

void CMainFrame::ResizeFrame()
{
    CRect fieldPos = GameCore::GetInstance()->GetMineFieldPos();
    int w = fieldPos.Width() > 0 ? fieldPos.Width() + FRAME_BORD_H : 800;
    int h = fieldPos.Height() > 0 ? fieldPos.Height() + FRAME_BORD_V + STATUS_VIEW_HEI + 7 : 600;

    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);
    
    CRect rc;
    rc.left = (cx - w) / 2;
    rc.top = (cy - h) / 2;
    rc.right = rc.left + w;
    rc.bottom = rc.top + h;
    MoveWindow(rc);

    GetMainView()->Invalidate();
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}



void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
}


void CMainFrame::OnDifficultyEasy()
{
    // TODO: 在此添加命令处理程序代码
    if (GameCore::GetInstance()->SetGameLevel(GL_Easy)) {
        ResizeFrame();
        _splitter.GetPane(0, 0)->SendMessage(WM_USER_GAME_RESET);
    }
}


void CMainFrame::OnDifficultyMiddle()
{
    // TODO: 在此添加命令处理程序代码
    if (GameCore::GetInstance()->SetGameLevel(GL_Middle)) {
        ResizeFrame();
        _splitter.GetPane(0, 0)->SendMessage(WM_USER_GAME_RESET);
    }
}


void CMainFrame::OnDifficultyExpert()
{
    // TODO: 在此添加命令处理程序代码
    if (GameCore::GetInstance()->SetGameLevel(GL_Expert)) {
        ResizeFrame();
        _splitter.GetPane(0, 0)->SendMessage(WM_USER_GAME_RESET);
    }
}


void CMainFrame::OnDifficultyCustomize()
{
    // TODO: 在此添加命令处理程序代码
    CCustomizeDlg dlg;
    if (dlg.DoModal() == IDOK) {
        GameCore::GetInstance()->Customize(dlg._rows, dlg._columns, dlg._mines);
        GameCore::GetInstance()->SetGameLevel(GL_Custom);
        ResizeFrame();
        GetStatusView()->SendMessage(WM_USER_GAME_RESET);
    }
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
    // TODO: 在此添加专用代码和/或调用基类

    CRect rc;
    GetClientRect(&rc);

    _splitter.CreateStatic(this, 2, 1);
    _splitter.CreateView(0, 0, RUNTIME_CLASS(CGameStatusView), CSize(rc.Width(), STATUS_VIEW_HEI), pContext);
    _splitter.CreateView(1, 0, RUNTIME_CLASS(CMineCraftView), CSize(rc.Width(), rc.Height() - STATUS_VIEW_HEI), pContext);


    GetStatusView()->SendMessage(WM_USER_GAME_RESET);

    return TRUE;
    //return CFrameWnd::OnCreateClient(lpcs, pContext);
}


CWnd* CMainFrame::GetMainView() const
{
    return (CWnd*)(dynamic_cast<CMineCraftView*>(_splitter.GetPane(1, 0)));
}

CWnd* CMainFrame::GetStatusView() const
{
    return (CWnd*)(dynamic_cast<CGameStatusView*>(_splitter.GetPane(0, 0)));
}