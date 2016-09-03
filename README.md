Strategie
=========

Ok, direkte einbindung in php macht probleme.
        Apache läuft derzeit im Prefork Modus, d.h. mit Prozessen, nicht threads.
        Verwendet mod_php5
        Verwendet php in NTS Version
                Anfrage -> 1 Apache Prozess -> 1 NTS Php Prozess -> mehrere threads
                Bei Anfrage stürzt apache mit segfault ab.

Neue herangehensweise.
Schritt eins:
        In Php: erzeuge json-file mit Eingangsdaten
        In Php: exec psql-c-only mitn Eingangsdaten gepiped
        http://php.net/manual/en/function.exec.php
Schritt zwei:
        In C: hole Daten
        In C: pipe Daten wieder an Php
        http://stackoverflow.com/questions/14047979/executing-python-script-in-php-and-exchanging-data-between-the-two
Schritt drei:
        In Php: hole Daten



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

In ungünstigen Fällen weigert sich php mit dieser SO zusammenzuarbeiten. 
Dann lokal Kompilieren: 

0. cd /home/soda/psql
1. sudo apt-get install php5-dev (für phpize)
2. sudo apt-get install libmysqlclient-dev (für libmysqlclient.so)
3. sudo apt-get install valgrind (zum Testen, nur zur Sicherheit)
4. phpize --clean
5. sodo chmod -R 777 /usr/lib/php5 (um Bibliotheken mit einbinden zu können)
6. ./mmod.sh

Wenn auch das nicht funktioniert, dann als dl verwenden. Dazu: 

1. In /etc/php5/apache2/php.ini und /etc/php5/cli/php.ini die Direktive "enable_dl = On" setzen.
2. ./mmod
3. Modul immer erst laden mit dem code-snippet:
	if(!extension_loaded('psql')) {
        	dl('psql.' . PHP_SHLIB_SUFFIX);
	}


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

