#pragma once
#include <afxext.h>

class CSettledSplitter :
    public CSplitterWnd
{
    DECLARE_DYNAMIC(CSettledSplitter)
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

