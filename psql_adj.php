<?php
$queries = array(
		" select a.Werte_W from hnddat.pegeldaten as a where a.Pegelnummer = 10041906 ",
                " select a.Werte_W from hnddat.pegeldaten as a where a.Pegelnummer = 10024000 ",
                " select a.Werte_W from hnddat.pegeldaten as a where a.Pegelnummer = 10030149 "
);
$result = doqueries($queries);
var_dump($result);
?>
