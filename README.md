# psql


Long-Term-Goals:
================

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

