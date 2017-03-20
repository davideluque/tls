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

void *printHello(void *tstruct){

	LISTA

	while(list->size){
		
		MUTEX_LOCK
		HILO AGARRA DIRECTORIO DE LA LISTA <> 
		LO SACA DE LA LISTA
		MUTEX_UNLOCK
		HILO LO REVISA

		MUTEX_LOCK
		AÑADE
		MUTEX_UNLOCK

		RETORNA
		REPITE
	}

	threadstruct *ts;

	ts = tstruct;

	printf("Hola desde el hilo %ld\n", ts->threadid);

	//pthread_exit(NULL);
}

void init_threadstruct(threadstruct *ts){
	ts->threadid = 0;
	ts->list = NULL;
}

void createThreads(int NUM_THREADS){

	long *taskids[NUM_THREADS];
	pthread_t threads[NUM_THREADS];
	int hilo, rc, i;

	for (hilo = 0; hilo < NUM_THREADS; t++){

		threadstruct *ts = (threadstruct *) malloc(sizeof(threadstruct));
		init_threadstruct(ts);

		taskids[hilo] = (long *) malloc(sizeof(long));
		
		*taskids[hilo] = hilo;

		ts->threadid = taskids[hilo];
		
		printf("Creando hilo %ld\n", hilo);
		
		rc = pthread_create(&threads[hilo], NULL, ciclodeHilos, (void *) ts);
		
		if (rc) perror("Error al crear hilo..\n");
	}

	for(i=0; i< NUM_THREADS; i++) pthread_join(threads[i], NULL);	
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