/*
 * 			Thread-project
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *
 * Descripción: Este archivo contiene varias definiciones de las funciones
 * utilizadas a lo largo del thread-list: análisis de argumentos, inicialización
 * de estructuras, manejo de hilos, exploración de directorios, asignación y
 * escritura de salida
 *
 * Universidad Simón Bolívar
 * Caracas, Venezuela
 * Marzo, 2017
 *
*/

#include "tls.h"

/* 	help: procedimiento que se encarga de mostrar en pantalla un mensaje de ayu-
* da sobre sintaxis, parametros de entrada y salida.
*/
void help(void){
	execlp("pager", "pager", "help");
}

/*	usage: se ejecuta cuando se invoca el programa con un comando inválido.
*/
void usage(void){
	printf("uso: tls [-h] | [-n i] [-d directorio] [salida]\n");
	printf("ejecute tls -h para mayor información\n");
	exit(0);
}

/* 	init_inputarg: procedimiento que se encarga de inicializar los valores intro-
* 	ducidos por el usuario.
*/
void init_inputargs(Inargs *in){
	in->concurrency = 1;
	getcwd(in->directory, PATH_MAX);
	strcpy(in->out, "stdout");
}

/* 	parseArgs: procedimiento que se encarga de verificar la instruccion que desea
* 	ejecutar el usuario.
*/
void parseArgs(Inargs *in, int argc, char *argv[]){
	int ch;
	bool h, nd;

	h = false;
	nd = false;

	while( (ch = getopt(argc, argv, "hn:d:")) != -1)
		switch(ch){
			case 'h':
				h = true;
				break;
			case 'n':
				if (!isdigit(optarg[0])) usage();
				in->concurrency = atoi(optarg);
				nd = true;
				break;
			case 'd':
				strcpy(in->directory, optarg);
				nd = true;
				break;
			case '?':
				usage();
			default:
				usage();
		}

	if(h && nd) usage();
	else if (h) help();

	if(optind < argc) strcpy(in->out, argv[optind]);
}

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
												List* infolist, List *idlelist){
  t->id = 0;
	t->thnum = thnum;
	t->has_dirallocated = 0;
  t->dirlist = dirlist;
  t->infolist = infolist;
	t->idlelist = idlelist;
}

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
void init_information(Information *i, long id, char *dir, int fcount,
												int bcount){
	i->id = id;
	strcpy(i->path, dir);
	i->fcount = fcount;
	i->bcount = bcount;
}

/* 	onwork: procedimiento que verifica si hay hilos trabajando.
* 	Parametros de entrada:
*	- thnum: numeros de hilos creados.
* 	- idlelist: apuntador a la lista de hilos que no tienen trabajo asignado.
*/
int onwork(int thnum, List* idlelist){
	return (thnum != idlelist->size);
}

/*	threadmgmt: procedimiento que sera ejecutada por los hijos, verifica si
*   un hilo tiene un directorio asignado para que este inicie a explorar el
*   directorio que se le fue asignado.
* 	Parametros de entrada:
*	- structure:
*/
void *threadmgmt(void *structure){
  Threadstruct *t;
	Node *n;
	int i;

	t = (Threadstruct *) structure;
	t->id = pthread_self();

	// Se verifica si hay directorios pendientes por asignar o hay hilos ociosos.
	while(!empty(t->dirlist) || onwork(t->thnum, t->idlelist)){

		// Se verifica si el hilo tiene directorio asignado
		if(t->has_dirallocated){

			// Hilo hijo explora directorio
      explore(t);
			// Hilo termino de explorar, se ajustan sus valores como el de un
	    // hilo ocioso.
			t->has_dirallocated = 0;

			n = (Node *) malloc(sizeof(Node));
			init_node(n, t);

			pthread_mutex_lock(&idlemutex);
			add(t->idlelist, n);
			pthread_mutex_unlock(&idlemutex);
    }
  }
	pthread_exit(NULL);
}

/* 	createThreads: procedimiento que se encarga de crear los hilos trabajadores.
* 	Parametros de entrada:
*	- thnum: numeros de hilos creados.
* 	- dirlist: apuntador a la lista de directorios pendientes.
*	- infolis: apuntador a la lista que almacena la informacion obtenida por los
* 	hilos.
* 	- idlelist: apuntador a la lista de hilos que no tienen trabajo asignado.
*/
void createThreads(Threadstruct *master, pthread_t *threads){
  int rc, i;
	Threadstruct *t;
	Node *n;

  for (i = 0; i < master->thnum; i++) {

		// Se crean hilos hijos.
    t = (Threadstruct *) malloc(sizeof(Threadstruct));

		// Se inicializan sus valores.
    init_threadstruct(t, master->thnum, master->dirlist, master->infolist,
											master->idlelist);

		n = (Node *) (malloc(sizeof(Node)));
		init_node(n, t);

		// Se anaden hilos ociosos a la idlelist
		add(master->idlelist, n);

    rc = pthread_create(&threads[i], NULL, threadmgmt, t);

		if (rc){
      printf("Error al crear hilo. Código de error:%d\n", rc);
      exit(-1);
    }
  }
}

/*	explore: procedimiento que se encarga de explorar un directorio.
* 	Parametros de entrada:
* 	- t: apuntador a la estructura del hilo.
*/
void explore(Threadstruct *t){
	DIR *dp;
	struct dirent *ep;
	struct stat statbuffer;
	char subdir[PATH_MAX];
	Node *n;
	Directory *d;
	Information *i;
	int filescount, bytescount;

	filescount = 0;
	bytescount = 0;

	dp = opendir(t->directory);

	if(!dp){
		perror("No fue posible abrir el directorio\n");
		return;
	}

	while((ep = readdir(dp)) != NULL){
		// Ignore hiden directories
		if(ep->d_name[0] != '.'){

			strcpy(subdir, t->directory);
			strcat(subdir, "/");
			strcat(subdir, ep->d_name);

			if(lstat(subdir, &statbuffer) == -1){
				perror("No se pudo obtener la información del archivo\n");
				return;
			}

			if(S_ISDIR(statbuffer.st_mode)){
				n = (Node *) malloc(sizeof(Node));
				d = (Directory *) malloc(sizeof(Directory));

				strcpy(d->dir, subdir);
				init_node(n, d);

				pthread_mutex_unlock(&dirmutex);
				add(t->dirlist, n);
				pthread_mutex_unlock(&dirmutex);
				continue;
			}

			else{
				filescount++;
				bytescount += statbuffer.st_size;
				continue;
			}
		}
	}
	closedir(dp);

	n = (Node *) malloc(sizeof(Node));
	i = (Information *) malloc(sizeof(Information));
	init_information(i, t->id, t->directory, filescount, bytescount);
	init_node(n, i);

	pthread_mutex_lock(&infomutex);
	add(t->infolist, n);
	pthread_mutex_unlock(&infomutex);
}

/* 	allocateDir: procedimiento que se encarga de asignarle un directorio a un hi-
* 	lo ociosio, sin trabajo.
*	Parametros de entrada:
* 	- t: apuntador a la estructura del hilo.
*/
void allocateDir(Threadstruct *master){
	Node *c, *d;
	Threadstruct *t;

	// Se verifica si hay directorios pendientes por asignar o hay hilos ociosos.
	while(!empty(master->dirlist) || onwork(master->thnum, master->idlelist)){
		if(master->idlelist->size != 0 && master->dirlist->size != 0){

			// Región crítica: Obtenemos hilo ociosio, sin trabajo.
			pthread_mutex_lock(&idlemutex);
			c = get(master->idlelist);
			t = (Threadstruct *) c->content;
			pthread_mutex_unlock(&idlemutex);

			// Región crítica: Obtenemos directorio pendiente por explorar.
			pthread_mutex_lock(&dirmutex);
			d = get(master->dirlist);

			// Asignamos directorio al hilo ocioso
			t->has_dirallocated = 1;
			strcpy(t->directory, d->content);
			pthread_mutex_unlock(&dirmutex);

		}
	}
}

/* 	writeInformation: procedimiento que se encarga mostrar la informacion reci-
* 	bida del hilo padre por sus hijos y mostrarla por defecto en la linea de co-
*	mando o si el usuario lo desea en un archivo de salida.
* 	Parametros de entrada:
* 	- out: nombre del archivo de salida.
*/
void writeInformation(Threadstruct *master, char *out){
	FILE *f;
	Node *first, *next;
	Information *info;
	bool file_specified;

	file_specified = false;

	if(strcmp(out,"stdout")){
		f = fopen(out, "w");
		file_specified = true;
	}

	if(master->infolist->size){
		first = master->infolist->first;
		info = (Information *) first->content;

		if (file_specified) fprintf(f, "%ld %s %d %lu\n", info->id, info->path,
																	info->fcount, info->bcount);

		else fprintf(stdout, "%ld %s %d %lu\n", info->id, info->path,
									info->fcount, info->bcount);

		next = first->next;

		while(next){
			info = (Information *) next->content;

			if (file_specified) fprintf(f, "%ld %s %d %d\n", info->id, info->path,
																		info->fcount, info->bcount);

			else fprintf(stdout, "%ld %s %d %d\n", info->id, info->path,
										info->fcount, info->bcount);

			next = next->next;
		}
	}
}
