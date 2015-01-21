/********************************************************
Copyright owned by Shanghai WithNet ScienTech Co. (Ltd.)
File name:MySqlSvc.h
Author:qingshan peng  Version:2.1  Date:2015/01/08
Description:
	������MYSQL���ݿ����ͷ�ļ���Ϊ��ͬģ���ṩ�Ա��������ݲ�ѯ
����
Dependencies:
Function List: 
History:
Author			Date		Detail
qingshan peng	20141223	���Ӵ�ǰ��������MSS_GetDataPreEx����.
qingshan peng	20150108	����gint_log.h������ֱ�Ӳ���printf
*********************************************************/
#ifndef __MYSQLSVC_H__
#define __MYSQLSVC_H__
#include "mysql.h"
enum eMSS_RC
{
	MSS_OK,				//�ɹ�
	MSS_FAILED,			//ʧ��
	MSS_DISCONNECT,		//mysql�����Ѿ��Ͽ�
	MSS_BADPARAM,		//һ���������
	MSS_STRUCTUNINIT,	//�����ṹû�б���ʼ��
	MSS_DATAERROR,		//��������
	MSS_BASEERROR,		//���ݿ��д���ִ���
	MSS_invailed		//��Ч��
};


//MYSQL������Ϣ�����ṹ���󶨵����ݱ�
//�û����Զ��岻ͬ�Ľṹ���ڱ�����ͬ������
//��
typedef struct tagMySqlConnect
{
	char szMySqlUser[20];		//20���ַ���mysql�û�
	char szMySqlPassword[20];	//�û�����
	char szHostName[20];		//��������
	unsigned short usPort;		//mysql�˿�
	char szDataBaseName[20];	//�󶨵����ݿ�
	MYSQL *pSqlCont;			//mysql���Ӿ��
	tagMySqlConnect *pNoChange;	//���ṹָ���Ƿ��ƻ�, ͬʱ��Ϊ��ʼ����־

}MYSQL_CONNECT;

/************************************************       
Function:MSS_Config
Description:
	����һ��MYSQL_CONNECT�ṹ
Input:
MYSQL_CONNECT * pConnect �ṹָ��
char * szMySqlUser	mysql�û������19���ַ���ͬ
char * szMySqlPassword mysql�û�����
char * szHostName	mysql������
unsigned short usPort	�����˿�
char * szDataBaseName	���ݿ�����
Output:
Returns:eMSS_RC
MSS_OK*
MSS_BADPARAM*
History:
************************************************/
eMSS_RC MSS_Config(MYSQL_CONNECT *pConnect,
						 char *szMySqlUser,
						 char *szMySqlPassword,
						 char *szHostName,
						 unsigned short usPort,
						 char *szDataBaseName);

/************************************************       
Function:MSS_Create
Description:
	����һ��MYSQL_CONNECT�ṹ������Ϊ����Ĳ�����
����Ҫ��ʾ����MSS_Release�������ͷŴ���
�Ľṹ��
Input:
MYSQL_CONNECT **ppConnect ���ڽ��մ����Ľṹ
char * szMySqlUser	mysql�û������19���ַ���ͬ
char * szMySqlPassword mysql�û�����
char * szHostName	mysql������
unsigned short usPort	�����˿�
char * szDataBaseName	���ݿ�����
Output:
Returns:eMSS_RC
MSS_OK*
MSS_BADPARAM*
History:
************************************************/
eMSS_RC MSS_Create(MYSQL_CONNECT **ppConnect,
							char *szMySqlUser,
							char *szMySqlPassword,
							char *szHostName,
							unsigned short usPort,
							char *szDataBaseName);


/************************************************       
Function:MSS_Release
Description:
	�ͷ���MSS_Create�����Ľṹ
Input:
MYSQL_CONNECT ** ppConnect ָ��ṹָ���ָ��
Output:
Returns:eMSS_RC
MSS_OK*
History:
************************************************/
eMSS_RC MSS_Release(MYSQL_CONNECT **pConnect);


/************************************************       
Function:MSS_Connect
Description:
	����ָ����MYSQL_CONNECT
Input:
MYSQL_CONNECT * pConnect ����������
Output:
Returns:eMSS_RC
MSS_OK* 
MSS_FAILED*
MSS_BADPARAM*
History:
************************************************/
eMSS_RC MSS_Connect(MYSQL_CONNECT *pConnect);
/************************************************       
Function:MSS_Connect
Description:
	�ر�����
Input:
MYSQL_CONNECT * pConnect ����������
Output:
Returns:eMSS_RC
MSS_OK* 
MSS_FAILED*
MSS_BADPARAM*
History:
************************************************/
eMSS_RC MSS_Disconnect(MYSQL_CONNECT *pConnect);

/************************************************       
Function:MSS_CheckServerIsOnline
Description:
	���mysql�������Ƿ���������
Input:
MYSQL_CONNECT * pConnect ����������
Output:
Returns:eMSS_RC
MSS_OK ��������
MSS_FAILED ���Ӳ�����
MSS_BADPARAM*
History:
************************************************/
eMSS_RC MSS_CheckServerIsOnline(MYSQL_CONNECT *pConnect);

/************************************************       
Function:MSS_GetData
Description:
	��ѯ���ݿ�,��ȡ�������������ֻҪ��szDataField
ָ������ѯ���ֶβ���SQL�﷨���У���֧��where�ȶ���ѡ
�����������磺��ѯtest���У����еļ�¼������SQL���Ϊ
select * form test�������ñ�����ֻҪ��szDataField��
��ֵ"*"��ա�
	�ر�ĵ��ñ�������ȥ��MYSQL_RES�ṹָ�룬һ��Ҫ
����MSS_ReleaseData�����ͷš�
Input:
MYSQL_CONNECT * pConnect ����������
char  * szDataTable ��Ҫ��ѯ�ı�
char * szDataField ����ѯ���ֶ�
Output:
MYSQL_RES * * ppOutRes ���صĽ����ָ��
Returns:eMSS_RC
MSS_OK ��ȡ�ɹ�
MSS_FAILED ��ȡʧ��
MSS_DISCONNECT*
MSS_BADPARAM*
History:
************************************************/
eMSS_RC MSS_GetData(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, MYSQL_RES **ppOutRes);

/************************************************       
Function:MSS_GetDataEx
Description:
	MSS_GetData����չ��֧��form���������������
szExLimitָ��������������ѭSQL�﷨��
Input:
MYSQL_CONNECT * pConnect ����������
char *szDataTable ����ѯ�ı�
char * szDataField ����ѯ�ֶ�
char * szExLimit ������������Ϊ��
MYSQL_RES * * ppOutRes ���صĽ����
Output:
Returns:eMSS_RC
MSS_OK ��ȡ�ɹ�
MSS_FAILED ��ȡʧ��
MSS_DISCONNECT*
MSS_BADPARAM*
History:
************************************************/
eMSS_RC MSS_GetDataEx(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, char *szExLimit, MYSQL_RES **ppOutRes);

/************************************************       
Function:MSS_GetDataPreEx
Description:
	MSS_GetDataPreEx����չ��֧��select���������������
szPreLimitָ��������������ѭSQL�﷨��
	��:SELECT DISTINCT * from ...
Input:
MYSQL_CONNECT * pConnect ����������
char *szDataTable ����ѯ�ı�
char * szDataField ����ѯ�ֶ�
char *szPreLimit select �����ŵ�ǰ��������DISTINCT֮��
char * szExLimit ������������Ϊ��
MYSQL_RES * * ppOutRes ���صĽ����
Output:
Returns:eMSS_RC
MSS_OK ��ȡ�ɹ�
MSS_FAILED ��ȡʧ��
MSS_DISCONNECT*
MSS_BADPARAM*
History:
************************************************/
eMSS_RC MSS_GetDataPreEx(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, char *szPreLimit, char *szExLimit, MYSQL_RES **ppOutRes);


/************************************************       
Function:MSS_InsertData
Description:
	����һ����¼�����������Զ����INSERT INTO��
�����ӵ���ȫ�����У�����Ҫ��szDataField�ÿղ�
��szDataValues������Ҫ��ӵ����ݲ���ѭSQL���﷨
�����磺"ֵ1, ֵ2, ֵ3"���������ȫ�����У���ô
��Ҫ��szDataFieldָ�����ݶ�Ӧ������"��1, ��2, ��3"��
Input:
MYSQL_CONNECT * pConnect ����������
char *szDataTable ������ı�
char * szDataField ���ֶ�
char * szDataValues ֵ�ֶ�
Output:
Returns:eMSS_RC
MSS_OK ����ɹ�
MSS_FAILED ����ʧ��
MSS_DISCONNECT*
MSS_BADPARAM*
History:
************************************************/
eMSS_RC MSS_InsertData(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, char *szDataValues);

/************************************************       
Function:MSS_DeleteData
Description:
	ɾ����szDelLimitָ����������ѭSQL�﷨���ر��
����ҪЯ��where����ɾ���ݵı���pConnect����ָ����
Input:
MYSQL_CONNECT * pConnect
char * szDataTable,
char * szDelLimit
Output:
Returns:eMSS_RC
History:
************************************************/
eMSS_RC MSS_DeleteData(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDelLimit);

/************************************************       
Function:MSS_ReleaseData
Description:
	�ͷŴ�mysql�õ��Ľ����
Input:
MYSQL_RES * pOutRes �����ָ��
Output:
Returns:eMSS_RC
MSS_OK*
MSS_FAILED*
History:
************************************************/
void MSS_ReleaseData(MYSQL_RES *pOutRes);

/************************************************       
Function:MSS_TruncateTable
Description:
	
Input:
MYSQL_CONNECT * pConnect
char * szDataTable
Output:
Returns:eMSS_RC
History:
************************************************/
eMSS_RC MSS_TruncateTable(MYSQL_CONNECT *pConnect, char *szDataTable);
eMSS_RC MSS_DoQuery(MYSQL_CONNECT *pConnect, char *szSQL);

void OpenOnceMysqlThread(void *pP);

void MSS_PutSQL(char *Sql);

#endif
