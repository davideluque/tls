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


/* 	help: procedimiento que se encarga de mostrar en pantalla un mensaje de ayu-
* da sobre sintaxis, parametros de entrada y salida.
*/
void help(void){
	// Try to execute pager? If not, print?
	execlp("/usr/bin/pager", "/usr/bin/pager", "help");
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
	getcwd(in->directory, 4096);
	strcpy(in->out, "stdout");
}

/* 	parseArgs: procedimiento que se encarga de verificar la instruccion que desea
* 	ejecutar el usuario.
*/
void parseArgs(Inargs *in, int argc, char *argv[]){
	int ch, index;
	bool h, nd;

<<<<<<< Updated upstream
	h, nd = false;
=======
	h = false;
	nd = false;
>>>>>>> Stashed changes

	while( (ch = getopt(argc, argv, "hn:d:")) != -1)
		switch(ch){
			case 'h':
				h = true;
				if(nd) usage();
				help();
			case 'n':
				if(h) usage();
				if (!isdigit(optarg[0])) usage();
				in->concurrency = atoi(optarg);
				nd = true;
				break;
			case 'd':
				if(h) usage();
				strcpy(in->directory, optarg);
				nd = true;
				break;
			case '?':
				usage();
			default:
				usage();
		}

	if(optind < argc) strcpy(in->out, argv[optind]);
}

<<<<<<< Updated upstream
=======
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
>>>>>>> Stashed changes
void init_threadstruct(Threadstruct *t, int thnum, List *dirlist, List* infolist,
												List *idlelist){
  t->id = 0;
	t->thnum = thnum;
	t->has_dirassigned = 0;
  t->dirlist = dirlist;
  t->infolist = infolist;
	t->idlelist = idlelist;
}

<<<<<<< Updated upstream
=======
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
>>>>>>> Stashed changes
void init_information(Information *i, long id, char *dir, int fcount, int bcount){
	i->id = id;
	strcpy(i->path, dir);
	i->fcount = fcount;
	i->bcount = bcount;
}

<<<<<<< Updated upstream
=======
/* 	onwork: procedimiento que verifica si hay hilos trabajando.
* 	Parametros de entrada:
*	- thnum: numeros de hilos creados. 
* 	- idlelist: apuntador a la lista de hilos que no tienen trabajo asignado.
*/
>>>>>>> Stashed changes
int onwork(int thnum, List* idlelist){
	return (thnum != idlelist->size);
}

<<<<<<< Updated upstream
=======
/*	threadmgmt: funcion que sera ejecutada por los hijos, verifica si un hilo 
* 	tiene un directorio asignado para que este inicie a explorar el directorio
*	que se le fue asignado.
* 	Prametros de entrada:
*	- structure:
*/
>>>>>>> Stashed changes
void *threadmgmt(void *structure){
  Threadstruct *t;
	Node *n;
	int i;
<<<<<<< Updated upstream

	t = (Threadstruct *) structure;
	t->id = pthread_self();

	//printf("Hola soy hilo %ld\n", t->id);

	while(!empty(t->dirlist) || onwork(t->thnum, t->idlelist)){
		if(t->has_dirassigned){
			//printf("Me asignaron un directorio %d\n", t->has_dirassigned);
			//printf("El directorio asignado es %s\n", t->directory);
      // Thread has an allocated dir
      explore(t);
			t->has_dirassigned = 0;

			n = (Node *) malloc(sizeof(Node));
			init_node(n, t);

			pthread_mutex_lock(&idlemutex);
			add(t->idlelist, n);
			pthread_mutex_unlock(&idlemutex);
    }
  }
  //printf("Hola me voy\n");
	pthread_exit(NULL);

}

void createThreads(int numthreads, pthread_t *threads, List *dirlist,
										List *infolist, List* idlelist){
  int rc, i;
	Threadstruct *t;
	Node *n;

  for (i = 0; i < numthreads; i++) {
		//printf("Iteracion %d de la creacion\n", i);
    t = (Threadstruct *) malloc(sizeof(Threadstruct));
    init_threadstruct(t, numthreads, dirlist, infolist, idlelist);

		n = (Node *) (malloc(sizeof(Node)));
		init_node(n, t);

		//pthread_mutex_lock(&idlemutex);
		add(idlelist, n);
		//pthread_mutex_unlock(&idlemutex);

    rc = pthread_create(&threads[i], NULL, threadmgmt, t);
		//pthread_join(threads[i], NULL);

		if (rc){
=======

	t = (Threadstruct *) structure;
	t->id = pthread_self();


	// Se verifica si hay directorios pendientes por asignar o hay hilos ociosos.
	while(!empty(t->dirlist) || onwork(t->thnum, t->idlelist)){

		// Se verifica si el hilo tiene directorio asignado
		if(t->has_dirassigned){

			// Hilo hijo explora directorio
      		explore(t);

      		// Hilo termino de explorar, se ajustan sus valores como el de un
			// hilo ocioso.
			t->has_dirassigned = 0;

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
void createThreads(int numthreads, pthread_t *threads, List *dirlist,
										List *infolist, List* idlelist){
  int rc, i;
	Threadstruct *t;
	Node *n;


  for (i = 0; i < numthreads; i++) {
    
    // Se crean hilos hijos.
    t = (Threadstruct *) malloc(sizeof(Threadstruct));
    // Se inicializan sus valores.
    init_threadstruct(t, numthreads, dirlist, infolist, idlelist);

	n = (Node *) (malloc(sizeof(Node)));
	init_node(n, t);

	// Se anaden hilos ociosos a la idlelist
	//pthread_mutex_lock(&idlemutex);
	add(idlelist, n);
	//pthread_mutex_unlock(&idlemutex);

    rc = pthread_create(&threads[i], NULL, threadmgmt, t);
	//pthread_join(threads[i], NULL);

	if (rc){
>>>>>>> Stashed changes
      printf("Error al crear hilo. Código de error:%d\n", rc);
      exit(-1);
    }
  }
}

<<<<<<< Updated upstream
void explore(Threadstruct *t){
	DIR *dp;
	struct dirent *ep;
	struct stat statbuffer;
=======
/*	explore: procedimiento que se encarga de explorar un directorio.
* 	Parametros de entrada:
* 	- t: apuntador a la estructura del hilo.
*/
void explore(Threadstruct *t){
	DIR *dp;
	struct dirent *ep;
	structs stat statbuffer;
>>>>>>> Stashed changes
	char subdir[4096];
	char file[4096];
	Node *n;
	Directory *d;
	Information *i;
	int filescount, bytescount;

	filescount = 0;
	bytescount = 0;

	//printf("Explorará el hilo %ld el directorio %s\n", t->id, t->directory);

	dp = opendir(t->directory);

	if(!dp){
		perror("No fue posible abrir el directorio\n");
	}

	while((ep = readdir(dp)) != NULL){
		// Ignore hiden directories
		if(ep->d_name[0] != '.'){

			strcpy(file, t->directory);
			strcat(file, "/");
			strcat(file, ep->d_name);

			if(lstat(file, &statbuffer) == -1){
				perror("No se pudo obtener la información del directorio\n");
			}

			if(S_ISDIR(statbuffer.st_mode)){
				n = (Node *) malloc(sizeof(Node));
				d = (Directory *) malloc(sizeof(Directory));

				strcpy(subdir, t->directory);
				strcat(subdir, "/");
				strcat(subdir, ep->d_name);

				//printf("subdirectorio %s\n", subdir);
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
<<<<<<< Updated upstream
	//printf("Termino de explorar el hilo\n");
=======


>>>>>>> Stashed changes
	n = (Node *) malloc(sizeof(Node));
	i = (Information *) malloc(sizeof(Information));
	init_information(i, t->id, t->directory, filescount, bytescount);
	init_node(n, i);
	pthread_mutex_lock(&infomutex);
	add(t->infolist, n);
	pthread_mutex_unlock(&infomutex);
<<<<<<< Updated upstream
}

void allocateDir(Threadstruct *master){
	Node *c, *d;
	Threadstruct *t;
	char dir[4096];

	// Master thread will allocate directories while the list isnt empty and there
	// are threads working
	while(!empty(master->dirlist) || onwork(master->thnum, master->idlelist)){
		//printf("Iteracion de la asignacion\n");
		if(master->idlelist->size != 0 && master->dirlist->size != 0){

			pthread_mutex_lock(&idlemutex);
			c = get(master->idlelist);
			t = (Threadstruct *) c->content;
			pthread_mutex_unlock(&idlemutex);

			//printf("Asignaremos a id:%ld\n", t->id);

			pthread_mutex_lock(&dirmutex);
			d = get(master->dirlist);
			t->has_dirassigned = 1;
			strcpy(t->directory, d->content);
			pthread_mutex_unlock(&dirmutex);

		}
	}
}

void writeInformation(Threadstruct *master, char *out){
	FILE *f;
	Node *first, *next;
	Information *contenido;
	bool archivo;
	char *std;
	strcpy(std, "stdout");

	archivo = false;

		if(strcmp(out,std)){
		f = fopen(out, "w");
		archivo = true;
	}

	if(master->infolist->size){
		first = master->infolist->first;
		contenido = (Information *) first->content;

		if (archivo){
			fprintf(f, "%ld %s %d %d\n", contenido->id, contenido->path,
							contenido->fcount, contenido->bcount);
		}
		else{
			fprintf(stdout, "%ld %s %d %d\n", contenido->id, contenido->path,
							contenido->fcount, contenido->bcount);
		}
=======
}

/* 	allocateDir: procedimiento que se encarga de asignarle un directorio a un hi-
* 	lo ociosio, sin trabajo.
*	Parametros de entrada:
* 	- t: apuntador a la estructura del hilo.	
*/
void allocateDir(Threadstruct *master){
	Node *c, *d;
	Threadstruct *t;
	char dir[4096];

	// Se verifica si hay directorios pendientes por asignar o hay hilos ociosos.
	while(!empty(master->dirlist) || onwork(master->thnum, master->idlelist)){
		

		if(master->idlelist->size != 0 && master->dirlist->size != 0){

			// Obtenemos hilo ociosio, sin trabajo.
			pthread_mutex_lock(&idlemutex);
			c = get(master->idlelist);
			t = (Threadstruct *) c->content;
			pthread_mutex_unlock(&idlemutex);

			// Obtenemos directorio pendiente por explorar.
			pthread_mutex_lock(&dirmutex);
			d = get(master->dirlist);
			
			// Asignamos directorio al hilo ocioso
			t->has_dirassigned = 1;
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
	Information *contenido;
	bool archivo;
	char *std;
	strcpy(std, "stdout");

	archivo = false;

	if(strcmp(out,std)){
		f = fopen(out, "w");
		archivo = true;
	}

	if(master->infolist->size){
		first = master->infolist->first;
		contenido = (Information *) first->content;

		if (archivo){
			fprintf(f, "%ld %s %d %d\n", contenido->id, contenido->path,
							contenido->fcount, contenido->bcount);
		}
		else{
			fprintf(stdout, "%ld %s %d %d\n", contenido->id, contenido->path,
							contenido->fcount, contenido->bcount);
		}
>>>>>>> Stashed changes

		next = first->next;

		while(next){
			contenido = (Information *) next->content;
			if (archivo){
				fprintf(f, "%ld %s %d %d\n", contenido->id, contenido->path,
								contenido->fcount, contenido->bcount);
			}
			else{
				fprintf(stdout, "%ld %s %d %d\n", contenido->id, contenido->path,
									contenido->fcount, contenido->bcount);
			}
			next = next->next;
		}
	}
}
