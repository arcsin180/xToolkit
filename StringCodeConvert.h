/********************************************************
Copyright owned by Shanghai WithNet ScienTech Co. (Ltd.)
File name:StringCodeConvert.h
Author:qingshan peng  Version:1.0  Date:2014/09/12
Description:
	通用的字符编码转换服务头文件。
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
	UTF8字符串转为为GBK的中文字符串转化的字符应
及时复制，否则会被清空，多线程操作要主意原子性。
内部字串缓冲空间为255个宽字符。
Input:
char * szUTF8
Output:
Returns:char *
	转化的结果
History:
************************************************/
char * UTF8ToGBK(const char *szUTF8);

/************************************************       
Function:UTF8ToGBK
Description:
	GBK中文字符串转为为UTF8的字符串转化的字符应
及时复制，否则会被清空，多线程操作要主意原子性。
内部字串缓冲空间为255个宽字符。
Input:
char * szGBK
Output:
Returns:char *
	转化的结果
History:
************************************************/
char * GBKToUTF8(const char *szGBK);
#endif