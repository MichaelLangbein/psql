PSQL : Installation 
========================

Anmerkung: Dieses modul wurde für die Thread-save Version von PHP entwickelt (siehe master branch). Der branch 2012 ist ein backport auf php 5.5.9 API 2012-NTS,
welche auf unseren Servern standardmäßig verwendet wird. Vermutlich leakt auch dieser port kein memory, das muss aber noch ausfühlicher getestet werden.

1. Installiere git; eventuell noch für proxy konfigurieren: "git config --global http.proxy http://www.proxy.bybn.de:80"
2. Mit phpinfo() das Extension-Verzeichnis-Suchen. Üblicherweise "/usr/lib/php5/20121212"
3. Kopiere psql/modules/psql.so in Extension-Verzeichnis.
4. php.ini: Hinzufügen: "extension=psql.so"
5. sudo service apache2 restart
6. Testen mit psql.php, psql_adj.php

PSQL : Funktionsweise
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

