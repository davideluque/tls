/*
 * 			Thread-project
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *
 * Descripción: Programa principal del thread-list. Esta función siempre es
 * ejecutada por el hilo maestro. El thread-list explora un directorio con un
 * número de hilos.
 *
 * Universidad Simón Bolívar
 * Caracas, Venezuela
 * Marzo, 2017.
 *
*/
#include "tls.h"

int main(int argc, char *argv[]) {
  /*
   * Por defecto los valores de los argumentos son inicializados mediante la
   * función init_inputargs con 1, directorio de trabajo actual y salida
   * estándar respectivamente.
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
  // Estructura del hilo maestro
  Threadstruct *masterthread;
  // Contador de iteracion.
  int i;

  /*
   * Inicialización de exclusiones mutuas para regiones críticas del programa.
   * Las regiones críticas son:
   * - Añadir hilos a la lista de hilos inactivos (sin directorio asginado)
   * - Añadir directorios a la lista de directorios a explorar
   * - Añadir información encontrada a la lista de información pos-exploración
   * Evitan las condiciones de carrera.
  */

  pthread_mutex_init(&idlemutex, NULL);
  pthread_mutex_init(&dirmutex, NULL);
  pthread_mutex_init(&infomutex, NULL);

  // Inicializamos los valores introducidos por el usuario.
  init_inputargs(in);

  // Ejecutamos la funcion que se encagara de procesar los valores dados por el
  // usuario.
  parseArgs(in, argc, argv);

  chdir(in->directory);
	getcwd(in->directory, PATH_MAX);

  threads = (pthread_t *) malloc(sizeof(in->concurrency * sizeof(pthread_t)));

  // Inicializamos la lista donde se almacenaran los directorios a explorar.
  init_list(dirlist);

  // Inicializamos la lista donde se almacenara a inofrmacion obtenida por los
  // hilos.
  init_list(infolist);

  // Inicializamos la lista donde se almacenaran los hilos que no tienen direc-
  // torios asignados.
  init_list(idlelist);

  // Se crea el hilo padre
  masterthread = (Threadstruct *) malloc(sizeof(Threadstruct));
  init_threadstruct(masterthread, in->concurrency, dirlist, infolist, idlelist);
  strcpy(masterthread->directory, in->directory);

  /*
   * Hilo padre explora directorio y agrega los directorios encontrados a la
   * lista de directorios pendientes
  */
  explore(masterthread);

  /*
	 * El hilo maestro tiene en su estructura la información necesaria para la
   * creación de hilos, i.e. concurrencia (número de hilos a crear) y los
   * apuntadores a la lista de directorios, de información encontrada y de
   * hilos inactivos.
   *
	 * Los hilos creados se mantienen esperando que el hilo maestro les asigne
   * un directorio para explorar.
	*/
  createThreads(masterthread, threads);

  // EL hilo padre asigna directorios a sus hilos hijos.
  allocateDir(masterthread);

  // Hilo padre espera por la finalización de hijos.
  for(i = 0; i < in->concurrency; i++) pthread_join(threads[i], NULL);

  // Escribe la información obtenida
  writeInformation(masterthread, in->out);

  return 0;
}
