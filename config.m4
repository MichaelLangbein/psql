dnl $Id$

dnl Fuege neue Option hinzu zu ./configure
dnl PHP_ARG_WITH ist eine Funktion, die durch ./configure aufgerufen wird. 
dnl sie bekommt als Argument den Wert mit, den der Nutzer hinter --with-psql 
dnl bzw. --with-psql-mysqllibdir schreibt.
PHP_ARG_WITH(psql, Erlaube PSQL-Modul,
[  --with-psql             Erlaube PSQL-Modul zusammen mit seinen Abhaengigkeiten.])

dnl Mit dieser  Option saehe ein config-Aufruf so aus:
dnl ./configure --with-psql 
dnl Das erste --with setzt eine lokale Variable $PHP_PSQL = 'yes'
dnl (oder, wenn ein Argument mitgegeben wurde, zum Wert dieses Argumentes) 




dnl Ok, hier der eigentliche Code
if test "$PHP_PSQL" != "no"; then

  dnl Pfad zu mysql-include
  MYSQL_INC_DIR=/usr/include/mysql
  dnl Versuche, MYSQL_LIB_DIR zu setzen
  for i in /usr /usr/lib /usr/lib/x86_64-linux-gnu /usr/lib/i386-linux-gnu /usr/local; do
      str="$i/libmysqlclient.so"
      if test -f $str; then
	MYSQL_LIB_DIR=$i
	break
      fi
  done
  if test -z "$MYSQL_LIB_DIR"; then
    AC_MSG_ERROR([Ich kann libmysqlclient.so nirgends finden, Mann.])
  else
    AC_MSG_RESULT([libmysqlclient.so gefunden in $MYSQL_LIB_DIR.])
  fi



    dnl Dies ist ein Wrapper um Autoconf's AC_CHECK_LIB()
    dnl Erzeugt ein Testprogramm, das versucht, die gewünschte Bibliothek einzubinden. 
    dnl Nimmt die Arguemte: 
    dnl zu ladende Bibliothek,
    dnl zu testende Funktion innerhalb der Bibliothek,
    dnl was tun wenn erfolgreich,
    dnl was tun wenn nicht erfolgreich,
    dnl extra cl-optionen zum einlesen der Bibliothek
    PHP_CHECK_LIBRARY(	mysqlclient, 
			mysql_close,
			[],
			[AC_MSG_ERROR([Einlesen von mysqlclient library hat nicht geklappt])],
			[-L$MYSQL_LIB_DIR]
		     )


    dnl Wenn die bibliothek erfolgreich getestet wurde, ist es zeit, sie bei php zu registrieren.
    dnl PSQL_SHARED_LIBADD ist wahrscheinlich eine vordefinierte Variable, zu der wir auch 
    dnl mehrere libs hinzufuegen koennen.
    PHP_ADD_LIBRARY_WITH_PATH(mysqlclient, $MYSQL_LIB_DIR, PSQL_SHARED_LIBADD)
    dnl Dies ist ein PHP-Wrapper um AC_SUBST. Macht aus der Shell-Veriable eine Output-Variable (diese geht dann in makefile).
    PHP_SUBST(PSQL_SHARED_LIBADD)
    dnl Und natuerlich nicht include vergessen, damit wir auch die zugehoerigen header bekommen.
    dnl Hiermit wird $MYSQL_INC_DIR zum PHP-Standard-Suchpfad hinzugefuegt. 
    PHP_ADD_INCLUDE($MYSQL_INC_DIR)


    AC_MSG_RESULT(
    [Wow, bis hierhin hat alles geklappt! MySQL wurde zu PHP hinzugefügt. 
     Nur zu deiner Information, mysql liegt auf deinem System hier: 
     MYSQL_LIB_DIR =  $MYSQL_LIB_DIR , verwendet in PHP_ADD_LIB als Pfad 
     MYSQL_INC_DIR =  $MYSQL_INC_DIR , verwendet in PHP_ADD_INCL als Pfad 
     Du machst das toll. Weiter so!])


  dnl Ok, jetzt zu pthreads 
  dnl Hier sollten wir etwas einfacher davonkommen. 
  dnl Versuche, PTHREADS_LIB_DIR zu setzen
  for i in /usr /usr/lib /usr/lib/x86_64-linux-gnu /usr/lib/i386-linux-gnu /usr/local; do
      str="$i/libpthread.so"
      if test -f $str; then
	PTHREADS_LIB_DIR=$i
	break
      fi
  done
  if test -z "$PTHREADS_LIB_DIR"; then
    AC_MSG_ERROR([Ich kann libpthread.so nirgends finden, Mann.])
  else
    AC_MSG_RESULT([libpthread.so gefunden in $PTHREADS_LIB_DIR.])
  fi
  AC_MSG_NOTICE([Nebenbei diese dubiose Variable LIBADD geht wie folgt ${PSQL_SHARED_LIBADD}])
  PHP_ADD_LIBRARY_WITH_PATH(pthread, $PTHREADS_LIB_DIR, PSQL_SHARED_LIBADD)
  PHP_SUBST(PSQL_SHARED_LIBADD)

  dnl openMP ist kein Package, sondern Teil des GCC compilers. Sollte also vorhanden sein. 
  dnl Wir brauchen also OMP nicht als library einzubinden. Aber wir müssen lib- und incl-flags mitgeben.
  dnl Dafuer gibt es sogar ein eigenes macro; Hiermit checken wir ob openMP unterstützt wird und setzen $OPENMP_CFLAGS:
  dnl AC_OPENMP
  dnl Und hiermit laden wir die entsprechenen flags (-fopenmp) in linker und compiler:
  dnl CFLAGS="$CFLAGS $OPENMP_CFLAGS"
  dnl PHP_SUBST(CFLAGS)
  dnl AC_MSG_RESULT([Haben  openmp mit den flags ${CFLAGS} eingebunden. Hoffe, das funktioniert.
  dnl 		Hiermit sieht libadd jetzt so aus: ${PSQL_SHARED_LIBADD}])

  dnl Hiermit wird die Extension endlich gebaut. 
  dnl Parameter:
  dnl "psql": Name der ext, gleich dem Namen des dirs,
  dnl Liste aller c-files der ext,ohne Komma's dazwischen
  dnl "$ext_shared", immer da, gesetzt duch ./configure beim aufruf von der Funktion PHP_ARG_WITH.
  PHP_NEW_EXTENSION(psql, psql.c meinincl.c, $ext_shared)
fi

    
