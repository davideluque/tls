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
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
<<<<<<< Updated upstream
=======
#include <ctype.h>
>>>>>>> Stashed changes
#include "list.h"

pthread_mutex_t idlemutex, dirmutex, infomutex;

/*
 * Estructura que almacena los argumentos de ejecución del programa,
 * i.e. número de hilos, directorio y nombre del archivo de salida.
*/
typedef struct inargs{
	int concurrency;
	char directory[4096];
	char out[256];
} Inargs;

<<<<<<< Updated upstream
=======
/* 
*/
>>>>>>> Stashed changes
typedef struct threadstruct{
  long id;
  char directory[4096];
	int thnum;
	int has_dirassigned;
  List *dirlist;
  List *infolist;
	List *idlelist;
} Threadstruct;

<<<<<<< Updated upstream
/*
 * Ejecuta pager localizado en /usr/bin/ para visualizar el manual del
 * programa que contiene información acerca de la sintaxis, descripción
 * de párametros, etc.
*/
void help(void);

/*
 * Se ejecuta cuando se invoca el programa con un comando inválido.
*/
void usage(void);

/*
 * Inicializa la estructura que almacenará los argumentos introducidos
 * al invocar el programa con valores por defecto.
*/
void init_inputargs(Inargs *in);

/*
 * Analiza la sintaxis de la invocación del programa mediante la línea
 * de comandos.
*/
void parseArgs(Inargs *in, int argc, char *argv[]);

void init_threadstruct(Threadstruct *t, int thnum, List *dirlist,
												List* infolist, List *idlelist);

int onwork(int thnum, List* idlelist);

void *threadmgmt(void *structure);

void createThreads(int numthreads, pthread_t *threads, List *dirlist,
										List *infolist, List* idlelist);

void explore(Threadstruct *t);

void allocateDir(Threadstruct *master);
=======
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
*	que se le fue asignado.
* 	Prametros de entrada:
*	- structure:
*/
void *threadmgmt(void *structure);

/* 	createThreads: procedimiento que se encarga de crear los hilos trabajadores.
* 	Parametros de entrada:
*	- thnum: numeros de hilos creados. 
* 	- dirlist: apuntador a la lista de directorios pendientes.
*	- infolis: apuntador a la lista que almacena la informacion obtenida por los
* 	hilos.
* 	- idlelist: apuntador a la lista de hilos que no tienen trabajo asignado.
*/
void createThreads(int numthreads, pthread_t *threads, List *dirlist,
										List *infolist, List* idlelist);
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
>>>>>>> Stashed changes
