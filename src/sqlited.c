#include <stdio.h>
#include <stdlib.h>

#include "sqlite3/sqlite3.h"
#include "sqlited.h"

int main(int argc, char* argv[])
{
    sqlite3 *db;
    /*使用这个数组缓存接收到的SQL语句*/
    char sql[MAXSQL];
    /*打开或创建数据库,使用参数argv[1]作为数据库路径*/
   	open_db(argv[1], &db);
   	/*初始化socket*/
   	socket_init();
    while(1)
    {
        get_sql_from_socket(sql);
        run_sql(sql, &db);
    }
    return 0;
}