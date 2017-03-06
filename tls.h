/*
 * 			Thread-list
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *  
 * Universidad Simón Bolívar
 * Caracas, Venezuela
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*	
 * Estructura que almacena las opciones introducidas por el usuario al invocar
 * el programa en la línea de comandos.
 *
*/
typedef struct Options{
	int concurrency;
	char directory[4096];
	char out[256];
} Options;

/*	
 * Ejecuta pager localizado en /usr/bin/ para visualizar el manual del programa
 *
*/
void help(void);

/*	
 * Inicializa la estructura que almacena las opciones introducidas al invocar
 * el programa.
 *
*/
void init_options(Options *opt);

/*	
 * Muestra en pantalla un mensaje de ayuda acerca de la sintaxis, descripción de
 * párametros, etc. y termina.
 *
*/
void usage(void);

/*	
 * Analiza la sintaxis de la invocación.
 *	
*/
void parseArgs(int argc, char *argv[]);
