/********************************************************
Copyright owned by Shanghai WithNet ScienTech Co. (Ltd.)
File name:StringCodeConvert.h
Author:qingshan peng  Version:1.0  Date:2014/09/12
Description:
	ͨ�õ��ַ�����ת������ͷ�ļ���
Dependencies:
Function List: 
History:
Author    Date      Detail
*********************************************************/
#ifndef __STRINGCODECONVERT_H__
#define __STRINGCODECONVERT_H__

/************************************************       
Function:UTF8ToGBK
Description:
	UTF8�ַ���תΪΪGBK�������ַ���ת�����ַ�Ӧ
��ʱ���ƣ�����ᱻ��գ����̲߳���Ҫ����ԭ���ԡ�
�ڲ��ִ�����ռ�Ϊ255�����ַ���
Input:
char * szUTF8
Output:
Returns:char *
	ת���Ľ��
History:
************************************************/
char * UTF8ToGBK(const char *szUTF8);

/************************************************       
Function:UTF8ToGBK
Description:
	GBK�����ַ���תΪΪUTF8���ַ���ת�����ַ�Ӧ
��ʱ���ƣ�����ᱻ��գ����̲߳���Ҫ����ԭ���ԡ�
�ڲ��ִ�����ռ�Ϊ255�����ַ���
Input:
char * szGBK
Output:
Returns:char *
	ת���Ľ��
History:
************************************************/
char * GBKToUTF8(const char *szGBK);
#endif