/********************************************************
Copyright owned by Shanghai WithNet ScienTech Co. (Ltd.)
File name:TimeConvert.cpp
Author:qingshan peng  Version:1.1  Date:2015/1/14
Description:
	时标转化服务源文件
Dependencies:
Function List: 
History:
Author			Date		Detail
qingshan peng	2015/1/14	添加了GetCurrentYearMonth方法
*********************************************************/
#include "TimeConvert.h"

#include <time.h>


char * TimeVarConvert(unsigned long ulSec)
{
	static char szTimeStr[60];
	static time_t timer;//time_t就是long int 类型
	static tm *tblock;

	timer = ulSec;
	tblock = localtime(&timer);
	strftime(szTimeStr, 60, "%Y-%m-%d %H:%M:%S", tblock);
	return szTimeStr;
}

char * GetCurrentTimestaep()
{
	return TimeVarConvert(GetCurrentSystemSecond());
}



char * TimeVarGetYearMonth( unsigned long ulSec )
{
	static char szTimeStr[60];
	static time_t timer;//time_t就是long int 类型
	static tm *tblock;

	timer = ulSec;
	tblock = localtime(&timer);
	strftime(szTimeStr, 60, "%Y%m", tblock);
	return szTimeStr;
}

unsigned int GetCurrentSystemSecond()
{
	static time_t timer;//time_t就是long int 类型
	time(&timer);
	return (unsigned int)timer;
}
/************************************************       
Function:GetCurrentYearMonth
Description:
	获取系统当前年月，例如 201401
Input:
Output:
Returns:char *
History:
************************************************/
char * GetCurrentYearMonth()
{
	static char szTimeStr[60];
	static time_t timer;//time_t就是long int 类型
	static tm *tblock;

	timer = GetCurrentSystemSecond();

	tblock = localtime(&timer);
	strftime(szTimeStr, 60, "%Y%m", tblock);
	return szTimeStr;
}
