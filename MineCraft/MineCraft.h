
// MineCraft.h: MineCraft 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // 主符号


// CMineCraftApp:
// 有关此类的实现，请参阅 MineCraft.cpp
//

class CMineCraftApp : public CWinApp
{
public:
	CMineCraftApp() noexcept;

    ULONG_PTR gdiplusToken;

// 重写
public:
	virtual BOOL InitInstance();


// 实现
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
};

extern CMineCraftApp theApp;
