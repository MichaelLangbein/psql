// Bereits in zend_engine definiert . #define _XOPEN_SOURCE // for mktime, gmtime, strptime, strftime
// Bereits in zend_engine definiert . #define _GNU_SOURCE // for sched_getcpu
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <mysql.h>
#include <time.h>
#include <sched.h>



typedef struct db_creds {
        char *host;
        char *usr;
        char *pw;
        char *db;
        unsigned int port;
        char *socket;
        unsigned int flags;
} db_creds;


typedef struct csv_parameter {
	int csvcol;
	int datecol;
	int discrcol;
} csv_parameter;


typedef struct thread_parameter {
        char *query;
        db_creds *  dbcr;
        csv_parameter * csv;
} thread_parameter;


typedef struct thread_out {
	char *** cmtrx_ppp;
	int num_rows;
	int num_cols;
} thread_out;



MYSQL* do_connect(char *host, char *usr, char *pw, char *db, unsigned int port, char *socket, unsigned int flags);

void do_disconnect(MYSQL *conn);

MYSQL_RES* get_result_set(MYSQL *conn, char *query, int type);

void print_error(MYSQL *conn, char *message);

void error(char * message);



int ** nullTermMtrx();

void printNullTermMtrx(int ** mtrx_pp);

void freeNullTermMtrx(int ** mtrx_pp);

char *** nullTermCmtrx();

void printNullTermCmtrx(char *** cmtrx_ppp);

void freeNullTermCmtrx(char *** cmtrx_ppp);

void freeNullTerm3DCmtrx(char **** cmtrx_pppp);

void printNullTerm3DCmtrx(char **** cmtrx_pppp);

char * cutOffToken(char * string, char * delimiter);

time_t getTstp(char * date, char * format);

char * getDate(time_t tstp, int offset);


char **** allocSplitRows(MYSQL_RES * reslt, int num_rows, int num_cols, csv_parameter * csv);

void* do_query(void* tp);
