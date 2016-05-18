#! /bin/bash

phpize;
./configure;
make;
cp modules/psql.so /usr/local/lib/php;
php -q psql.php;
php -q psql_adj.php;
