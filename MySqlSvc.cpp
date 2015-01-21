/********************************************************
Copyright owned by Shanghai WithNet ScienTech Co. (Ltd.)
File name:MySqlSvc.cpp
Author:qingshan peng  Version:1.0  Date:2014/10/09
Description:
	������MYSQL���ݿ����ʵ���ļ�
Dependencies:
Function List: 
History:
Author    Date      Detail
*********************************************************/
#include "MySqlSvc.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#if defined WIN32 || defined _WIN32
#ifndef _snprintf
#define _snprintf sprintf_s
#endif

#ifndef _strncpy
#define _strncpy strcpy_s
#endif

#else

#ifndef _snprintf
#define _snprintf snprintf
#endif

#ifndef _strncpy
#define _strncpy strncpy
#endif

#endif

/************************************************************************/
/*								�ڲ�����	                            */
/************************************************************************/
#define MY_QUERY(sql) \
	int iRes = 0;\
	if (NULL == pConnect->pSqlCont) return MSS_FAILED;\
	iRes = mysql_query(pConnect->pSqlCont, sql);\
	if (0 != iRes)\
	{\
		printf("Query mysql faile!! cause:%s\n",\
			mysql_error(pConnect->pSqlCont));\
		iRes = mysql_errno(pConnect->pSqlCont);\
		if (2103 == iRes || 2006 == iRes)\
		{\
			MSS_Disconnect(pConnect);\
			iRes = MSS_Connect(pConnect);\
		}\
		return MSS_FAILED;\
	}
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
						 char *szDataBaseName)
{
	//�򵥵Ĳ������
	if (strlen(szMySqlUser) > 19 ||
		strlen(szMySqlPassword) > 19 ||
		strlen(szHostName) > 19 ||
		strlen(szDataBaseName) > 19 ||
		NULL == pConnect)
	{
		return MSS_BADPARAM;
	}

	//�������ṹ
	memset(pConnect, 0, sizeof(MYSQL_CONNECT));

	//��ʼ��ֵ
	_snprintf(pConnect->szMySqlUser, sizeof(pConnect->szMySqlUser), "%s", szMySqlUser);
	_snprintf(pConnect->szMySqlPassword, sizeof(pConnect->szMySqlPassword), "%s", szMySqlPassword);
	_snprintf(pConnect->szHostName, sizeof(pConnect->szHostName), "%s", szHostName);
	pConnect->usPort = usPort;
	_snprintf(pConnect->szDataBaseName, sizeof(pConnect->szDataBaseName), "%s", szDataBaseName);
	pConnect->pNoChange = pConnect;

	return MSS_OK;
}

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
							char *szDataBaseName)
{
	//�򵥵Ĳ������
	if (strlen(szMySqlUser) > 19 ||
		strlen(szMySqlPassword) > 19 ||
		strlen(szHostName) > 19 ||
		strlen(szDataBaseName) > 19 ||
		NULL == ppConnect)
	{
		return MSS_BADPARAM;
	}
	//��������������
	*ppConnect = NULL;

	*ppConnect = new MYSQL_CONNECT;
	if (NULL == *ppConnect)
	{
		//ֱ���˳��ˣ�����������
		abort();
	}
	//�������ú���,������ʧ�ܡ�
	return	MSS_Config(*ppConnect,
						szMySqlUser,
						szMySqlPassword,
						szHostName,
						usPort,
						szDataBaseName);
}
/************************************************       
Function:MSS_Release
Description:
	�ͷ���MSS_Create�����Ľṹ
Input:
MYSQL_CONNECT ** ppConnect ָ��ṹָ���ָ��
Output:
Returns:eMSS_RC
MSS_OK*
MSS_STRUCTUNINIT*
History:
************************************************/
eMSS_RC MSS_Release(MYSQL_CONNECT **pConnect)
{
	if (NULL == *pConnect)
	{
		return MSS_OK;
	}
	assert(*pConnect == (*pConnect)->pNoChange);
	if (*pConnect != (*pConnect)->pNoChange)
	{
		return MSS_STRUCTUNINIT;
	}

	MSS_Disconnect(*pConnect);

	delete *pConnect;

	*pConnect = NULL;

	return MSS_OK;
}
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
MSS_STRUCTUNINIT*
History:
************************************************/
eMSS_RC MSS_Connect(MYSQL_CONNECT *pConnect)
{
	if (NULL == pConnect)
	{
		return MSS_BADPARAM;
	}
	assert(pConnect == (pConnect)->pNoChange);
	if (pConnect != (pConnect)->pNoChange)
	{
		return MSS_STRUCTUNINIT;
	}

	pConnect->pSqlCont = mysql_init(NULL);

	if (!mysql_real_connect(
			pConnect->pSqlCont,
			pConnect->szHostName,
			pConnect->szMySqlUser,
			pConnect->szMySqlPassword,
			pConnect->szDataBaseName,
			pConnect->usPort,
			NULL,
			0))
	{
		//����ʧ��
		printf("mysql_real_connect Faile Code:%s",
				mysql_error(pConnect->pSqlCont));
		mysql_close(pConnect->pSqlCont);
		pConnect->pSqlCont = NULL;
		return MSS_FAILED;
	}
	mysql_set_character_set(pConnect->pSqlCont, "utf8");
	return MSS_OK;
}

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
MSS_STRUCTUNINIT*
History:
************************************************/
eMSS_RC MSS_Disconnect(MYSQL_CONNECT *pConnect)
{
	if (NULL == pConnect)
	{
		return MSS_BADPARAM;
	}

	assert(pConnect == (pConnect)->pNoChange);
	if (pConnect != (pConnect)->pNoChange)
	{
		return MSS_STRUCTUNINIT;
	}

	if (NULL == pConnect->pSqlCont)
	{
		//�Ѿ��ر���
		return MSS_OK;
	}
	mysql_close(pConnect->pSqlCont);
	pConnect->pSqlCont = NULL;
	return MSS_OK;
}

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
MSS_BADPARAM ��������
MSS_STRUCTUNINIT*
History:
************************************************/
eMSS_RC MSS_CheckServerIsOnline(MYSQL_CONNECT *pConnect)
{
// 	static char szSql[20];
// 
// 	assert(pConnect == (pConnect)->pNoChange);
// 	if (pConnect != (pConnect)->pNoChange)
// 	{
// 		return MSS_STRUCTUNINIT;
// 	}
// 
// 	if (NULL == pConnect->pSqlCont)
// 	{
// 		//���Ӿ��Ϊ�� ֱ����Ϊ�ǶϿ���
// 		return MSS_FAILED;
// 	}
// 
// 	_snprintf(szSql, sizeof(szSql), "use %s", pConnect->szDataBaseName);
// 	//����ѡ�����ݿ�����������Ƿ�����
// 	if (0 != mysql_query(pConnect->pSqlCont, szSql))
// 	{
// 
// 		return MSS_FAILED;
// 	}
	
	return MSS_OK;
}
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
eMSS_RC MSS_GetData(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, MYSQL_RES **ppOutRes)

{
	//�������
	if (NULL == pConnect ||
		NULL == szDataField ||
		NULL == *ppOutRes)
	{
		return MSS_BADPARAM;
	}

	assert(pConnect == (pConnect)->pNoChange);
	if (pConnect != (pConnect)->pNoChange)
	{
		return MSS_STRUCTUNINIT;
	}
	//��������Ƿ�����

	if (MSS_OK != MSS_CheckServerIsOnline(pConnect))
	{
		//�����쳣�˳��Թر�������ӡ�
		MSS_Disconnect(pConnect);
		return MSS_DISCONNECT;
	}
	//�����Ͽ�ʼ��ѯ
	char szSQL[255];

	if (NULL == szDataField)
	{
		_snprintf(szSQL, sizeof(szSQL), "select * from %s", szDataField, szDataTable);
	}
	else
	{
		_snprintf(szSQL, sizeof(szSQL), "select %s from %s", szDataField, szDataTable);
	}
	MY_QUERY(szSQL);

	*ppOutRes = mysql_store_result(pConnect->pSqlCont);
	return MSS_OK;
}

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
eMSS_RC MSS_GetDataEx(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, char *szExLimit, MYSQL_RES **ppOutRes)
{
	return MSS_GetDataPreEx(pConnect, szDataTable, szDataField, NULL, szExLimit, ppOutRes);
}
/************************************************       
Function:MSS_GetDataPreEx
Description:
	MSS_GetDataEx
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
eMSS_RC MSS_GetDataPreEx(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, char *szPreLimit, char *szExLimit, MYSQL_RES **ppOutRes)
{
	//�������
	if (NULL == pConnect ||
		NULL == szDataField ||
		NULL == ppOutRes)
	{
		return MSS_BADPARAM;
	}

	assert(pConnect == (pConnect)->pNoChange);
	if (pConnect != (pConnect)->pNoChange)
	{
		return MSS_STRUCTUNINIT;
	}
	//��������Ƿ�����

	if (MSS_OK != MSS_CheckServerIsOnline(pConnect))
	{
		//�����쳣�˳��Թر�������ӡ�
		MSS_Disconnect(pConnect);
		return MSS_DISCONNECT;
	}
	//�����Ͽ�ʼ��ѯ
	char szSelSQL[50] = "select";
	if (szPreLimit && strlen(szPreLimit) > 0)
	{
		_snprintf(szSelSQL, sizeof(szSelSQL), "select %s", szPreLimit);
	}

	char szSQL[255];
	if (NULL == szExLimit)
	{
		_snprintf(szSQL, sizeof(szSQL), "%s %s from %s", szSelSQL, szDataField, szDataTable);
	}
	else
	{
		_snprintf(szSQL, sizeof(szSQL), "%s %s from %s %s", szSelSQL, szDataField, szDataTable, szExLimit);
	}
	
	MY_QUERY(szSQL);
	*ppOutRes = mysql_store_result(pConnect->pSqlCont);
	return MSS_OK;
}

/************************************************       
Function:MSS_ReleaseData
Description:
	�ͷŴ�mysql�õ��Ľ����
Input:
MYSQL_RES * pOutRes �����ָ��
Output:
Returns:eMSS_RC
MSS_OK*
History:
************************************************/
void MSS_ReleaseData(MYSQL_RES *pOutRes)
{
	mysql_free_result(pOutRes);
}
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
eMSS_RC MSS_InsertData(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, char *szDataValues)
{
	//�������
	if (NULL == pConnect ||
		NULL == szDataValues)
	{
		return MSS_BADPARAM;
	}

	if (pConnect != (pConnect)->pNoChange)
	{
		return MSS_STRUCTUNINIT;
	}
	//��������Ƿ�����
	//note:Ϊ�˽��ͷ���ȡ�����´��� [10/30/2014 qingshan peng]
// 	if (MSS_OK != MSS_CheckServerIsOnline(pConnect))
// 	{
// 		//�����쳣�˳��Թر�������ӡ�
// 		MSS_Disconnect(pConnect);
// 		return MSS_DISCONNECT;
// 	}

	char szSQL[255];
	if (NULL == szDataField)
	{
		_snprintf(szSQL, sizeof(szSQL), "insert into %s value(%s)", szDataTable, szDataValues);
	}
	else
	{
		_snprintf(szSQL, sizeof(szSQL), "insert into %s(%s)  value(%s)", szDataTable, szDataField, szDataValues);
	}

	MY_QUERY(szSQL);

	return MSS_OK;
}
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
MSS_OK ɾ���ɹ�
MSS_FAILED ɾ��ʧ��
MSS_DISCONNECT*
MSS_BADPARAM*
History:
************************************************/
eMSS_RC MSS_DeleteData(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDelLimit)
{
	//�������
	if (NULL == pConnect)
	{
		return MSS_BADPARAM;
	}

	assert(pConnect == (pConnect)->pNoChange);
	if (pConnect != (pConnect)->pNoChange)
	{
		return MSS_STRUCTUNINIT;
	}

	if (MSS_OK != MSS_CheckServerIsOnline(pConnect))
	{
		//�����쳣�˳��Թر�������ӡ�
		MSS_Disconnect(pConnect);
		return MSS_DISCONNECT;
	}
	char szSQL[255];
	_snprintf(szSQL, sizeof(szSQL), "delete from %s where %s", szDataTable, szDelLimit);
	MY_QUERY(szSQL);

	return MSS_OK;
}

eMSS_RC MSS_TruncateTable(MYSQL_CONNECT *pConnect, char *szDataTable)
{
	//�������
	if (NULL == pConnect ||
		NULL == szDataTable)
	{
		return MSS_BADPARAM;
	}

	if (pConnect != (pConnect)->pNoChange)
	{
		return MSS_STRUCTUNINIT;
	}
	//��������Ƿ�����
	char szSQL[255];
	_snprintf(szSQL, sizeof(szSQL),"TRUNCATE TABLE %s", szDataTable);
	MY_QUERY(szSQL);
	return MSS_OK;
}

/************************************************       
Function:MSS_DoQuery
Description:
Input:
MYSQL_CONNECT * pConnect
char * szSQL
Output:
Returns:eMSS_RC
History:
************************************************/
eMSS_RC MSS_DoQuery(MYSQL_CONNECT *pConnect, char *szSQL)
{
	//�������
	if (NULL == pConnect)
	{
		return MSS_BADPARAM;
	}

	if (pConnect != (pConnect)->pNoChange)
	{
		return MSS_STRUCTUNINIT;
	}
	//��������Ƿ�����

	MY_QUERY(szSQL);

	return MSS_OK;
}

#include "sysinc.h"
#ifdef WIN32
#define MY_LOCK HANDLE
#define MY_EVENT HANDLE
#else
#define MY_LOCK pthread_mutex_t
#define MY_EVENT pthread_cond_t
#endif // WIN32

bool g_bInitArry;
unsigned char ucID;
MY_LOCK g_Lock;
MY_LOCK g_Lock0;
MY_EVENT g_Event;
MY_EVENT g_Event0;//������
//queue<string> lsSQLArry;

struct Item 
{
	char szSQL[2048];
	Item *pNext;
};

Item *pHead;

char szSQLSet[1024 * 300];
unsigned short usNum;
#ifdef WIN32
void ThreadGetSql(void *pP)
#else
void * ThreadGetSql(void *pP)
#endif
{
	MYSQL_CONNECT *pConnect = (MYSQL_CONNECT *)pP;
	Item *pItem;
	printf("%s\n", "start");
	//string SQL;
	while(1)
	{
#ifdef WIN32
		WaitForSingleObject(g_Event, INFINITE);
		WaitForSingleObject(g_Lock, INFINITE);

		while (pHead)
		{
			pItem = pHead;
			pHead = pHead->pNext;
			++usNum;
			//sprintf(szSQLSet + strlen(szSQLSet), "%s;", pItem->szSQL);
			MSS_DoQuery(pConnect, pItem->szSQL);
			delete pItem;
		}
		ReleaseMutex(g_Lock);
#else
		pthread_cond_wait(& g_Event, &g_Lock0);
		pthread_mutex_lock(& g_Lock);

		while (pHead)
		{
			pItem = pHead;
			pHead = pHead->pNext;
			//sprintf(szSQLSet + strlen(szSQLSet), "%s;", pItem->szSQL);
			//printf("%s\n", pItem->szSQL);
			MSS_DoQuery(pConnect, pItem->szSQL);
			delete pItem;
			
		}
		pthread_mutex_unlock(&g_Lock);//�ͷŻ�����

#endif // WIN32
	}

}
void OpenOnceMysqlThread(void *pP)
{
	if (!g_bInitArry)
	{
#ifdef WIN32
		g_Lock = CreateMutex(NULL, FALSE, NULL);
		g_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
		_beginthread(ThreadGetSql, 0, pP);
#else
		pthread_mutex_init(& g_Lock, 0);
		pthread_mutex_init(& g_Lock0, 0);
		pthread_cond_init(& g_Event, 0);
		pthread_t ntid;
		int err;
		//printf("isdebug\n");
		err = pthread_create(&ntid, 0, ThreadGetSql, pP);
		if(err != 0){
			printf("can't create thread: %s\n",strerror(err));
			return;
		}
		void *state;
		//pthread_join(ntid, &state);
#endif // WIN32
	}
	return;
}

void MSS_PutSQL(char *Sql)
{
//	string sSQL = Sql;
	Item *pItem = new Item;
	memset(pItem, 0 , sizeof(Item));
	strcpy(pItem->szSQL, Sql);
	//printf("%s11\n", Sql);
#ifdef WIN32
	WaitForSingleObject(g_Lock, INFINITE);
	pItem->pNext = pHead;
	pHead = pItem;
	ReleaseMutex(g_Lock);
	SetEvent(g_Event);
#else
	pthread_mutex_lock(& g_Lock);
	pItem->pNext = pHead;
	pHead = pItem;
	pthread_mutex_unlock(& g_Lock);
	pthread_cond_signal(& g_Event);
	//printf("%s122\n", Sql);
#endif // WIN32
	return;
}
