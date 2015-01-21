/********************************************************
Copyright owned by Shanghai WithNet ScienTech Co. (Ltd.)
File name:TimeConvert.h
Author:qingshan peng  Version:1.1  Date:2015/1/14
Description:
	ʱ��ת������ͷ�ļ�ͷ
Dependencies:
Function List: 
History:
Author			Date		Detail
qingshan peng	2015/1/14	�����GetCurrentYearMonth����
*********************************************************/
#ifndef __TIMECONVERT_H__
#define __TIMECONVERT_H__

/************************************************       
Function:TimeVarConvert
Description:
	תΪ��1970��Ϊ��ʼ������Ϊ������ʱ���뷵��
Input:
unsigned long ulSec ����
Output:
Returns:char *
	����ת������ִ��׵�ַ���û���Ҫ��ʱ����
������ڶ��߳���ʹ����Ҫע��ԭ���ԣ�������
�������ҡ�
History:
************************************************/
char *TimeVarConvert(unsigned long ulSec);
/************************************************       
Function:TimeVarConvert
Description:
	תΪ��1970��Ϊ��ʼ������Ϊ�������޷ָ���
Input:
unsigned long ulSec ����
Output:
Returns:char *
	����ת������ִ��׵�ַ���û���Ҫ��ʱ����
������ڶ��߳���ʹ����Ҫע��ԭ���ԣ�������
�������ҡ�
History:
************************************************/
char *TimeVarGetYearMonth(unsigned long ulSec);

/************************************************       
Function:GetCurrentSystemSecond
Description:
	��1970��Ϊ��ʼ������
Input:
Output:
Returns:unsigned int
History:
************************************************/
unsigned int GetCurrentSystemSecond();


/************************************************       
Function:GetCurrentTimestaep
Description:
	��ȡϵͳ��ǰʱ��
Input:
Output:
Returns:char *
History:
************************************************/
char *GetCurrentTimestaep();

/************************************************       
Function:GetCurrentYearMonth
Description:
	��ȡϵͳ��ǰ���£����� 201401
Input:
Output:
Returns:char *
History:
************************************************/
char *GetCurrentYearMonth();
#endif