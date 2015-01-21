/********************************************************
Copyright owned by Shanghai WithNet ScienTech Co. (Ltd.)
File name:StringCodeConvert.h
Author:qingshan peng  Version:1.1  Date:2015/01/12
Description:
	ͨ�õ��ַ�����ת������ͷ�ļ���
Dependencies:
Function List: 
History:
Author    Date      Detail
*********************************************************/
#if defined WIN32 || defined _WIN32
#include "StringCodeConvert.h"
#include <Windows.h>
#else
#include <iconv.h> 
#endif

#include <string.h>
/************************************************       
Function:UTF8ToGBK
Description:
	UTF8�ַ���תΪΪGBK�������ַ���
Input:
char * szUTF8 ԭUTF8����
Output:
Returns:char *
	ת����Ľ��
History:
************************************************/
char * UTF8ToGBK(const char *szUTF8)
{
	static char wszGBK[255 * 2]; //��255�������ַ�
	memset(wszGBK, 0, sizeof(wszGBK));
#if defined WIN32 || defined _WIN32
	static wchar_t wszTmp[255]; //ת��Ҫʹ��UNICOED���м�ֵ
	MultiByteToWideChar(CP_UTF8, 0, szUTF8, -1, wszTmp, 255);  
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszTmp, -1, wszGBK, 255 * 2, NULL, NULL);
#else
	static iconv_t cd;
	size_t inlen = strlen(szUTF8);
	size_t outlen = sizeof(wszGBK);
	char *pin = (char *)szUTF8; 
	char *pout = wszGBK; 
	cd = iconv_open("GBK", "UTF-8");
	if((iconv_t)-1 == cd)
	{
		return wszGBK;
	}
	if (-1 == iconv(cd, &pin, (size_t *)&inlen, &pout, (size_t *)&outlen))
	{
		wszGBK[0] = 0;
		return wszGBK;
	}
	
	iconv_close(cd);
#endif
	return wszGBK;  
}

char * GBKToUTF8(const char *szGBK)
{
	static char wszUTF8[255 * 3]; //��255�������ַ�
	//note:��Ӵ��ж� [1/12/2015 qingshan peng]
	if (NULL == szGBK ||
		strlen(szGBK) > 500)
	{
		wszUTF8[0] = 0;
		return wszUTF8;
	}
	memset(wszUTF8, 0, sizeof(wszUTF8));
#if defined WIN32 || defined _WIN32
	static wchar_t wszTmp[255]; //ת��Ҫʹ��UNICOED���м�ֵ
	MultiByteToWideChar(CP_ACP, 0, szGBK, -1, wszTmp, 255);  
	WideCharToMultiByte(CP_UTF8, 0, wszTmp, -1, wszUTF8, 255 * 2, NULL, NULL);  
#else
	static iconv_t cd;
	size_t inlen = strlen(szGBK);
	size_t outlen = sizeof(wszUTF8); 
	char *pin = (char *)szGBK; 
	char *pout = wszUTF8; 
	cd = iconv_open("UTF-8", "GBK");
	if((iconv_t)-1 == cd)
	{
		return wszUTF8;
	}
	if (-1 == iconv(cd, &pin, (size_t *)&inlen, &pout, (size_t *)&outlen))
	{
		wszUTF8[0] = 0;
		return wszUTF8;
	}
	
	iconv_close(cd);
#endif
	return wszUTF8;
}
