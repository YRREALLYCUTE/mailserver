
// mailserverDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "mysocket.h"
#define MAILSIZE 100
// CmailserverDlg 对话框
class CmailserverDlg : public CDialogEx
{
// 构造
public:
	CmailserverDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAILSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_log;
	CEdit m_info;
	CEdit m_content;
	CStatic m_bmp;
	mysocket Socket;
	CComboBox m_userlist;
	CString textlist[MAILSIZE];
	HBITMAP bmplist[MAILSIZE];
	int count;
	afx_msg void OnCbnSelchangeUserlist();
};
