/********************************************************
Copyright owned by Shanghai WithNet ScienTech Co. (Ltd.)
File name:TimeConvert.cpp
Author:qingshan peng  Version:1.1  Date:2015/1/14
Description:
	ʱ��ת������Դ�ļ�
Dependencies:
Function List: 
History:
Author			Date		Detail
qingshan peng	2015/1/14	�����GetCurrentYearMonth����
*********************************************************/
#include "TimeConvert.h"

#include <time.h>


char * TimeVarConvert(unsigned long ulSec)
{
	static char szTimeStr[60];
	static time_t timer;//time_t����long int ����
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
	static time_t timer;//time_t����long int ����
	static tm *tblock;

	timer = ulSec;
	tblock = localtime(&timer);
	strftime(szTimeStr, 60, "%Y%m", tblock);
	return szTimeStr;
}

unsigned int GetCurrentSystemSecond()
{
	static time_t timer;//time_t����long int ����
	time(&timer);
	return (unsigned int)timer;
}
/************************************************       
Function:GetCurrentYearMonth
Description:
	��ȡϵͳ��ǰ���£����� 201401
Input:
Output:
Returns:char *
History:
************************************************/
char * GetCurrentYearMonth()
{
	static char szTimeStr[60];
	static time_t timer;//time_t����long int ����
	static tm *tblock;

	timer = GetCurrentSystemSecond();

	tblock = localtime(&timer);
	strftime(szTimeStr, 60, "%Y%m", tblock);
	return szTimeStr;
}
