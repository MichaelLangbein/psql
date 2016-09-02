#! /bin/bash

# 1. Apache braucht kein Kompilieren.
#2. Kompilieren von Php
#  2.1. apt-get install libxml2-dev, libmysqlclient-dev,
#  2.2. chmod 777 /usr/local/lib
#  2.3. wget http://de2.php.net/get/php-5.6.25.tar.gz/from/this/mirror
#  2.4. ./configure --enable-debug --enable-maintainer-zts ; make; make install
#  2.5.  cp php.ini-development /usr/local/lib/php.ini
#  2.6. extension=psql.so
#3. Kompilieren der Extension
#  3.0. Falls Installation nötig:
#    3.0.1. ext_skell --module=psql --proto=gekÃrzte headerfile
#    3.0.2. Anpassen der config.m4: mindestens PHP_ARG_ENABLE und PHP_NEW EXTENSION
#  3.1. git clone https://github.com/rinso86/psql.git
#  3.2. ./mmod.sh
#
# Debuggen ist bemerkenswert einfach. 
#	1. gdb /usr/local/bin/php
#	2. (gdb) break psql.c:84
#	3. (gdb) run psql_adj.php

phpdir="/usr/local/lib";
modulesdir="${phpdir}/php/extensions/debug-zts-20121212/";
modulename="psql";
sourcedir="/home/hnd/php-5.5.9/";
moduleentry="extension=${modulename}.so";
phpexe=$(which php)


if [ ! -f $phpdir/php.ini ];
then
	cp $sourcedir/php.ini-developement $phpdir/php.ini
fi


phpize;
./configure;
make all;


cp modules/${modulename}.so ${modulesdir};


if grep -n  "${moduleentry}" $phpdir/php.ini
then
	echo "Modul bereits korrekt in php.ini eingetragen."
else
	echo "$moduleentry" >> $phpdir/php.ini
fi

$phpexe ${modulename}.php
valgrind -v --show-leak-kinds=all --leak-check=full --log-file=val.log $phpexe ${modulename}_adj.php ?> exec.log
