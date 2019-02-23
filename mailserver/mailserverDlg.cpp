
// mailserverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mailserver.h"
#include "mailserverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CmailserverDlg �Ի���



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


// CmailserverDlg ��Ϣ�������

BOOL CmailserverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	this->SetWindowTextW(L"�ʼ�������");
	count = 0;
	int Isopen = 0;
	this->MessageBoxW(L"������25�Ŷ˿�.......");
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CmailserverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CmailserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CmailserverDlg::OnCbnSelchangeUserlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString temp;
	int nindex = m_userlist.GetCurSel();
	m_userlist.GetLBText(nindex,temp);

	char *ordertemp = new char[100];
	//��CString ת��Ϊchar*
	int i;
	char *tmpch;
	int wLen = WideCharToMultiByte(CP_ACP, 0, temp, -1, NULL, 0, NULL, NULL);
	tmpch = new char[wLen + 1];
	WideCharToMultiByte(CP_ACP, 0, temp, -1, tmpch, wLen, NULL, NULL);
	for (i = 0; tmpch[i] != '\0'; i++)
		ordertemp[i] = tmpch[i];
	ordertemp[i] = '\0';
	//���������ǽ�char*ת��Ϊint 
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
