#include "pch.h"
#include "CSettledSplitter.h"

IMPLEMENT_DYNAMIC(CSettledSplitter, CSplitterWnd)

BEGIN_MESSAGE_MAP(CSettledSplitter, CSplitterWnd)
    ON_WM_LBUTTONDOWN()
    ON_WM_SETCURSOR()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void CSettledSplitter::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    return;

    CSplitterWnd::OnLButtonDown(nFlags, point);
}


BOOL CSettledSplitter::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    return TRUE;

    return CSplitterWnd::OnSetCursor(pWnd, nHitTest, message);
}


void CSettledSplitter::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CWnd::OnMouseMove(nFlags, point);
    //CSplitterWnd::OnMouseMove(nFlags, point);
}
