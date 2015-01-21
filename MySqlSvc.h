/********************************************************
Copyright owned by Shanghai WithNet ScienTech Co. (Ltd.)
File name:MySqlSvc.h
Author:qingshan peng  Version:2.1  Date:2015/01/08
Description:
	基础的MYSQL数据库服务头文件，为不同模块提供稍便利的数据查询
功能
Dependencies:
Function List: 
History:
Author			Date		Detail
qingshan peng	20141223	增加带前置条件的MSS_GetDataPreEx函数.
qingshan peng	20150108	脱离gint_log.h依赖，直接采用printf
*********************************************************/
#ifndef __MYSQLSVC_H__
#define __MYSQLSVC_H__
#include "mysql.h"
enum eMSS_RC
{
	MSS_OK,				//成功
	MSS_FAILED,			//失败
	MSS_DISCONNECT,		//mysql服务已经断开
	MSS_BADPARAM,		//一般参数错误
	MSS_STRUCTUNINIT,	//描述结构没有被初始化
	MSS_DATAERROR,		//数据有误
	MSS_BASEERROR,		//数据库读写出现错误
	MSS_invailed		//无效的
};


//MYSQL连接信息描述结构，绑定到数据表
//用户可以定义不同的结构用于表述不同的数据
//表。
typedef struct tagMySqlConnect
{
	char szMySqlUser[20];		//20个字符的mysql用户
	char szMySqlPassword[20];	//用户密码
	char szHostName[20];		//主机名称
	unsigned short usPort;		//mysql端口
	char szDataBaseName[20];	//绑定的数据库
	MYSQL *pSqlCont;			//mysql连接句柄
	tagMySqlConnect *pNoChange;	//检测结构指针是否被破坏, 同时作为初始化标志

}MYSQL_CONNECT;

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
	释放由MSS_Create创建的结构
Input:
MYSQL_CONNECT ** ppConnect 指向结构指针的指针
Output:
Returns:eMSS_RC
MSS_OK*
History:
************************************************/
eMSS_RC MSS_Release(MYSQL_CONNECT **pConnect);


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
History:
************************************************/
eMSS_RC MSS_Connect(MYSQL_CONNECT *pConnect);
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
History:
************************************************/
eMSS_RC MSS_Disconnect(MYSQL_CONNECT *pConnect);

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
MSS_BADPARAM*
History:
************************************************/
eMSS_RC MSS_CheckServerIsOnline(MYSQL_CONNECT *pConnect);

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
eMSS_RC MSS_GetData(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, MYSQL_RES **ppOutRes);

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
eMSS_RC MSS_GetDataEx(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, char *szExLimit, MYSQL_RES **ppOutRes);

/************************************************       
Function:MSS_GetDataPreEx
Description:
	MSS_GetDataPreEx的扩展版支持select后面的限制条件，
szPreLimit指定限制条件，遵循SQL语法。
	如:SELECT DISTINCT * from ...
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
eMSS_RC MSS_GetDataPreEx(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, char *szPreLimit, char *szExLimit, MYSQL_RES **ppOutRes);


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
************************************************/
eMSS_RC MSS_InsertData(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDataField, char *szDataValues);

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
History:
************************************************/
eMSS_RC MSS_DeleteData(MYSQL_CONNECT *pConnect, char *szDataTable, char *szDelLimit);

/************************************************       
Function:MSS_ReleaseData
Description:
	释放从mysql得到的结果集
Input:
MYSQL_RES * pOutRes 结果集指针
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
