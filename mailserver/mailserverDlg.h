
// mailserverDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "mysocket.h"
#define MAILSIZE 100
// CmailserverDlg �Ի���
class CmailserverDlg : public CDialogEx
{
// ����
public:
	CmailserverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAILSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
