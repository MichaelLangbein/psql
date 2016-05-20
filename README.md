PSQL : Short-Term-Goals:
========================

1. Funktion error aus myincl rauszunehmen und für php umschreiben
2. Übergebe die db-creds als persistente Parameter. MaW: sollen einmal pro Apache-Prozess von php aus
   gesetzt werden, und danach (als globals?) innerhalb des Moduls fortbestehen. 
3. Welche Form müssen die Ausgabedaten haben?
3.1. Funktionen in C können keine Arrays ausgeben. Die threads übergeben anstelle dessen einen pointer zum Ergebnis. 
     Die eigentlichen Daten, weil im stack des threads, werden gelöscht. 
     Wir müssen darum für den array memory auf dem heap reservieren. Der zurückgegebene pointer weist dann auf den heap.
     Was ausgegeben wird ist ein pointer zu einem 2D-Array von strings. Das wäre ein char *** cmtrx_ppp.
3.2. Eine gute Sache ist jedenfalls die: MySQL-API gibt immer strings aus, und für jede Spalte können wir die max Länge bestimmen.
     Wir wissen also wieviel memory wir benötigen werden. Siehe das Projekt "pthread_sql_test" für einen Prototypen.

PSQL : Long-Term-Goals:
========================

1. Abfrage beliebiger queries in parallel
-----------------------------------------

	$queries = array(
       		array(
                	'query' => 'select * from muppets',
                	'cols' => array('int', 'string', 'string')
        		),
        	array(
                	'query' => 'select Pegelnummer, Werte_W from hnddat',
                	'cols' => array('int', 'csv')
        	)
	)
	$result = doqueries($queries);

Anmerkung: 'csv' ist natürlich kein Datentyp. Viel mehr soll das Modul die mit 'csv' markierte Spalte
erkennen und in Reihen aufsplitten. 


2. Nur ein query, beutzen C vor allem, um schneller CSV splitten zu können
--------------------------------------------------------------------------

	$query = array(
        	'query' = 'select Pegelnummer., Werte_W from hnddat',
	        'cols' = array('int', 'csv')
	);

