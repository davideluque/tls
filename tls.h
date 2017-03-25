/*
 * 			Thread-project
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *
 * Descripción: Cabecera del archivo tls.c que contiene las librerías
 * necesarias, variables globales, prototipos de estructuras y de
 * procedimientos y métodos.
 *
 * Universidad Simón Bolívar
 * Caracas, Venezuela
 * Marzo, 2017.
 *
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <ctype.h>
#include "list.h"

pthread_mutex_t idlemutex, dirmutex, infomutex;

/*
 * Estructura que almacena los argumentos de ejecución del programa,
 * i.e. número de hilos, directorio y nombre del archivo de salida.
*/
typedef struct inargs{
	int concurrency;
	char directory[PATH_MAX];
	char out[256];
} Inargs;

/*
 * Estructura de hilos que almacena información necesaria durante su ejecución,
 * i.e. id de hilo, directorio asignado a explorar, número de hilos en el
 * programa, indicador de asignación o no de directorio y apuntadores a: lista
 * de directorios, de información encontrada, de hilos sin directorio asignado.
*/
typedef struct threadstruct{
  long id;
  char directory[PATH_MAX];
	int thnum;
	int has_dirallocated;
  List *dirlist;
  List *infolist;
	List *idlelist;
} Threadstruct;

/* 	help: procedimiento que se encarga de mostrar en pantalla un mensaje de ayu-
* da sobre sintaxis, parametros de entrada y salida.
*/
void help(void);

/*	usage: se ejecuta cuando se invoca el programa con un comando inválido.
*/
void usage(void);

/* 	init_inputarg: procedimiento que se encarga de inicializar los valores intro-
* 	ducidos por el usuario.
*/
void init_inputargs(Inargs *in);

/*
/* 	parseArgs: procedimiento que se encarga de verificar la instruccion que desea
* 	ejecutar el usuario.
*/
void parseArgs(Inargs *in, int argc, char *argv[]);

/* 	init_threastruct: procedimiento que se encarga de inicializar los valores co-
* rrespondientes a los los atributos de un hilo.
* 	Parametros de entrada:
* 	- t: apuntador a estructura threadstruct.
*	- thnum: numeros de hilos creados.
* 	- dirlist: apuntador a la lista de directorios pendientes.
*	- infolis: apuntador a la lista que almacena la informacion obtenida por los
* 	hilos.
* 	- idlelist: apuntador a la lista de hilos que no tienen trabajo asignado.
*/
void init_threadstruct(Threadstruct *t, int thnum, List *dirlist,
												List* infolist, List *idlelist);

/*	init_information: procedimiento que se encarga de inicializar los valores en
* 	en los que se almacena la informacion obtenida por el hilo.
*	Parametros de entrada:
*	- i: apuntador a la estructura information que contendra la informacion ob-
* 	tenida por el nodo.
*	- id: identificador del hilo.
* 	- dir: directorio explorado por el hilo.
*	- fcount: cantidad de archivos encontrados por el hilos.
*	- bcount: cantidad de bytes.
*/
void init_information(Information *i, long id, char *dir, int fcount, int bcount);

/* 	onwork: procedimiento que verifica si hay hilos trabajando.
* 	Parametros de entrada:
*	- thnum: numeros de hilos creados.
* 	- idlelist: apuntador a la lista de hilos que no tienen trabajo asignado.
*/
int onwork(int thnum, List* idlelist);

/*	threadmgmt: funcion que sera ejecutada por los hijos, verifica si un hilo
* 	tiene un directorio asignado para que este inicie a explorar el directorio
*
*/
void *threadmgmt(void *structure);

/*	explore: procedimiento que se encarga de explorar un directorio.
* 	Parametros de entrada:
* 	- t: apuntador a la estructura del hilo.
*/
void explore(Threadstruct *t);

/* 	allocateDir: procedimiento que se encarga de asignarle un directorio a un hi-
* 	lo ociosio, sin trabajo.
*	Parametros de entrada:
* 	- t: apuntador a la estructura del hilo.
*/
void allocateDir(Threadstruct *master);

/* 	writeInformation: procedimiento que se encarga mostrar la informacion reci-
* 	bida del hilo padre por sus hijos y mostrarla por defecto en la linea de co-
*	mando o si el usuario lo desea en un archivo de salida.
* 	Parametros de entrada:
* 	- out: nombre del archivo de salida.
*/
void writeInformation(Threadstruct *master, char *out);
