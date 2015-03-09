/********************************************************
Copyright owned by Shanghai WithNet ScienTech Co. (Ltd.)
File name:MySqlSvc.cpp
Author:qingshan peng  Version:2.3  Date:2015/03/09
Description:
	基础的MYSQL数据库服务头文件，为不同模块提供稍便利的数据查询
功能
Dependencies:
Function List: 
History:
Author			Date		Detail
qingshan peng	20141223	增加带前置条件的MSS_GetDataPreEx函数.
qingshan peng	20150108	脱离gint_log.h依赖，直接采用printf
qingshan peng   20150126	对于数据的插入采用手动提交的方式
							新增MSS_Insert函数，本方法会根据配置
							的提交上限主动提交插入事务。
qingshan peng	20150309	修改MSS_InsertData始终返回正常的bug,
							改为返回MSS_Insert。
*********************************************************/
#include "MySqlSvc.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
/*								内部工具	                            */
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

eMSS_RC MSS_Insert(MYSQL_CONNECT *pConnect, char *szSql)
{
	int iRes = 0;
	time_t timer;//time_t就是long int 类型
	time(&timer);
	if (NULL == pConnect->pSqlCont) return MSS_FAILED;

	iRes = mysql_query(pConnect->pSqlCont, szSql);
	if (0 != iRes)
	{
		printf("Query mysql faile! cause:%s\n", mysql_error(pConnect->pSqlCont));
		printf("%s\n", szSql);
		iRes = mysql_errno(pConnect->pSqlCont);
		if (2103 == iRes || 2006 == iRes)
		{
			MSS_Disconnect(pConnect);
			iRes = MSS_Connect(pConnect);
		}
		return MSS_FAILED;
	}

	++pConnect->uiQueryCount;
	if (pConnect->uiQueryCount >= pConnect->uiCommitNumCfg)
	{
		mysql_commit(pConnect->pSqlCont);
		pConnect->uiQueryCount = 0;
		pConnect->uiCommitTimer = (unsigned int)timer + 120;//延时2分钟
	}
	else
	{
		//没有到数量上限则看是否超过1分钟没有提交了。
		if (pConnect->uiCommitTimer < timer)
		{
			mysql_commit(pConnect->pSqlCont);
			pConnect->uiCommitTimer = (unsigned int)timer;
		}
	}
	return MSS_OK;
}
/************************************************       
Function:MSS_Config
Description:
	配置一个MYSQL_CONNECT结构
Input:
MYSQL_CONNECT * pConnect 结构指针
char * szMySqlUser	mysql用户名，最长19个字符下同
char * szMySqlPassword mysql用户密码
char * szHostName	mysql主机名
unsigned short usPort	主机端口
char * szDataBaseName	数据库名称
unsigned int uiCommitNumCfg		手动提交的上限值
Output:
Returns:eMSS_RC
MSS_OK*
MSS_BADPARAM*
History:
qingshan peng   20150126	添加手动提交插入上限的配置参数
************************************************/
eMSS_RC MSS_Config(MYSQL_CONNECT *pConnect,
						 char *szMySqlUser,
						 char *szMySqlPassword,
						 char *szHostName,
						 unsigned short usPort,
						 char *szDataBaseName,
						 unsigned int uiCommitNumCfg)
{
	//简单的参数检查
	if (strlen(szMySqlUser) > 19 ||
		strlen(szMySqlPassword) > 19 ||
		strlen(szHostName) > 19 ||
		strlen(szDataBaseName) > 19 ||
		NULL == pConnect)
	{
		return MSS_BADPARAM;
	}

	//清空这个结构
	memset(pConnect, 0, sizeof(MYSQL_CONNECT));

	//开始赋值
	_snprintf(pConnect->szMySqlUser, sizeof(pConnect->szMySqlUser), "%s", szMySqlUser);
	_snprintf(pConnect->szMySqlPassword, sizeof(pConnect->szMySqlPassword), "%s", szMySqlPassword);
	_snprintf(pConnect->szHostName, sizeof(pConnect->szHostName), "%s", szHostName);
	pConnect->usPort = usPort;
	_snprintf(pConnect->szDataBaseName, sizeof(pConnect->szDataBaseName), "%s", szDataBaseName);
	pConnect->uiCommitNumCfg = uiCommitNumCfg;

	pConnect->pNoChange = pConnect;

	return MSS_OK;
}

/************************************************       
Function:MSS_Create
Description:
	创建一个MYSQL_CONNECT结构并配置为输入的参数，
你需要显示调用MSS_Release方法来释放创建
的结构。
Input:
MYSQL_CONNECT **ppConnect 用于接收创建的结构
char * szMySqlUser	mysql用户名，最长19个字符下同
char * szMySqlPassword mysql用户密码
char * szHostName	mysql主机名
unsigned short usPort	主机端口
char * szDataBaseName	数据库名称
unsigned int uiCommitNumCfg	手动提交的上限值
Output:
Returns:eMSS_RC
MSS_OK*
MSS_BADPARAM*
History:
qingshan peng   20150126	添加手动提交插入上限的配置参数
************************************************/
eMSS_RC MSS_Create(MYSQL_CONNECT **ppConnect,
							char *szMySqlUser,
							char *szMySqlPassword,
							char *szHostName,
							unsigned short usPort,
							char *szDataBaseName,
							unsigned int uiCommitNumCfg)
{
	//简单的参数检查
	if (strlen(szMySqlUser) > 19 ||
		strlen(szMySqlPassword) > 19 ||
		strlen(szHostName) > 19 ||
		strlen(szDataBaseName) > 19 ||
		NULL == ppConnect)
	{
		return MSS_BADPARAM;
	}
	//这句代码会带来错误？
	*ppConnect = NULL;

	*ppConnect = new MYSQL_CONNECT;
	if (NULL == *ppConnect)
	{
		//直接退出了，几乎不可能
		abort();
	}
	//调用配置函数,不可能失败。
	return	MSS_Config(*ppConnect,
						szMySqlUser,
						szMySqlPassword,
						szHostName,
						usPort,
						szDataBaseName,
						uiCommitNumCfg);
}

/************************************************       
Function:MSS_Release
Description:
	释放由MSS_Create创建的结构
Input:
MYSQL_CONNECT ** ppConnect 指向结构指针的指针
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
	连接指定的MYSQL_CONNECT
Input:
MYSQL_CONNECT * pConnect 连接描述符
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
		//连接失败
		printf("mysql_real_connect Faile Code:%s",
				mysql_error(pConnect->pSqlCont));
		mysql_close(pConnect->pSqlCont);
		pConnect->pSqlCont = NULL;
		return MSS_FAILED;
	}
	mysql_set_character_set(pConnect->pSqlCont, "utf8");
	mysql_autocommit(pConnect->pSqlCont, 0);//关闭自动提交
	return MSS_OK;
}

/************************************************       
Function:MSS_Connect
Description:
	关闭连接
Input:
MYSQL_CONNECT * pConnect 连接描述符
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
		//已经关闭了
		return MSS_OK;
	}
	mysql_close(pConnect->pSqlCont);
	pConnect->pSqlCont = NULL;
	return MSS_OK;
}

/************************************************       
Function:MSS_CheckServerIsOnline
Description:
	检测mysql服务器是否正常连接
Input:
MYSQL_CONNECT * pConnect 连接描述符
Output:
Returns:eMSS_RC
MSS_OK 连接正常
MSS_FAILED 连接不正常
MSS_BADPARAM 参数错误
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
// 		//连接句柄为空 直接认为是断开了
// 		return MSS_FAILED;
// 	}
// 
// 	_snprintf(szSql, sizeof(szSql), "use %s", pConnect->szDataBaseName);
// 	//利用选择数据库来检测连接是否正常
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
	查询数据库,获取结果集，本方法只要在szDataField
指定待查询的字段并按SQL语法排列，不支持where等额外选
择条件。例如：查询test表中，所有的记录，正常SQL语句为
select * form test，而调用本方法只要在szDataField中
赋值"*"或空。
	特别的调用本方法回去的MYSQL_RES结构指针，一定要
调用MSS_ReleaseData方法释放。
Input:
MYSQL_CONNECT * pConnect 连接描述符
char  * szDataTable 需要查询的表
char * szDataField 待查询的字段
Output:
MYSQL_RES * * ppOutRes 返回的结果集指针
Returns:eMSS_RC
MSS_OK 获取成功
MSS_FAILED 获取失败
MSS_DISCONNECT*
MSS_BADPARAM*
History:
************************************************/
eMSS_RC MSS_GetData(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, MYSQL_RES **ppOutRes)

{
	//参数检查
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
	//检查连接是否正常

	if (MSS_OK != MSS_CheckServerIsOnline(pConnect))
	{
		//连接异常了尝试关闭这个连接。
		MSS_Disconnect(pConnect);
		return MSS_DISCONNECT;
	}
	//检查完毕开始查询
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
	MSS_GetData的扩展版支持form后面的限制条件，
szExLimit指定限制条件，遵循SQL语法。
Input:
MYSQL_CONNECT * pConnect 连接描述符
char *szDataTable 待查询的表
char * szDataField 待查询字段
char * szExLimit 限制条件可以为空
MYSQL_RES * * ppOutRes 返回的结果集
Output:
Returns:eMSS_RC
MSS_OK 获取成功
MSS_FAILED 获取失败
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
MYSQL_CONNECT * pConnect 连接描述符
char *szDataTable 待查询的表
char * szDataField 待查询字段
char *szPreLimit select 紧挨着的前置条件如DISTINCT之类
char * szExLimit 限制条件可以为空
MYSQL_RES * * ppOutRes 返回的结果集
Output:
Returns:eMSS_RC
MSS_OK 获取成功
MSS_FAILED 获取失败
MSS_DISCONNECT*
MSS_BADPARAM*
History:
************************************************/
eMSS_RC MSS_GetDataPreEx(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, char *szPreLimit, char *szExLimit, MYSQL_RES **ppOutRes)
{
	//参数检查
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
	//检查连接是否正常

	if (MSS_OK != MSS_CheckServerIsOnline(pConnect))
	{
		//连接异常了尝试关闭这个连接。
		MSS_Disconnect(pConnect);
		return MSS_DISCONNECT;
	}
	//检查完毕开始查询
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
	释放从mysql得到的结果集
Input:
MYSQL_RES * pOutRes 结果集指针
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
	插入一条记录，本方法会自动添加INSERT INTO。
如果添加的是全部的列，你需要将szDataField置空并
在szDataValues中填入要添加的数据并遵循SQL的语法
规则如："值1, 值2, 值3"，如果不是全部的列，那么
需要在szDataField指定数据对应的列如"列1, 列2, 列3"。
Input:
MYSQL_CONNECT * pConnect 连接描述符
char *szDataTable 待插入的表
char * szDataField 列字段
char * szDataValues 值字段
Output:
Returns:eMSS_RC
MSS_OK 插入成功
MSS_FAILED 插入失败
MSS_DISCONNECT*
MSS_BADPARAM*
History:
Author			Date		Detail
qingshan peng	20150309	修改始终返回正常的
							bug,改为返回MSS_Insert
************************************************/
eMSS_RC MSS_InsertData(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, char *szDataValues)
{
	//参数检查
	if (NULL == pConnect ||
		NULL == szDataValues)
	{
		return MSS_BADPARAM;
	}

	if (pConnect != (pConnect)->pNoChange)
	{
		return MSS_STRUCTUNINIT;
	}
	//检查连接是否正常
	//note:为了降低符合取消如下代码 [10/30/2014 qingshan peng]
// 	if (MSS_OK != MSS_CheckServerIsOnline(pConnect))
// 	{
// 		//连接异常了尝试关闭这个连接。
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

	return MSS_Insert(pConnect, szSQL);
}
/************************************************       
Function:MSS_DeleteData
Description:
	删除由szDelLimit指定的数据遵循SQL语法，特别的
不需要携带where。被删数据的表由pConnect隐含指定。
Input:
MYSQL_CONNECT * pConnect
char * szDataTable,
char * szDelLimit
Output:
Returns:eMSS_RC
MSS_OK 删除成功
MSS_FAILED 删除失败
MSS_DISCONNECT*
MSS_BADPARAM*
History:
************************************************/
eMSS_RC MSS_DeleteData(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDelLimit)
{
	//参数检查
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
		//连接异常了尝试关闭这个连接。
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
	//参数检查
	if (NULL == pConnect ||
		NULL == szDataTable)
	{
		return MSS_BADPARAM;
	}

	if (pConnect != (pConnect)->pNoChange)
	{
		return MSS_STRUCTUNINIT;
	}
	//检查连接是否正常
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
	//参数检查
	if (NULL == pConnect)
	{
		return MSS_BADPARAM;
	}

	if (pConnect != (pConnect)->pNoChange)
	{
		return MSS_STRUCTUNINIT;
	}
	//检查连接是否正常

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
MY_EVENT g_Event0;//生产者
//queue<string> lsSQLArry;

struct Item 
{
	char szSQL[2048];
	Item *pNext;

	Item *pFree;
};

Item *pItemPool = NULL;

Item *pHead;

char szSQLSet[1024 * 300];
unsigned short usNum;
void CreateItem(Item **ppItemPool, int iNum)
{
	Item *pNewItem = NULL;
	for (int i = 0;i < iNum; ++i)
	{
		pNewItem = new Item;
		memset(pNewItem, 0, sizeof(Item));
		pNewItem->pFree = (*ppItemPool);
		(*ppItemPool) = pNewItem;
	}
}

Item *GetItem()
{
	Item *pItem = NULL;
	if (!pItemPool)
	{
		CreateItem(&pItemPool, 100);
	}
	pItem = pItemPool;
	pItemPool = pItemPool->pFree;

	pItem->pFree = NULL;
	return pItem;
}

void ReleaseItem(Item *pItem)
{
	pItem->szSQL[0] = 0;
	pItem->pNext = NULL;

	pItem->pFree = pItemPool;
	pItemPool = pItem;
}
#ifdef WIN32
void ThreadGetSql(void *pP)
#else
void * ThreadGetSql(void *pP)
#endif
{
	MYSQL_CONNECT *pConnect = (MYSQL_CONNECT *)pP;
	Item *pItem = NULL;
	Item *pItemExecutedList = NULL;
	printf("%s\n", "start");
	//string SQL;
	while(1)
	{
#ifdef WIN32
		WaitForSingleObject(g_Event, INFINITE);
		WaitForSingleObject(g_Lock, INFINITE);
		pItemExecutedList = pHead;
		pHead = NULL;
		ReleaseMutex(g_Lock);
		while (pItemExecutedList)
		{
			pItem = pItemExecutedList;
			pItemExecutedList = pItemExecutedList->pNext;
			++usNum;
			//sprintf(szSQLSet + strlen(szSQLSet), "%s;", pItem->szSQL);
			MSS_Insert(pConnect, pItem->szSQL);
			ReleaseItem(pItem);
		}
#else
		pthread_cond_wait(& g_Event, &g_Lock0);
		pthread_mutex_lock(& g_Lock);
		pItemExecutedList = pHead;
		pHead = NULL;
		pthread_mutex_unlock(&g_Lock);//释放互斥锁

		while (pItemExecutedList)
		{
			pItem = pItemExecutedList;
			pItemExecutedList = pItemExecutedList->pNext;
			//sprintf(szSQLSet + strlen(szSQLSet), "%s;", pItem->szSQL);
			//printf("%s\n", pItem->szSQL);
			MSS_Insert(pConnect, pItem->szSQL);
			ReleaseItem(pItem);

		}

#endif // WIN32
	}

}
void OpenOnceMysqlThread(void *pP)
{

	//note:初始化一个缓冲池 [1/26/2015 qingshan peng]
	CreateItem(&pItemPool, 2000);

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
		g_bInitArry = true;
	}
	return;
}

void MSS_PutSQL(char *Sql)
{
//	string sSQL = Sql;
	Item *pItem = GetItem();
//memset(pItem, 0 , sizeof(Item));
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
