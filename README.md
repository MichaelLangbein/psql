PSQL : Short-Term-Goals:
========================

1. Funktion error aus myincl rauszunehmen und für php umschreiben
2. Versichere, dass wechsel von malloc zu emalloc gut geklappt hat. 
3. Übergebe die db-creds als persistente Parameter. MaW: sollen einmal pro Apache-Prozess von php aus
   gesetzt werden, und danach (als globals?) innerhalb des Moduls fortbestehen. 
4. Zend-Engine erwartet immer noch nur einen Eingangsparameter, mit den dbcreds haben wir aber 2. Wo kann man das anpassen?


PSQL : Long-Term-Goals:
========================

1. Abfrage beliebiger queries in parallel
-----------------------------------------

	$dbcreds = array(
		"host" => "localhost",
		"usr" => "root",
		"pw" => "rinso86",
		"db" => "hnddat"
	);
	
	$queries = array(
		array(
				"query" => " select a.Pegelnummer, a.Werte_W, a.Diskretisierung, a.Datum from hnddat.pegeldaten as a where a.Pegelnummer = 10043708  LIMIT 100",
				"csvdata" => array(
					"csv" => 1, 
					"discr" => 2, 
					"date" => 3 
				)
		),
		array(
			"query" => " select a.Pegelnummer, a.Werte_W, a.Diskretisierung, a.Datum from hnddat.pegeldaten as a where a.Pegelnummer = 10043708  LIMIT 100",
			"csvdata" => array(
				"csv" => 1, 
				"discr" => 2, 
				"date" => 3 
			)
		),
		array(
			"query" => " select a.Pegelnummer, a.Werte_W, a.Diskretisierung, a.Datum from hnddat.pegeldaten as a where a.Pegelnummer = 10043708  LIMIT 100",
			"csvdata" => array(
				"csv" => 1, 
				"discr" => 2, 
				"date" => 3 
			)
		)
	);
	
	$result = doqueries(dbcreds, $queries);
	var_dump($result);



2. Nur ein query, beutzen C vor allem, um schneller CSV splitten zu können
--------------------------------------------------------------------------

	$query = array(
        	'query' = 'select Pegelnummer., Werte_W from hnddat',
	        "csvdata" => array(
				"csv" => 1, 
				"discr" => 2, 
				"date" => 3 
			)
	);

