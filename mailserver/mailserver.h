
// mailserver.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CmailserverApp: 
// �йش����ʵ�֣������ mailserver.cpp
//

class CmailserverApp : public CWinApp
{
public:
	CmailserverApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CmailserverApp theApp;