<?php
$queries = array(
		" select a.Werte_W from hnddat.pegeldaten as a where a.Pegelnummer = 10042104 ",
                " select a.Werte_W from hnddat.pegeldaten as a where a.Pegelnummer = 12006757 ",
                " select a.Werte_W from hnddat.pegeldaten as a where a.Pegelnummer = 13325505 "
);
$result = doqueries($queries);
var_dump($result);
?>
