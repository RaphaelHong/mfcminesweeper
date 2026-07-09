
// MineCraftView.h: CMineCraftView 类的接口
//

#pragma once

#include "GameCore.h"


class CMineCraftView : public CView
{
protected: // 仅从序列化创建
	CMineCraftView() noexcept;
	DECLARE_DYNCREATE(CMineCraftView)

// 特性
public:
	CMineCraftDoc* GetDocument() const;

    MineMouseManager _mouseMgr;

	std::vector<MineCell*> _blink_cells;
	BOOL _disableMouseClick{ false };
	int _blink_count{ 0 };

// 操作
public:
    void ClipRectTest(CDC* pDrawDC);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CMineCraftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg LRESULT OnMessageRedrawSingleCell(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageBlinkCells(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // MineCraftView.cpp 中的调试版本
inline CMineCraftDoc* CMineCraftView::GetDocument() const
   { return reinterpret_cast<CMineCraftDoc*>(m_pDocument); }
#endif

