#include "meinincl.h"

MYSQL* do_connect(char *host, char *usr, char *pw, char *db, unsigned int port, char *socket, unsigned int flags){

	MYSQL *conn;

	conn = mysql_init(NULL);
	if(conn == NULL){
		mysql_print_error(conn, "mysql_init hat nicht funktioniert; vielleicht kein memory?");
		return (NULL);
	}

	/* Connect to database */
	if (mysql_real_connect(conn, host, usr, pw, db, port, socket, flags) == NULL ){
		mysql_print_error(conn, "mysql_real_connect hat nicht funktioniert; Stimmen die credentials?");
		return (NULL);
	}

	return conn;
}

void do_disconnect(MYSQL *conn){
	mysql_close(conn);
}


/**
 * Type == 1: store_result.
 * store_result liefert das Ergebnis direkt und speichert es auf dem Client.
 * store_result ist etwas langsamer, aber blockiert nicht andere Prozesse.
 * Type == 2: use_result.
 * use_result ist schneller und braucht weniger memory, aber blockiert andere Prozesse. Liest Reihen erst aus, wenn nötig.
 * use_result sollte nicht verwendet werden, wenn pro Reihe viel prozessiert werden muss.
 */
MYSQL_RES* get_result_set(MYSQL *conn, char *query, int type){

    MYSQL_RES *res;

    /* send SQL query */
    if (mysql_query(conn, query) != 0) {
    	mysql_print_error(conn, "Fehler bei Datenabfrage. ");
    	return (NULL);
    }

    /* Return pointer to resultset. */
    if(type == 1){
    	res = mysql_store_result(conn);
    }else if(type == 2){
    	res = mysql_use_result(conn);
    }
    if(res == NULL){
    	mysql_print_error(conn, "mysql_xxx_result hat nicht funktioniert. ");
    	return (NULL);
    }

    return res;
}

void mysql_print_error(MYSQL *conn, char *message){
	fprintf(stderr, "Fehler in PSQL. Fehlernachricht: %s\n", message);

	if(conn != NULL){
		fprintf(stderr, "Fehler Nummer %u in PSQL. Fehlernachricht: %s\n", mysql_errno(conn), mysql_error(conn) );
	}
}



void error(char *msg){
	fprintf(stderr, "%s : %s\n", msg, strerror(errno));
	exit(1);
}


void* do_query(void* tp){
	char *host = "localhost";
	char *usr = "root";
	char *pw = "rinso86";
	char *db = "hnddat";
	unsigned int port = 0; 
	char *socket = NULL;
	unsigned int flags = 0;
	MYSQL *conn = do_connect(host, usr, pw, db, port, socket, flags);
	char *query = (char*)tp;
	printf("Thread wird gleich folgenden Query ausführen:  %s\n", query);
	
	MYSQL_RES *reslt = get_result_set(conn, query, 1);
	long int nr = (long int)mysql_num_rows(reslt);
	
	mysql_free_result(reslt);
	do_disconnect(conn);
	mysql_thread_end();
	return (void*)(nr); 
}
