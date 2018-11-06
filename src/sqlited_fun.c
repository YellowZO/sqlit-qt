#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
                                                                                                    
#include "sqlite3/sqlite3.h"
#include "sqlited.h"

/*全局用于socket的变量*/
struct sockaddr_in servaddr, cliaddr;
socklen_t cliaddr_len;
int listenfd, connfd;
char str[INET_ADDRSTRLEN];

void open_db(const char *filename, sqlite3 **ppDb)
{
	  int rc;
   	rc = sqlite3_open(filename, ppDb);
    if(rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*ppDb));
        exit(0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }
}

void run_sql(char *sql, sqlite3 **ppDb)
{
	int rc,col,i;
	char *ErrMsg = 0;
    sqlite3_stmt *stmt;
    char buffer[MAXSQL]="\0";

    printf("%s\n", sql);
    rc = sqlite3_exec(*ppDb, sql, callback, buffer, &ErrMsg);
	  if( rc != SQLITE_OK )
	  {
        sprintf(buffer, "error: %s\n", ErrMsg);
        sqlite3_free(ErrMsg);
    }
    write(connfd, buffer, MAXSQL);
    close(connfd);
}
/*回调函数,每条select结果执行一次*/
static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    char *buffer = (char*)data;
    /*将结果存入数组中统一发送*/
    if(buffer[0]=='\0')
    {
        for(i=0; i<argc; i++)
        {
            strcat(buffer, azColName[i]);
            strcat(buffer, " ");
        }
        strcat(buffer, "\n");
    }
    for(i=0; i<argc; i++)
    {
        strcat(buffer, argv[i] ? argv[i] : "NULL");
        strcat(buffer, " ");
    }
    strcat(buffer, "\n");
    return 0;
}

int socket_init()
{
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 20);
    printf("Accepting connections ...\n");
    return 0;
}

int get_sql_from_socket(char *sql)
{
    int i, n;
    cliaddr_len = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
    n = read(connfd, sql, MAXSQL);
    sql[n] = '\0';
    printf("received from %s at PORT %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));
}