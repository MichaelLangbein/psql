<?php
$dbcreds = array(
	"host" => "localhost",
	"usr" => "root",
	"pw" => "rinso86",
	"db" => "hnddat"
);

$queries = array(
	"a" => array(
			"query" => " select a.Pegelnummer, a.Werte_W, a.Diskretisierung, a.Datum from hnddat.pegeldaten as a where a.Pegelnummer = 10043708  LIMIT 100",
			"csvdata" => array(
				"csv" => 1, 
				"discr" => 2, 
				"date" => 3 
			)
	),
	"b" => array(
		"query" => " select a.Pegelnummer, a.Werte_W, a.Diskretisierung, a.Datum from hnddat.pegeldaten as a where a.Pegelnummer = 10043708  LIMIT 100",
		"csvdata" => array(
			"csv" => 1, 
			"discr" => 2, 
			"date" => 3 
		)
	),
	"c" => array(
		"query" => " select a.Pegelnummer, a.Werte_W, a.Diskretisierung, a.Datum from hnddat.pegeldaten as a where a.Pegelnummer = 10043708  LIMIT 100",
		"csvdata" => array(
			"csv" => 1, 
			"discr" => 2, 
			"date" => 3 
		)
	)
);

$result = doqueries($dbcreds, $queries);
var_dump($result);
?>
