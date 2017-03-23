/*
 * 			Thread-project
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *  
 * Universidad Simón Bolívar
 * Caracas, Venezuela
*/


#include "tls.h"

int main(int argc, char *argv[])
{

	pthread_mutex_t dirmutex;
	pthread_mutex_init(&dirmutex, NULL);
	
	/*
	 * Por defecto los valores de los argumentos son inicializados mediante la 
	 * función init_inputargs con 1, directorio de trabajo actual y "salida" 
	 * respectivamente.
	 *
	*/
	Inargs *in = (Inargs *) malloc(sizeof(Inargs));
	init_inputargs(in);

	parseArgs(in, argc, argv);

	/*
	 * Se crearan el numero de hilos indicado en el valor de concurrency. Cada
	 * hilo tendra asignada una estructura threadstruct que contendra su id, su
	 * estado, la lista de directorios (para añadir los directorios que encuentre),
	 * la lista de resultados (para completar al finalizar la exploración del dir),
	 * el mutex de directorios (evitar problemas al añadir, quitar directorios),
	 * el mutex de resultados (mismo caso de directorios), el mutex de estado
	 *
	 * Las estructuras de todos los hilos se almacenan en el arreglo ts.
	 *
	 * Los hilos quedan esperando que el hilo maestro explore el directorio prin-
	 * cipal. 
	 *
	*/	
	List *dirlist = (List *) malloc(sizeof(List));
	init_list(dirlist);

	pthread_t threads[in->concurrency];
	threadstruct *tsarray[in->concurrency];
	createThreads(in->concurrency, dirlist, dirmutex, threads, tsarray);

	/*
	 * El hilo maestro inicializa la lista de directorios y comienza a explorar
	 * el directorio pasado por consola (o el directorio por defecto) agregando
	 * los subdirectorios encontrados a la lista dirlist
	 *
	*/
	explore(in->directory, dirlist);

	/*
	 *
	 *
	 *
	*/
	printf("Comienzo a asignar directorios a la gente\n");
	allocateDir(in->concurrency, dirlist, tsarray, dirmutex);

	int i;
	for(i=0; i< in->concurrency; i++) pthread_join(threads[i], NULL);	

	return 0;
}