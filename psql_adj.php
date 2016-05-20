<?php
$queries = array(
		" select a.Pegelnummer, a.Werte_W from hnddat.pegeldaten as a where a.Pegelnummer = 10043708  LIMIT 100",
                " select a.Pegelnummer, a.Werte_W from hnddat.pegeldaten as a where a.Pegelnummer = 13206004  LIMIT 100",
                " select a.Pegelnummer, a.Werte_W from hnddat.pegeldaten as a where a.Pegelnummer = 13208000  LIMIT 100"
);
$result = doqueries($queries);
var_dump($result);
?>
