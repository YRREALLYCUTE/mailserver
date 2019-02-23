// mysocket.cpp : 实现文件
//

#include "stdafx.h"
#include "mailserver.h"
#include "mysocket.h"
#include "base64Decode.h"
#include "mailserverDlg.h"

bool strcom(CString a1, char* b);
void CString2Char(CString str, char ch[]);
// mysocket

mysocket::mysocket()
{
	ISDATA = false;
	ISQUIT = false;
}

mysocket::~mysocket()
{
}


// mysocket 成员函数


void mysocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CmailserverDlg * dlg = (CmailserverDlg *)AfxGetApp()->GetMainWnd();
	int length = Receive(data, sizeof(data), 0);//data中的内容即EHLO等

	CString log, content;
	dlg->m_log.GetWindowTextW(log);
	content += data;// 类型转换，将char* 转为CString

	if (length != 0) {
		log = log + L"Client: " + content.Left(length) + L"\r\n";
		dlg->m_log.SetWindowTextW(log);

		if (!ISDATA) {
			if (strcom(content.Left(4), "ehlo")|| strcom(content.Left(4), "helo")) {
				char * send = "250 OK \r\n";
				Send(send, strlen(send), 0);
				AsyncSelect(FD_READ);
				log += L"Server: 250 OK!\r\n\r\n";
				dlg->m_log.SetWindowTextW(log);
				return;
			}
			if (strcom(content.Left(10), "mail from:")) {
				char *send = "250 OK!\r\n";
				Send(send, strlen(send), 0);
				AsyncSelect(FD_READ);
				log += L"Server: 250 OK!\r\n\r\n";
				dlg->m_log.SetWindowTextW(log);
				return;
			}
			if (strcom(content.Left(8), "rcpt to:")) {
				char *send = "250 OK\r\n";
				Send(send, strlen(send), 0);
				AsyncSelect(FD_READ);
				log += L"Server: 250 OK!\r\n\r\n";
				dlg->m_log.SetWindowTextW(log);
				return;
			}
			if (strcom(content.Left(4), "data")) {
				ISDATA = true;
				char* send = "354 Go ahead!\r\n";
				Send(send, strlen(send), 0);
				AsyncSelect(FD_READ);
				log += L"354 Go ahead\r\n\r\n";
				dlg->m_log.SetWindowTextW(log);
				return;
			}
			if (strcom(content.Left(4), "quit")) {
				ISQUIT = true;
				char* send = "221 Quit, GoodBye! \r\n";
				Send(send, strlen(send), 0);
				AsyncSelect(FD_READ);
				log += L"Server: 221 Quit, GoodBye!\r\n\r\n";
				dlg->m_log.SetWindowTextW(log);
				return;
			}
			char* send = "500 order is wrong\r\n";

			Send(send, strlen(send), 0);
			log = log + L"Server: 500 Order is wrong\r\n\r\n";
			ISQUIT = true;
			dlg->m_log.SetWindowTextW(log);
			return;
		}
		else {
			CString str;
			//将接收到的邮件的内容显示到邮件data的窗口
			dlg->m_info.GetWindowTextW(str);
			//dlg->m_info.GetWindowTextW(strTmp);
			str += content.Left(length);
			newmail += content.Left(length);
			dlg->m_info.SetWindowTextW(str);

			//newmail = str.Mid(strTmp.GetLength(), length - strTmp.GetLength());

			if (content.Find(L"\r\n.\r\n") != -1) {
				ISDATA = false;
				char* send;
				send = "250 OK!\r\n";
				Send(send, strlen(send), 0);

				log += L"Server: 250 OK!\r\n\r\n";
				dlg->m_log.SetWindowTextW(log);
				
				int combolength = dlg->m_userlist.GetCount();
				//如果maillsit为空的话就直接显示出邮件的内容，否则的话先存储到缓冲列表里。
				if(combolength == 0) {
					dlg->count += 1;
					CString itemCount;
					itemCount.Format(_T("%d"), dlg->count);
					

					dlg->m_info.GetWindowTextW(bmp);//图片
					dlg->m_info.GetWindowTextW(text);//文本

					 //图片解码显示
					if (bmp.Find(L"Content-Type: application/octet-stream;") != -1)
					{
						int Attachment_Start = bmp.Find(L"Content-Disposition: attachment", 0);
						int Bmp_Start = bmp.Find(L"\r\n\r\n", Attachment_Start);
						CString Start = bmp.Mid(Bmp_Start + 4, bmp.GetLength() - Bmp_Start - 4);
						int length = Start.Find(L"\r\n\r\n", 0);
						bmp = Start.Left(length);
						HBITMAP picture;
						BmpDecode(bmp, picture);
						dlg->m_bmp.SetBitmap(picture);
						dlg->bmplist[dlg->count - 1] = picture;
					}
					//文本解码显示
					if (text.Find(L"Content-Type: text/plain;") != -1) {
						CString temptext = text;
						int Start = text.Find(L"Content-Transfer-Encoding: base64", 0);
						int Text_Start = text.Find(L"\r\n", Start);
						CString real_Start = text.Mid(Text_Start + 4, text.GetLength() - Text_Start - 4);
						int length = real_Start.Find(L"\r\n", 0);
						text = real_Start.Left(length);

						CString content;
						CString From, To, Date, Subject;

						int DateStart = temptext.Find(L"Date: ", 0);
						int DateLength = temptext.Find(L"\r\n", DateStart);
						Date = temptext.Mid(DateStart, DateLength - DateStart);
						Date += L"\r\n";

						int FromStart = temptext.Find(L"From:", 0);
						int FromLength = temptext.Find(L">", FromStart);
						From = temptext.Mid(FromStart, FromLength - FromStart + 1);
						From += L"\r\n";


						int ToStart = temptext.Find(L"To: ", 0);
						int ToLength = temptext.Find(L">", ToStart);
						To = temptext.Mid(ToStart, ToLength - ToStart + 1);
						To += L"\r\n";

						int SubjectStart = temptext.Find(L"Subject:", 0);
						int SubjectLength = temptext.Find(L"\r\n", SubjectStart);
						Subject = temptext.Mid(SubjectStart, SubjectLength - SubjectStart);
						Subject += L"\r\n";


						dlg->m_content.GetWindowTextW(content);
						content = content + Date + From + To + Subject + L"Content: \r\n";
						content += WordDecode(text, sizeof(text));
						dlg->m_content.SetWindowTextW(content);
						dlg->m_userlist.InsertString(dlg->count - 1, itemCount + L"   (" + From + ")");
						dlg->textlist[dlg->count - 1] = content;
					}
				}

				else {
					dlg->count += 1;
					CString itemCount;
					itemCount.Format(_T("%d"), dlg->count);
					

					bmp = newmail;
					text = newmail; //将两个临时变量赋值为最新的邮件的内容

					//图片解码存储，但不显示
					if (bmp.Find(L"Content-Type: application/octet-stream;") != -1)
					{
						int Attachment_Start = bmp.Find(L"Content-Disposition: attachment", 0);
						int Bmp_Start = bmp.Find(L"\r\n\r\n", Attachment_Start);
						CString Start = bmp.Mid(Bmp_Start + 4, bmp.GetLength() - Bmp_Start - 4);
						int length = Start.Find(L"\r\n\r\n", 0);
						bmp = Start.Left(length);
						HBITMAP picture;
						BmpDecode(bmp, picture);
					//	dlg->m_bmp.SetBitmap(picture);
						dlg->bmplist[dlg->count - 1] = picture;
					}
					//文本解码存储，但不显示
					if (text.Find(L"Content-Type: text/plain;") != -1) {
						CString temptext = text;
						int Start = text.Find(L"Content-Transfer-Encoding: base64", 0);
						int Text_Start = text.Find(L"\r\n", Start);
						CString real_Start = text.Mid(Text_Start + 4, text.GetLength() - Text_Start - 4);
						int length = real_Start.Find(L"\r\n", 0);
						text = real_Start.Left(length);

						CString content;
						CString From, To, Date, Subject;

						int DateStart = temptext.Find(L"Date: ", 0);
						int DateLength = temptext.Find(L"\r\n", DateStart);
						Date = temptext.Mid(DateStart, DateLength - DateStart);
						Date += L"\r\n";

						int FromStart = temptext.Find(L"From:", 0);
						int FromLength = temptext.Find(L">", FromStart);
						From = temptext.Mid(FromStart, FromLength - FromStart + 1);
						From += L"\r\n";


						int ToStart = temptext.Find(L"To: ", 0);
						int ToLength = temptext.Find(L">", ToStart);
						To = temptext.Mid(ToStart, ToLength - ToStart + 1);
						To += L"\r\n";

						int SubjectStart = temptext.Find(L"Subject:", 0);
						int SubjectLength = temptext.Find(L"\r\n", SubjectStart);
						Subject = temptext.Mid(SubjectStart, SubjectLength - SubjectStart);
						Subject += L"\r\n";


						//AfxGetMainWnd()->GetDlgItemTextW(IDC_CONTENT, content);
						content = content + Date + From + To + Subject + L"Content: \r\n";
						content += WordDecode(text, sizeof(text));
						dlg->m_userlist.InsertString(dlg->count - 1, itemCount + L"   (" + From + L")");
						dlg->textlist[dlg->count - 1] = content;
					}
				}

				/*dlg->m_info.GetWindowTextW(bmp);//图片
				dlg->m_info.GetWindowTextW(text);//文本

				//图片解码显示
				if (bmp.Find(L"Content-Type: application/octet-stream;") != -1)
				{
					int Attachment_Start = bmp.Find(L"Content-Disposition: attachment", 0);
					int Bmp_Start = bmp.Find(L"\r\n\r\n", Attachment_Start);
					CString Start = bmp.Mid(Bmp_Start + 4, bmp.GetLength() - Bmp_Start - 4);
					int length = Start.Find(L"\r\n\r\n", 0);
					bmp = Start.Left(length);
					HBITMAP picture;
					BmpDecode(bmp, picture);
					dlg->m_bmp.SetBitmap(picture);

				}
				//文本解码显示
				if (text.Find(L"Content-Type: text/plain;") != -1) {
					CString temptext = text;
					int Start = text.Find(L"Content-Transfer-Encoding: base64", 0);
					int Text_Start = text.Find(L"\r\n", Start);
					CString real_Start = text.Mid(Text_Start + 4, text.GetLength() - Text_Start - 4);
					int length = real_Start.Find(L"\r\n", 0);
					text = real_Start.Left(length);

					CString content;
					CString From, To, Date, Subject;
					
					int DateStart = temptext.Find(L"Date: ", 0);
					int DateLength = temptext.Find(L"\r\n", DateStart);
					Date = temptext.Mid(DateStart, DateLength - DateStart);
					Date += L"\r\n";
					
					int FromStart = temptext.Find(L"From:", 0);
					int FromLength = temptext.Find(L">", FromStart);
					From = temptext.Mid(FromStart ,FromLength - FromStart+1) ;
					From += L"\r\n";


					int ToStart = temptext.Find(L"To: ", 0);
					int ToLength = temptext.Find(L">", ToStart);
					To = temptext.Mid(ToStart, ToLength - ToStart+1);
					To += L"\r\n";

					int SubjectStart = temptext.Find(L"Subject:", 0);
					int SubjectLength = temptext.Find(L"\r\n", SubjectStart);
					Subject = temptext.Mid(SubjectStart, SubjectLength - SubjectStart);
					Subject += L"\r\n";

					
					AfxGetMainWnd()->GetDlgItemTextW(IDC_CONTENT, content);
					content = content + Date + From + To + Subject +L"Content: \r\n";
					content += WordDecode(text, sizeof(text));
					AfxGetMainWnd()->SetDlgItemText(IDC_CONTENT, content);
				}*/
			}
			AsyncSelect(FD_READ);

			return;
		}
	}
	if (ISQUIT)//退出
	{
		Close();//关闭套接字
		ISQUIT = false;
		return;

	}

	CAsyncSocket::OnReceive(nErrorCode);
}


void mysocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CmailserverDlg * dlg = (CmailserverDlg *)AfxGetApp()->GetMainWnd();
	mysocket *socktest = new mysocket();
	CString log;
	dlg->m_log.GetWindowTextW(log);

	int ISCONN = Accept(*socktest);
	if (ISCONN != 0) {
		log += L" connect success! \r\n\r\n";
		char *send = "220 localhost ready! \r\n";
		socktest->Send(send, strlen(send), 0);
		socktest->AsyncSelect(FD_READ);
	}
	else {
		log += L" connect failed!\r\n ";
	}
	dlg->m_log.SetWindowTextW(log);
	CAsyncSocket::OnAccept(nErrorCode);
}

//字符转换函数的实现
void CString2Char(CString str, char ch[]){
	int i;
	char *tmpch;
	int wLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	tmpch = new char[wLen + 1];                                             
	WideCharToMultiByte(CP_ACP, 0, str, -1, tmpch, wLen, NULL, NULL); 
	for (i = 0; tmpch[i] != '\0'; i++)
		ch[i] = tmpch[i];
	ch[i] = '\0';
}

//忽略大小写
bool strcom(CString a1, char *b) {

	char *a = new char[100];

	CString2Char(a1,a);

	int len1 = strlen(a);
	int len2 = strlen(b);
	if (len1 < len2) {
		return false;
	}
	int py = 0;
	for (int i = 0; i<len2; i++) {
		if (a[i] >= 'A' && a[i] <= 'Z') {
			py = 32;
		}
		else {
			py = 0;
		}
		if (a[i] + py != b[i]) {
			return false;
		}
	}
	return true;
}