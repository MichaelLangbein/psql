<?php
$queries = array(
		" select a.Pegelnummer, a.Werte_W from hnddat.pegeldaten as a where a.Pegelnummer = 10043708 AND DATUM <= \"2015-05-10\" LIMIT 10",
                " select a.Pegelnummer, a.Werte_W from hnddat.pegeldaten as a where a.Pegelnummer = 13206004 AND DATUM <= \"2015-05-10\" LIMIT 10",
                " select a.Pegelnummer, a.Werte_W from hnddat.pegeldaten as a where a.Pegelnummer = 13208000 AND DATUM <= \"2015-05-10\" LIMIT 10"
);
$result = doqueries($queries);
var_dump($result);
?>
