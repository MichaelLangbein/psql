#include "meinincl.h"

MYSQL* do_connect(char *host, char *usr, char *pw, char *db, unsigned int port, char *socket, unsigned int flags){

	MYSQL *conn;

	conn = mysql_init(NULL);
	if(conn == NULL){
		print_error(conn, "mysql_init hat nicht funktioniert; vielleicht kein memory?");
		return (NULL);
	}

	/* Connect to database */
	if (mysql_real_connect(conn, host, usr, pw, db, port, socket, flags) == NULL ){
		print_error(conn, "mysql_real_connect hat nicht funktioniert; Stimmen die credentials?");
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
    	print_error(conn, "Fehler bei Datenabfrage. ");
    	return (NULL);
    }

    /* Return pointer to resultset. */
    if(type == 1){
    	res = mysql_store_result(conn);
    }else if(type == 2){
    	res = mysql_use_result(conn);
    }
    if(res == NULL){
    	print_error(conn, "mysql_xxx_result hat nicht funktioniert. ");
    	return (NULL);
    }

    return res;
}

/* MySQL-spezifische Fehlermeldung */
void print_error(MYSQL *conn, char *message){
	fprintf(stderr, "Fehler in PSQL. Fehlernachricht: %s\n", message);

	if(conn != NULL){
		fprintf(stderr, "Fehler Nummer %u in PSQL. Fehlernachricht: %s\n", mysql_errno(conn), mysql_error(conn) );
	}
}


/* Allgemeine Fehlermeldung */
void error(char * message){
        fprintf(stderr, "%s : %s\n", message, strerror(errno));
        exit(1);
}




void printNullTerm3DCmtrx(char **** cmtrx_pppp){
	char *** cmtrx_ppp;
	char ** cmtrx_pp;
	char * cmtrx_p;
	int n, m, l;

	n = 0;
	while(cmtrx_pppp[n] != NULL){
		cmtrx_ppp = cmtrx_pppp[n];

		m = 0;
		while(cmtrx_ppp[m] != NULL){
			cmtrx_pp = cmtrx_ppp[m];

			l=0;
			while(cmtrx_pp[l] != NULL){
				cmtrx_p = cmtrx_pp[l];
				printf("%s ", cmtrx_p);
				l++;
			}
			printf("\n");
			m++;
		}
		n++;
	}
}

void freeNullTerm3DCmtrx(char **** cmtrx_pppp){
	char *** cmtrx_ppp;
	char ** cmtrx_pp;
	char * cmtrx_p;
	int n, m, l;

	n = 0;  // for each mysql-result line ...
	while(cmtrx_pppp[n] != NULL){
		cmtrx_ppp = cmtrx_pppp[n];

		m=0;  // for each csv-element line ...
		while(cmtrx_ppp[m] != NULL){
			cmtrx_pp = cmtrx_ppp[m];

			l=0;  // for each column ...
			while(cmtrx_pp[l] != NULL){
				cmtrx_p = cmtrx_pp[l];
				free(cmtrx_p);
				l++;
			}  // ... end for each column
			//free(cmtrx_pp[l]);
			free(cmtrx_pp);
			m++;
		}   // ... end for each csv element
		//free(cmtrx_ppp[m]);
		free(cmtrx_ppp);
		n++;
	}   // ... end for each mysql-result line
	//free(cmtrx_pppp[n]);
	free(cmtrx_pppp);
}



char * cutOffToken(char * string, char * delimiter){
	char * splitpoint;
	if(!( splitpoint = strchr(string, *delimiter) )){
		return NULL;
	}
	char * rest = &splitpoint[1];
	*splitpoint = '\0';
	return rest;
}


/**
 * char[] => struct tm => time_t
 */
time_t getTstp(char * date, char * format){
	struct tm time;
	time.tm_hour = 0;
	time.tm_min = 0;
	time.tm_sec = 0;
	strptime(date, format, &time);
	time_t time_out = timegm(&time);
	return time_out;
}

/**
 * time_t => struct tm => char[]
 */
char * getDate(time_t tstp, int offset){
	tstp += offset;
	struct tm * time = gmtime(&tstp);
	char datestring[18];
	strftime(datestring, 18, "%Y-%m-%d %H:%M", time);
	char * datestring_p = datestring;
	return datestring_p;
}






char **** allocSplitRows(MYSQL_RES * reslt, int num_rows, int num_cols, csv_parameter * csv){

	MYSQL_ROW row;
	int r, d, c, discr, num_steps;
	char * csvstring;
	char * entry;
	char * date;
	time_t date_tstp;
	char * datetime;
	char datetime_arr[18];


	char **** cmtrx_pppp = (char ****)malloc((num_rows + 1) * sizeof(char ***));

	for (r = 0; r < num_rows; r++) {
		row = mysql_fetch_row(reslt);

		discr = atoi(row[csv->discrcol]);
		csvstring = row[csv->csvcol];
		date = row[csv->datecol];
		date_tstp = getTstp(date, "%Y-%m-%d");

		num_steps = 1440 / discr;
		cmtrx_pppp[r] = (char ***)malloc((num_steps + 1) * sizeof(char **));

		for (d = 0; d < num_steps; d++) {

			entry = csvstring;
			csvstring = cutOffToken(csvstring, ";");
			cmtrx_pppp[r][d] = (char **)malloc( (1 + num_cols) * sizeof(char *) );

			for(c = 0; c<num_cols; c++){
				if(c == csv->csvcol){
					cmtrx_pppp[r][d][c] = (char *)malloc( (strlen(entry) + 1) * sizeof(char) );
					strcpy( cmtrx_pppp[r][d][c], entry );
				}
				else if(c == csv->datecol){
					datetime = getDate(date_tstp, d*discr*60);
					strcpy( datetime_arr, datetime);
					cmtrx_pppp[r][d][c] = (char *)malloc( (strlen(datetime) + 1) * sizeof(char) );
					strcpy( cmtrx_pppp[r][d][c], datetime_arr );
				}
				else{
					cmtrx_pppp[r][d][c] = (char *)malloc( (strlen(row[c]) + 1) * sizeof(char) );
					strcpy( cmtrx_pppp[r][d][c], row[c] );
				}
			}
			cmtrx_pppp[r][d][num_cols] = NULL;
		}
		cmtrx_pppp[r][num_steps] = NULL;
	}
	cmtrx_pppp[num_rows] = NULL;


	return cmtrx_pppp;
}



void* do_query(void* tp){
	/* Jeder thread muss eigene Verbindung aufmachen. 
	   MySQL-Verbindungen arbeiten nämlich Anfragen immer sequentiell ab.
	 */
    thread_parameter * thrpar = (thread_parameter *)tp;
    MYSQL *conn = do_connect(thrpar->dbcr->host, thrpar->dbcr->usr, thrpar->dbcr->pw, thrpar->dbcr->db,
                                 thrpar->dbcr->port, thrpar->dbcr->socket, thrpar->dbcr->flags);
        
	
	char *query = thrpar->query;
	int cpu = sched_getcpu();
    printf("Thread auf core %i wird gleich folgenden Query ausführen:  %s\n", cpu, query);

	/* Wir holen das Ergebnis in Modus "1", also als "store_result". 
	   Wir wollen die Daten nämlich schnell von MySQL-Server runter haben, damit der wieder 
       für andere Dinge platz hat.
	*/
    MYSQL_RES *reslt = get_result_set(conn, query, 1);
    long int num_rows = (long int)mysql_num_rows(reslt);
    int num_cols = mysql_num_fields(reslt);
	
	
	/* Hier kommt ein schwieriger Teil: 
	   Das Ergebnis ist ein array von strings. Wie aber bringen wir diese zurück an main?
    	   Schließlich geben wir nur einen pointer zum Ergebis aus. Das eigentliche Ergebnis
	   wird schon gelöscht sein, wenn der thread endet. 
	   Wir müssen darum einen Platz auf dem Heap frei machen. 
	*/

    char **** cmtrx_pppp = allocSplitRows(reslt, num_rows, num_cols, thrpar->csv);

    mysql_free_result(reslt);
    do_disconnect(conn);
    mysql_thread_end();
 
    return (void*) cmtrx_pppp;
}
