/********************************************************
Copyright owned by Shanghai WithNet ScienTech Co. (Ltd.)
File name:TimeConvert.h
Author:qingshan peng  Version:1.1  Date:2015/1/14
Description:
	时标转化服务头文件头
Dependencies:
Function List: 
History:
Author			Date		Detail
qingshan peng	2015/1/14	添加了GetCurrentYearMonth方法
*********************************************************/
#ifndef __TIMECONVERT_H__
#define __TIMECONVERT_H__

/************************************************       
Function:TimeVarConvert
Description:
	转为以1970年为起始的秒数为年月日时分秒返回
Input:
unsigned long ulSec 秒数
Output:
Returns:char *
	返回转化结果字串首地址，用户需要及时拷贝
结果。在多线程中使用需要注意原子性，避免结果
出现紊乱。
History:
************************************************/
char *TimeVarConvert(unsigned long ulSec);
/************************************************       
Function:TimeVarConvert
Description:
	转为以1970年为起始的秒数为年月且无分隔符
Input:
unsigned long ulSec 秒数
Output:
Returns:char *
	返回转化结果字串首地址，用户需要及时拷贝
结果。在多线程中使用需要注意原子性，避免结果
出现紊乱。
History:
************************************************/
char *TimeVarGetYearMonth(unsigned long ulSec);

/************************************************       
Function:GetCurrentSystemSecond
Description:
	以1970年为起始的秒数
Input:
Output:
Returns:unsigned int
History:
************************************************/
unsigned int GetCurrentSystemSecond();


/************************************************       
Function:GetCurrentTimestaep
Description:
	获取系统当前时标
Input:
Output:
Returns:char *
History:
************************************************/
char *GetCurrentTimestaep();

/************************************************       
Function:GetCurrentYearMonth
Description:
	获取系统当前年月，例如 201401
Input:
Output:
Returns:char *
History:
************************************************/
char *GetCurrentYearMonth();
#endif