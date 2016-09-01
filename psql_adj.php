<?php
$dbcreds = array(
	"host" => "localhost",
	"usr" => "hnd",
	"pw" => "123shnd4",
	"db" => "hnddat"
);

$queries = array(
	"a" => array(
			"query" => " select a.Pegelnummer, a.Werte_W, a.Diskretisierung, a.Datum from hnddat.pegeldaten as a where a.Pegelnummer = 10043708 and a.Werte_W IS NOT NULL LIMIT 100",
			"csvdata" => array(
				"csv" => 1, 
				"discr" => 2, 
				"date" => 3
			)
	),
	"b" => array(
		"query" => " select a.Pegelnummer, a.Werte_W, a.Diskretisierung, a.Datum from hnddat.pegeldaten as a where a.Pegelnummer = 10053009 and a.Werte_W IS NOT NULL LIMIT 100",
		"csvdata" => array(
			"csv" => 1, 
			"discr" => 2, 
			"date" => 3
		)
	),
	"c" => array(
		"query" => " select a.Pegelnummer, a.Werte_W, a.Diskretisierung, a.Datum from hnddat.pegeldaten as a where a.Pegelnummer = 10046105 and a.Werte_W IS NOT NULL LIMIT 100",
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
