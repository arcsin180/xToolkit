/********************************************************
Copyright owned by Shanghai WithNet ScienTech Co. (Ltd.)
File name:TimeConvert.cpp
Author:qingshan peng  Version:1.0  Date:2014/09/12
Description:
	时标转化服务源文件
Dependencies:
Function List: 
History:
Author    Date      Detail
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
	static time_t timer;//time_t就是long int 类型
	time(&timer);

	return TimeVarConvert((unsigned long)timer);
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
