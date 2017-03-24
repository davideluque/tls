/*
 * 			Thread-project
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *
 * Descripción:
 *
 * Universidad Simón Bolívar
 * Caracas, Venezuela
*/
#include "tls.h"

int main(int argc, char *argv[]) {
  /*
   * Por defecto los valores de los argumentos son inicializados mediante la
   * función init_inputargs con 1, directorio de trabajo actual y "salida"
   * respectivamente.
  */
  Inargs *in = (Inargs *) malloc(sizeof(Inargs));
  // Lista que contendra los directorios pendientes a explorar.
  List *dirlist = (List *) malloc(sizeof(List));
  // Lista que contiene la informacion obtenida por los hilos.
  List *infolist = (List *) malloc(sizeof(List));
  // Lista que contiene los hilos que no tienen directorios asignados.
  List *idlelist = (List *) malloc(sizeof(List));
  // Apuntador a los hilos.
  pthread_t *threads;
  //
  Threadstruct *threads_structs;
  //
  Threadstruct *masterthread;
  // Contador de iteracion.
  int i;

  pthread_mutex_init(&idlemutex, NULL);
  pthread_mutex_init(&dirmutex, NULL);
  pthread_mutex_init(&infomutex, NULL);

  // Inicializamos los valores introducidos por el usuario.
  init_inputargs(in);

  // Ejecutamos la funcion que se encagara de procesar los valores dados por el
  // usuario.
  parseArgs(in, argc, argv);

  //chdir(in->directory);
	getcwd(in->directory, 4096);

  /*
   * Dos estructuras globales threads y threads_structs. La primera almacena los
   * hilos, la segunda almacenara las estructuras de los hilos.
  */
  threads = (pthread_t *) malloc(sizeof(in->concurrency * sizeof(pthread_t)));
  threads_structs = (Threadstruct *) malloc(sizeof(in->concurrency * sizeof(Threadstruct)));

  // Inicializamos la lista donde se almacenaran los directorios a explorar.
  init_list(dirlist);

  // Inicializamos la lista donde se almacenara a inofrmacion obtenida por los
  // hilos.
  init_list(infolist);

  // Inicializamos la lista donde se almacenaran los hilos que no tienen direc-
  // torios asignados.
  init_list(idlelist);

  masterthread = (Threadstruct *) malloc(sizeof(Threadstruct));
  init_threadstruct(masterthread, in->concurrency, dirlist, infolist, idlelist);
  strcpy(masterthread->directory, in->directory);

  explore(masterthread);

  /*
	 * Se crearan el numero de hilos indicado en el valor de concurrency.
   *
	 * Los hilos quedan esperando que el hilo maestro explore el directorio
	 * principal.
	*/
  //printf("Inicio de la creacion\n");
  createThreads(in->concurrency, threads, dirlist, infolist, idlelist);
  //printf("Fin de la creacion\n");

  //printf("Comienzo de la asignacion\n");
  allocateDir(masterthread);

  for(i = 0; i < in->concurrency; i++) pthread_join(threads[i], NULL);

  printf("salida salida %")
  writeInformation(masterthread, in->out);

  //printf("Hola me voy soy el papa\n");
  return 0;
}
