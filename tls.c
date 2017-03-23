/*
 * 			Thread-list
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *  
 * Universidad Simón Bolívar
 * Caracas, Venezuela
*/

#include "includes.h"
#include "tls.h"
#include "list.c"

void help(void){
	// Try to execute pager? If not, print?
	execlp("/usr/bin/pager", "/usr/bin/pager", "help");
}

void usage(void){
	printf("uso: tls [-h] | [-n i] [-d directorio] [salida]\n");
	printf("ejecute tls -h para mayor información\n");
	exit(0);
}

void init_inputargs(Inargs *in){
	in->concurrency = 1;
	getcwd(in->directory, 4096);
	strcpy(in->out, "salida");
}

void parseArgs(Inargs *in, int argc, char *argv[]){
	
	int ch, index;
	bool h, nd;

	h = false;
	nd = false;
	while( (ch = getopt(argc, argv, "hn:d:")) != -1)
		switch(ch){
			case 'h': 
				h = true;
				if(nd) usage;
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

	return;

}

void init_threadstruct(threadstruct *ts, long id, List *dirlist, ResList *reslist, 
						pthread_mutex_t dirmutex, pthread_mutex_t resmutex,
						pthread_mutex_t stamutex, threadstruct *tsarray[]){
	ts->id = id;
	ts->status = 0;
	ts->dir = NULL;
	ts->dirlist = dirlist;
	ts->reslist = reslist;
	ts->dirmutex = dirmutex;
	ts->resmutex = resmutex;
	ts->stamutex = stamutex;

	threadstruct *arreglo;
	arreglo = *tsarray;

	ts->tsarray = arreglo;
}

bool working(threadstruct *ts){
	int i, size;
	bool w;

	size = sizeof(ts->tsarray) / sizeof(ts->tsarray[0]);

	pthread_mutex_lock(&ts->stamutex);
	w = false; // We assume that all threads are not working 
	for (i = 0; i < size; i++){
		if(ts->tsarray[i].status) w = true;
	}
	pthread_mutex_unlock(&ts->stamutex);

	return w;

}

void init_reselement(ResElement *r, long id, char *path, int fcount, int bcount){
	r->id = id;
	strcpy(r->path, path);
	r->fcount = fcount;
	r->bcount = bcount;
}

void threadexplore(threadstruct *ts){

	DIR *dp;
	struct dirent *ep;
	
	dp = opendir(ts->dir);

	if(!dp){
		perror("No fue posible abrir el directorio\n");
		return;
	}

	int filescount, bytescount;

	while((ep = readdir(dp)) != NULL){	

		// Ignore hiden directories
		if(ep->d_name[0] != '.'){

			struct stat statbuffer;

			if(stat(ep->d_name, &statbuffer) == -1){
				perror("No se pudo obtener la información del directorio\n");
				exit(-1);
			}

			if(S_ISDIR(statbuffer.st_mode)){
				char subdir[1024];
				strcpy(subdir, ts->dir);
				strcat(subdir, "/");
				strcat(subdir, ep->d_name);
				pthread_mutex_lock(&ts->dirmutex);
				add(ts->dirlist, subdir);
				pthread_mutex_unlock(&ts->dirmutex);
			}

			else{
				filescount++;
				bytescount++;
			}
		}
	}

	ResElement *r = (ResElement *) malloc(sizeof(ResElement));
	init_reselement(r, ts->id, ts->dir, filescount, bytescount);

	pthread_mutex_lock(&ts->resmutex);
	addRes(ts->reslist, r);
	pthread_mutex_unlock(&ts->resmutex);

	ts->dir = NULL;

	return;

}

void *threadmgmt(void *tstruct){

	threadstruct *ts;
	ts = tstruct;

	// Each thread waits for the master thread to allocates them dirs. 
	while(!empty(ts->dirlist) || working(ts)){
		if(ts->dir){
			printf("El hilo %ld comenzara a explorar %s\n", ts->id, ts->dir);
			threadexplore(ts);
			pthread_mutex_lock(&ts->stamutex);
			ts->status = 0;
			pthread_mutex_unlock(&ts->stamutex);
		}
		continue;
	}

	pthread_exit(NULL);
}

void createThreads(int numthreads, List *dirlist, 
					pthread_mutex_t dirmutex, pthread_t threads[], 
					threadstruct *tsarray[]){

	pthread_mutex_t resmutex;
	pthread_mutex_init(&resmutex, NULL);

	pthread_mutex_t stamutex;
	pthread_mutex_init(&stamutex, NULL);

	int thread, rc, i;

	ResList *reslist = (ResList *) malloc(sizeof(ResList));

	for (thread = 0; thread < numthreads; thread++){

		threadstruct *ts = (threadstruct *) malloc(sizeof(threadstruct));
		init_threadstruct(ts, thread, dirlist, reslist, dirmutex, resmutex, stamutex, tsarray);
		tsarray[thread] = ts;

		rc = pthread_create(&threads[thread], NULL, threadmgmt, (void *) tsarray[thread]);
		if (rc){
			printf("Error al crear hilo. CodError:%d\n", rc);
			exit(-1);
		}
	}
}

void allocateDir(int numthreads, List *dirlist, threadstruct *tsarray[], 
					pthread_mutex_t dirmutex){

	int thread;

	// Master thread will allocate directories while the list isnt empty and
	// there arent threads working
	while(!empty(dirlist) || working(tsarray[0])){

		for(thread = 0; thread < numthreads; thread++){

			if(!tsarray[thread]->status && !empty(dirlist)){

				pthread_mutex_lock(&tsarray[0]->stamutex);
				tsarray[thread]->status = 1;
				pthread_mutex_unlock(&tsarray[0]->stamutex);

				pthread_mutex_lock(&dirmutex);
				printf("Le voy a asignar un directorio al hilo xs\n");
				tsarray[thread]->dir = get(dirlist);
				pthread_mutex_unlock(&dirmutex);
			}
			continue;
		}
	}

}

void explore(char *directory, List *list){

	DIR *dp;
	struct dirent *ep;

	dp = opendir(directory);

	if(!dp){
		perror("No fue posible abrir el directorio\n");
		return;
	}

	while((ep = readdir(dp)) != NULL){	

		// Ignore hiden directories
		if(ep->d_name[0] != '.'){

			struct stat statbuffer;

			if(stat(ep->d_name, &statbuffer) == -1){
				perror("No se pudo obtener la información del directorio\n");
				exit(-1);
			}

			if(S_ISDIR(statbuffer.st_mode)){
				//char subdir[1024];
				//strcpy(subdir, directory);
				//strcat(subdir, "/");
				//strcat(subdir, ep->d_name);
				//strcat(subdir, "/");
				add(list, ep->d_name);
			}
		}
	}
}

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
