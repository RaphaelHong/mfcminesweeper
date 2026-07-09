
// MineCraftView.cpp: CMineCraftView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MineCraft.h"
#endif

#include "MineCraftDoc.h"
#include "MineCraftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_BINK_CELLS (1001)


using namespace Gdiplus;

// CMineCraftView

IMPLEMENT_DYNCREATE(CMineCraftView, CView)

BEGIN_MESSAGE_MAP(CMineCraftView, CView)
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
    ON_MESSAGE(WM_USER_REDRAW_SINGLE_CELL, OnMessageRedrawSingleCell)
	ON_MESSAGE(WM_USER_BLINK_CELLS, OnMessageBlinkCells)
END_MESSAGE_MAP()

// CMineCraftView 构造/析构

CMineCraftView::CMineCraftView() noexcept
{
	// TODO: 在此处添加构造代码
    _mouseMgr.SetOwner(GetGameCore->GetMineFieldPtr());
}

CMineCraftView::~CMineCraftView()
{
}

BOOL CMineCraftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMineCraftView 绘图

void CMineCraftView::OnDraw(CDC* /*pDC*/)
{
	CMineCraftDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    /*
    CDC* pDC = GetDC();
    CBrush brush(RGB(0, 181, 132));//brush(RGB(30, 95, 78));
    pDC->FillRect(CRect(0, 0, 800, 600), &brush);
    //CBrush grayBr(RGB(244, 244, 244));
    */
    
    CDC* pDC = GetDC();

    CRect rcCli;
    GetClientRect(&rcCli);

    CDC* pMemDC = new CDC;
    pMemDC->CreateCompatibleDC(pDC);
    CBitmap memBmp;
    memBmp.CreateCompatibleBitmap(pDC, rcCli.Width(), rcCli.Height());
    pMemDC->SelectObject(&memBmp);

    CBrush brush(RGB(0, 181, 132));//brush(RGB(30, 95, 78));
    pMemDC->FillRect(CRect(0, 0, 800, 600), &brush);

    GameCore::GetInstance()->DrawMineField(pMemDC);

    pDC->BitBlt(0, 0, rcCli.Width(), rcCli.Height(), pMemDC, 0, 0, SRCCOPY);

    delete pMemDC;
    pMemDC = nullptr;
}


// CMineCraftView 诊断

#ifdef _DEBUG
void CMineCraftView::AssertValid() const
{
	CView::AssertValid();
}

void CMineCraftView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMineCraftDoc* CMineCraftView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMineCraftDoc)));
	return (CMineCraftDoc*)m_pDocument;
}
#endif //_DEBUG


// CMineCraftView 消息处理程序
void CMineCraftView::ClipRectTest(CDC* pDrawDC)
{
    //定义画刷
    //CDC *pDC = GetDC();
    CBrush Brush(RGB(0, 250, 0));
    CBrush *pOldBrush = pDrawDC->SelectObject(&Brush);

    //设置裁剪区
    CRgn rgnA;
    CRgn rgnB;
    rgnA.CreateEllipticRgn(0, 0, 300, 300);
    pDrawDC->SelectClipRgn(&rgnA);

    rgnB.CreateEllipticRgn(200, 200, 500, 500);
    pDrawDC->SelectClipRgn(&rgnB, RGN_OR);
    //文本设置和输出
    pDrawDC->SetTextColor(RGB(0, 0, 250));
    pDrawDC->TextOut(130, 150, L"code");

    //创建路径层对象
    pDrawDC->BeginPath();
    pDrawDC->RoundRect(CRect(100, 100, 200, 200), CPoint(150, 150));
    pDrawDC->EndPath();
    pDrawDC->SelectClipPath(RGN_DIFF);

    //路径层中的矩形
    pDrawDC->Rectangle(CRect(340, 340, 360, 360));

    //创建路径层对象
    pDrawDC->BeginPath();
    pDrawDC->RoundRect(CRect(300, 300, 400, 400), CPoint(350, 350));
    pDrawDC->EndPath();
    pDrawDC->SelectClipPath(RGN_DIFF);
    //画网格
    for (int i = 0; i < 500; i += 10)
    {
        pDrawDC->MoveTo(0, i);
        pDrawDC->LineTo(500, i);
        pDrawDC->MoveTo(i, 0);
        pDrawDC->LineTo(i, 500);
    }

    pDrawDC->SelectObject(pOldBrush);
}

void CMineCraftView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (_disableMouseClick) {
        return CView::OnRButtonDown(nFlags, point);
    }

    _mouseMgr.OnMouseLBtnDown(point);

    CView::OnLButtonDown(nFlags, point);
}


void CMineCraftView::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (_disableMouseClick) {
        return CView::OnRButtonDown(nFlags, point);
    }

    _mouseMgr.OnMouseRBtnDown(point);

    CView::OnRButtonDown(nFlags, point);
}

LRESULT CMineCraftView::OnMessageRedrawSingleCell(WPARAM wParam, LPARAM lparam)
{
    MineCell* cell = reinterpret_cast<MineCell*>(wParam);

    CDC* pDC = GetDC();
	Graphics gp(pDC->GetSafeHdc());
	MineCell::GetHandler(cell->Surface())->Draw(cell, &gp);

    return 0;
}

LRESULT CMineCraftView::OnMessageBlinkCells(WPARAM wParam, LPARAM lParam)
{
	auto cells = reinterpret_cast<std::vector<MineCell*>*>(wParam);
	if (cells == nullptr) return 0;

	_blink_cells.assign(cells->begin(), cells->end());    

	SetTimer(TIMER_BINK_CELLS, 100, NULL);

	_disableMouseClick = TRUE;
    _blink_count = 0;
	
	return 0;
}   

void CMineCraftView::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
	case TIMER_BINK_CELLS:
	{
        CDC* pDC = GetDC();
        Graphics gp(pDC->GetSafeHdc());
        if (_blink_count % 2 == 0) {
            for (auto cell : _blink_cells) {
                MineCell::GetHandler(CellSurfaceStatus::hollow)->Draw(cell, &gp);
            }
        }
        else {
            for (auto cell : _blink_cells) {
                MineCell::GetHandler(cell->Surface())->Draw(cell, &gp);
            }
        }
		
        _blink_count++;

        if (_blink_count > 3) {
            KillTimer(TIMER_BINK_CELLS);
            _disableMouseClick = FALSE;
        }
        break;
	}
	default:
		break;
    }
	CView::OnTimer(nIDEvent);
}