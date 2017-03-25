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

/*
 * Ejecuta pager para visualizar el manual del programa que contiene
 * información acerca de la sintaxis, descripción de párametros, etc.
*/
void help(void);

/*
 * Se ejecuta cuando el programa es invocado mediante un comando inválido.
*/
void usage(void);

/*
 * Inicializa con valores por defecto la estructura que almacenará los
 * argumentos introducidos al invocar el programa.
*/
void init_inputargs(Inargs *in);

/*
 * Analiza la sintaxis de la invocación del programa mediante la línea
 * de comandos.
*/
void parseArgs(Inargs *in, int argc, char *argv[]);

/*
 * Inicializa la estructura de los hilos con parámetros y valores por defecto.
*/
void init_threadstruct(Threadstruct *t, int thnum, List *dirlist,
												List* infolist, List *idlelist);

/*
 * Verifica dado el número de hilos y la lista de hilos inactivos, si hay
 * hilos trabajando
*/
int onwork(int thnum, List* idlelist);

/*
 * Procedimiento ejecutado por los hilos al momento de su creación. Mantiene a
 * los hilos en un ciclo esperando por asignacion de directorios. Al tener uno,
 * son enviados a explorarlo.
*/
void *threadmgmt(void *structure);

/*
 * Procedimiento ejecutado por el hilo maetsro que se encarga de crear los
 * hilos indicados en los argumentos de invocación del programa.
*/
void createThreads(Threadstruct *master, pthread_t *threads);

void explore(Threadstruct *t);

void allocateDir(Threadstruct *master);

void writeInformation(Threadstruct *master, char *out);
