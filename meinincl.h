#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <mysql.h>

typedef struct {
	char *query;
	char *host;
        char *usr;
        char *pw;
        char *db; 
        unsigned int port;
        char *socket;
        unsigned int flags;
} thread_parameter;


MYSQL* do_connect(char *host, char *usr, char *pw, char *db, unsigned int port, char *socket, unsigned int flags);

void do_disconnect(MYSQL *conn);

MYSQL_RES* get_result_set(MYSQL *conn, char *query, int type);

void mysql_print_error(MYSQL *conn, char *message);

void error(char *msg);

void* do_query(void* tp);
