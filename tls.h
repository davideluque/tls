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
 * Estructura que almacena las opciones de ejecución del programa, i.e. número
 * de hilos, directorio y nombre del archivo de salida.
 *
*/
typedef struct Options{
	int concurrency;
	char directory[4096];
	char out[256];
} Options;

/*	
 * Ejecuta pager localizado en /usr/bin/ para visualizar el manual del programa
 * que contiene información acerca de la sintaxis, descripción de párametros, etc.
 *
*/
void help(void);

/*	
 * Se ejecuta cuando se invoca el programa con un comando inválido.
 *
*/
void usage(void);

/*	
 * Inicializa la estructura que almacena las opciones introducidas al invocar
 * el programa.
 *
*/
void init_options(Options *opt);

/*	
 * Analiza la sintaxis de la invocación del programa mediante la línea de comandos.
 *	
*/
void parseArgs(Options *options, int argc, char *argv[]);
