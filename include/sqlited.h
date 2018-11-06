#ifndef SQLITED_H
#define SQLITED_H
#ifdef __cplusplus
extern "C" {
#endif

#include <netinet/in.h>

#define MAXSQL 1024
#define SERV_PORT 8000

extern struct sockaddr_in servaddr, cliaddr;
extern socklen_t cliaddr_len;
extern int listenfd, connfd;
extern char str[INET_ADDRSTRLEN];

/*操作sqlite的函数*/
void open_db(const char *filename, sqlite3 **ppDb);
void run_sql(char *sql, sqlite3 **ppDb);
static int callback(void *data, int argc, char **argv, char **azColName);

/*scoket通信的函数*/
int socket_init();
int get_sql_from_socket(char *sql);

#ifdef __cplusplus
}
#endif
#endif