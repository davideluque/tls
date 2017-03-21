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

void allocateDir(int numthreads, threadstruct ts){

	int thread;

	while(true){

		// La cola esta vacia? Revisar despues de que todos los hilos esten desocupados
		for(thread = 0; thread < numthreads; thread++){
			if(!ts[thread]->status){
				ts[thread]->status = 1;
				pthread_mutex_lock(bloqueo);
				ts[thread]->dir = remove();
			}
		}
	}

}

void *threadmgmt(void *tstruct){

	threadstruct *ts;
	ts = tstruct;

	// Each created thread waits for dir to be allocated
	while(true){
		if(ts->dir){
			threadexplore(ts->dir)			
		}
	}


	/*
		MUTEX_UNLOCK
		HILO LO REVISA

		MUTEX_LOCK
		AÑADE
		MUTEX_UNLOCK

		RETORNA
		REPITE
	}*/

	printf("Hola desde el hilo %ld\n", ts->threadid);

	//pthread_exit(NULL);
}

void init_threadstruct(threadstruct *ts, int id){
	ts->threadid = id;
	ts->status = 0;
}

void createThreads(int numthreads){

	pthread_t threads[numthreads];
	threadstruct *threadstructs[numthreads];
	int hilo, rc, i;

	for (hilo = 0; hilo < numthreads; hilo++){

		threadstruct *ts = (threadstruct *) malloc(sizeof(threadstruct));
		init_threadstruct(ts, hilo);
		threadstructs[hilo] = ts;

		rc = pthread_create(&threads[hilo], NULL, threadmgmt, (void *) threadstructs[hilo]);
		if (rc) printf("Error al crear hilo. CodError:%d\n", rc);
	}

	allocateDir(numthreads, threadstructs);

	for(i=0; i< numthreads; i++) pthread_join(threads[i], NULL);	
}

void explore(char *directory, List *list){

	DIR *dp;
	struct dirent *ep;

	dp = opendir(directory);

	if(!dp){
		perror("No fue posible abrir el directorio\n");
		exit(-1);
	}

	while((ep = readdir(dp)) != NULL){	

		// Ignore hiden directories
		if(ep->d_name[0] != '.'){

			struct stat statbuffer;

			if(stat(ep->d_name, &statbuffer) == -1){
				perror("No se pudo obtener la información del directorio\n");
				exit(-1);
			}

			if(S_ISDIR(statbuffer.st_mode)) add(list, ep->d_name);
		}
	}
}

int main(int argc, char *argv[])
{

	/*
	 * Por defecto los valores de los argumentos son inicializados mediante la 
	 * función init_inputargs con 1, directorio de trabajo actual y "salida" 
	 * respectivamente.
	 *
	*/
	Inargs *in = (Inargs *) malloc(sizeof(Inargs));
	init_inputargs(in);

	parseArgs(in, argc, argv);

	List *list = (List *) malloc(sizeof(List));
	init_list(list);

	explore(in->directory, list);

	createThreads(in->concurrency);

	return 0;
}