#if !defined(AFX_NEWSOCKCLASS_H__8EB4225C_953E_4C9D_B82B_735228DD045A__INCLUDED_)
#define AFX_NEWSOCKCLASS_H__8EB4225C_953E_4C9D_B82B_735228DD045A__INCLUDED_
#define BUFFERSIZE 4096
#endif

#if _MSC_VER > 1000
#pragma once
#endif

// mysocket ÃüÁîÄ¿±ê

class mysocket : public CAsyncSocket
{
public:
	mysocket();
	virtual ~mysocket();
	
	virtual void OnReceive(int nErrorCode);
	virtual void OnAccept(int nErrorCode);

	bool ISDATA;
	bool ISQUIT;
	bool ISBMP;
	CString bmp;
	CString text;
	CString newmail;
	char data[BUFFERSIZE];
};


