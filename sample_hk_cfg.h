
// sample_hk_cfg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Csample_hk_cfgApp:
// �йش����ʵ�֣������ sample_hk_cfg.cpp
//

class Csample_hk_cfgApp : public CWinApp
{
public:
	Csample_hk_cfgApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Csample_hk_cfgApp theApp;