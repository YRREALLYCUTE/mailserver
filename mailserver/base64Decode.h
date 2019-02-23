#pragma once
#include"stdafx.h"
//base64的字符
char base64Code[65] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J','K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T','U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd','e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n','o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x','y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7','8', '9', '+', '/'
};

//函数原型
BOOL Char2Byte(BYTE &, char&);
BOOL BmpDecode(CString&, HBITMAP&);
CString WordDecode(CString input, int length);

//将字符型变量转换为二进制
BOOL Char2Byte(BYTE&a, char &b) {
	for (int i = 0; i < 65; i++) {
		if (b == base64Code[i]) {
			a = i & 0x00ff;
			return true;
		}
	}
	return false;
}
//bmp图片的解码
BOOL BmpDecode(CString &attachment, HBITMAP &picture)
{
	BYTE buf[4];
	CFile file;
	if (file.Open(L"1.bmp", CFile::modeWrite | CFile::modeCreate))
	{
		int len = attachment.GetLength();
		BYTE temp[4];
		int i = 0;
		while (i<len)
		{
			for (int k = 0; k<4; k++)
			{
				if (i<len)
				{
					char t = attachment.GetAt(i);
					if (!Char2Byte(temp[k], t))
					{
						temp[k] = 255;
						k--;
					}
					i++;
				}
			}
			buf[0] = (temp[0] << 2) | (temp[1] >> 4);
			buf[1] = (temp[1] << 4) | (temp[2] >> 2);
			buf[2] = (temp[2] << 6) | (temp[3]);
			file.Write(buf, 3);
			file.SeekToEnd();
		}
	}
	file.Close();
	picture = (HBITMAP)::LoadImage(NULL, L"1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	return true;
}
//邮件文本的解码
CString WordDecode(CString input, int  length)
{
	int n, i, j, pad;
	unsigned char *p;
	static unsigned char *dst;
	unsigned char * src;
	length = 0;
	pad = 0;
	n = input.GetLength();
	src = new unsigned char[n];
	for (i = 0;i<n;i++)
		src[i] = input[i];

	while (n>0 && src[n - 1] == '=') {
		src[n - 1] = 0;
		pad++;
		n--;
	}
	for (i = 0;i<n;i++) { 
		p = (unsigned char *)strchr((const char *)base64Code, (int)src[i]);
		if (!p)
			break;
		src[i] = p - (unsigned char *)base64Code;
	}

	dst = (unsigned char *)malloc(n * 3 / 4 + 1);
	memset(dst, 0, n * 3 / 4 + 1);
	for (i = 0, j = 0;i<n;i += 4, j += 3) {
		dst[j] = (src[i] << 2) + ((src[i + 1] & 0x30) >> 4);
		dst[j + 1] = ((src[i + 1] & 0x0F) << 4) + ((src[i + 2] & 0x3C) >> 2);
		dst[j + 2] = ((src[i + 2] & 0x03) << 6) + src[i + 3];
		length += 3;
	}
	length -= pad;
	return CString(dst);
}