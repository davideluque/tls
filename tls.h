/*
 * 			Thread-list
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *  
 * Universidad Simón Bolívar
 * Caracas, Venezuela
*/

#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include "list.h"

/*
 * Estructura que almacena los argumentos de ejecución del programa, i.e. número
 * de hilos, directorio y nombre del archivo de salida.
 *
*/
typedef struct inargs{
	int concurrency;
	char directory[4096];
	char out[256];
} Inargs;

/*
 *
 *
 *
 *
*/
typedef struct Threadstruct{
	long id;
	int status;
	char *dir;
	List *dirlist;
	ResList *reslist;
	pthread_mutex_t dirmutex;
	pthread_mutex_t resmutex;
	pthread_mutex_t stamutex;
	struct Threadstruct *tsarray;

} threadstruct;

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
 * Inicializa la estructura que almacenará los argumentos introducidos al invocar 
 * el programa con valores por defecto.
 *
*/
void init_inputargs(Inargs *in);

/*	
 * Analiza la sintaxis de la invocación del programa mediante la línea de comandos.
 *	
*/
void parseArgs(Inargs *in, int argc, char *argv[]);


/*	
 * Función que ejecuta el hilo principal o maestro para explorar el directorio
 * pasado mediante la línea de comandos añadiendo los directorios encontrados a 
 * list.
 *	
*/
void explore(char *directory, List *list);

/*
 *
 *
 *
 *
*/
void init_threadstruct(threadstruct *ts, long id, List *dirlist, ResList *reslist, 
						pthread_mutex_t dirmutex, pthread_mutex_t resmutex,
						pthread_mutex_t stamutex, threadstruct *tsarray[]);

/*
 *
 *
 *
 *
*/
bool working(threadstruct *ts);

/*
 *
 *
 *
 *
*/
void init_reselement(ResElement *r, long id, char *path, int fcount, int bcount);

/*
 *
 *
 *
 *
*/
void threadexplore(threadstruct *ts);

/*
 *
 *
 *
 *
*/
void *threadmgmt(void *tstruct);

/*
 *
 *
 *
 *
*/
void createThreads(int numthreads, List *dirlist, 
					pthread_mutex_t dirmutex, pthread_t threads[], 
					threadstruct *tsarray[]);

/*
 *
 *
 *
 *
*/
void allocateDir(int numthreads, List *dirlist, threadstruct *tsarray[], 
					pthread_mutex_t dirmutex);

/*
 *
 *
 *
 *
*/
void explore(char *directory, List *list);

