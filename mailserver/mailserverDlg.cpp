
// mailserverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "mailserver.h"
#include "mailserverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmailserverDlg 对话框



CmailserverDlg::CmailserverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAILSERVER_DIALOG, pParent)
	, count(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmailserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG, m_log);
	DDX_Control(pDX, IDC_INFO, m_info);
	DDX_Control(pDX, IDC_CONTENT, m_content);
	DDX_Control(pDX, IDC_BMP, m_bmp);
	DDX_Control(pDX, IDC_USERLIST, m_userlist);
}

BEGIN_MESSAGE_MAP(CmailserverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_USERLIST, &CmailserverDlg::OnCbnSelchangeUserlist)
END_MESSAGE_MAP()


// CmailserverDlg 消息处理程序

BOOL CmailserverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	this->SetWindowTextW(L"邮件服务器");
	count = 0;
	int Isopen = 0;
	this->MessageBoxW(L"即将打开25号端口.......");
	Isopen = Socket.Create(25, SOCK_STREAM, FD_ACCEPT | FD_READ);

	if (Isopen == 1) {
		int Islisten = 0;
		Islisten = Socket.Listen(1);
		if (Islisten == 1) {
			m_log.SetWindowTextW(L" smtp server is ready!\r\n");
		}
		else {
			m_log.SetWindowTextW(L" failed !\r\n");
		}
	}
	else {
		m_log.SetWindowTextW(L" failed !\r\n");
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CmailserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmailserverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CmailserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CmailserverDlg::OnCbnSelchangeUserlist()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp;
	int nindex = m_userlist.GetCurSel();
	m_userlist.GetLBText(nindex,temp);

	char *ordertemp = new char[100];
	//将CString 转化为char*
	int i;
	char *tmpch;
	int wLen = WideCharToMultiByte(CP_ACP, 0, temp, -1, NULL, 0, NULL, NULL);
	tmpch = new char[wLen + 1];
	WideCharToMultiByte(CP_ACP, 0, temp, -1, tmpch, wLen, NULL, NULL);
	for (i = 0; tmpch[i] != '\0'; i++)
		ordertemp[i] = tmpch[i];
	ordertemp[i] = '\0';
	//接下来的是将char*转化为int 
	int order = 0;
	
	int numLen = 0;

	for (int i = 0;i < strlen(ordertemp); i++) {
		if (ordertemp[i] == ' ') {
			break;
		}
		else {
			numLen ++;
		}
	}

	for (int i = numLen - 1; i >=0; i--) {
		order += (ordertemp[i] - 48) * pow(10.0, numLen - i - 1);
	}

	m_content.SetWindowTextW(textlist[order - 1]);
	m_bmp.SetBitmap(bmplist[order - 1]);
}
