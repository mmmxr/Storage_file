#include "login_server.h"
#define MAX_LEN 1024

char sql[MAX_LEN];
int InsertSql(login* user)
{
    mysql_init(&m_conn);
    if(!mysql_real_connect(&m_conn,"localhost","root","root","File",3306,NULL,0))
    {
        return -1;//链接失败
    }
    sprintf(sql,"insert into File_Server values(%s,%s);",user->name,user->password);

    if(mysql_query(&m_conn,sql))
    {
        return -1;
    }
    mysql_close(&m_conn);
    return 1;
}
int SelectSql(login* user)
{
    mysql_init(&m_conn);
    if(!mysql_real_connect(&m_conn,"localhost","root","root","File",3306,NULL,0))
    {
        return -1;//链接失败
    }
    sprintf(sql,"select passwd from File_Server where name='%s';",user->name);
    if(mysql_query(&m_conn,sql)) {
        return -1;
    }
    if((res=mysql_store_result(&m_conn)) == NULL )
    {
        perror("mysql_store_result");
        return -1;
    }
    else//保存结果集成功
    {
        while((row=mysql_fetch_row(res)) != NULL )//重复读取行，直到row为NULL
        {
            printf("passwd is %s\n",row[1]);//打印当前行的数据
            if(strcmp(user->password,row[1])==0)
            {
                break;
            }
        }
    }
    //释放结果集
    mysql_free_result(res);
    mysql_close(&m_conn);
    
    return 1;
}
