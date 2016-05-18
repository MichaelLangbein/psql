/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_psql.h"
#include "meinincl.h"
#include <pthread.h>

/* If you declare any globals in php_psql.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(psql)
*/

/* True global resources - no need for thread safety here */
static int le_psql;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("psql.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_psql_globals, psql_globals)
    STD_PHP_INI_ENTRY("psql.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_psql_globals, psql_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_psql_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_psql_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "psql", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

/* {{{ proto array doqueries(array queries)
   Nimmt einen array von sql-queries und führt diese in parallel aus. */
PHP_FUNCTION(doqueries)
{
	int argc = ZEND_NUM_ARGS();
	zval *queries = NULL;

	if (zend_parse_parameters(argc TSRMLS_CC, "a", &queries) == FAILURE) 
		return;


        

	
	/* SCHRITT 1: PHP-Input zu c-array
	   In PHP sind alle variablen erst einmal zvals, eine union hinter der mehrere Datentypen liegen koennen.
	   In unserem Fall wissen wir, dass der hinterliegende Datentyp ein HashTable ist. 
	   Aus diesem holen wir schliesslich die strings, die in den Hash-vals liegen. 
	   Damit haben wir endlich einen blanken c-array von strings bekommen. 
	*/
	int laenge_ht = 0;
	char *queries_str[10];

	HashTable *queries_ht = Z_ARRVAL_P(queries);
	HashPosition position;
	zval **data = NULL;
	for(
	zend_hash_internal_pointer_reset_ex(queries_ht, &position);
	zend_hash_get_current_data_ex(queries_ht, (void**) &data, &position) == SUCCESS;
	zend_hash_move_forward_ex(queries_ht, &position)
	){
		char * query = Z_STRVAL_PP(data);
		queries_str[laenge_ht] = query;
		laenge_ht++;
		printf("Wir haben erfolgreich den Eingangsstring %s ausgelesen.\n", query);
	}



	/* SCHRITT 2: Verarbeiten der Daten; c-array zu c-array
	   Hier beginnt die eigentliche Arbeit. 
	*/
	if(mysql_library_init(0, NULL, NULL)){
                php_printf("Oh-oh... mysql_library_init hat nicht funktioniert.");
		RETURN_FALSE;
        }
	int num_threads = laenge_ht;
	pthread_t threads[num_threads];
        long t;
        for(t=0; t<num_threads; t++){
                if( pthread_create( &threads[t], NULL, do_query, (void*)*queries_str[t] ) == -1 ) error("Thread nicht erstellt");
        }

        void* result;

        for(t=0; t<num_threads; t++){
                if( pthread_join( threads[t], &result ) == -1 ) error("Thread nicht zusammengefügt");
                long int nr = (long int)result;
                printf("Query %ld hat %ld Reihen ausgelesen\n", t, nr);
        }

        mysql_library_end();



        /*SCHRITT 3: c-array zu PHP-Output
 	  Wir bereiten schon mal den return value vor 
	  Was hier passiert ist folgendes: 
	  Der zval "return_value" bekommt den zu grunde liegenden Datentyp "HashTable".
	  Und dieser wiederum wird befuellt mit den Ergebnissen, die wir aus unserem Modul geholt haben. 
	*/
        array_init(return_value);
        add_assoc_long(return_value, "Hat geklappt", 1);

	RETURN_TRUE;
}
/* }}} */


/* {{{ php_psql_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_psql_init_globals(zend_psql_globals *psql_globals)
{
	psql_globals->global_value = 0;
	psql_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(psql)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(psql)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(psql)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(psql)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(psql)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "psql support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ psql_functions[]
 *
 * Every user visible function must have an entry in psql_functions[].
 */
const zend_function_entry psql_functions[] = {
	PHP_FE(confirm_psql_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(doqueries,	NULL)
	PHP_FE_END	/* Must be the last line in psql_functions[] */
};
/* }}} */

/* {{{ psql_module_entry
 */
zend_module_entry psql_module_entry = {
	STANDARD_MODULE_HEADER,
	"psql",
	psql_functions,
	PHP_MINIT(psql),
	PHP_MSHUTDOWN(psql),
	PHP_RINIT(psql),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(psql),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(psql),
	PHP_PSQL_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PSQL
ZEND_GET_MODULE(psql)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
