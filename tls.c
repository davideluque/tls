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

void init_options(Options *opt){
	opt->concurrency = 1;
	getcwd(opt->directory, 4096);
	strcpy(opt->out, "salida");
}

void parseArgs(Options *options, int argc, char *argv[]){
	
	int ch;
	int index;

	while( (ch = getopt(argc, argv, "hn:d:")) != -1)
		switch(ch){
			case 'h': 
				help();
			case 'n':
				if (!isdigit(optarg[0])) usage();
				options->concurrency = atoi(optarg);
				break;
			case 'd':
				strcpy(options->directory, optarg);
				break;
			case '?':
				usage();
			default:
				usage();
		}

	if(optind < argc) strcpy(options->out, argv[optind]);

	return;

}

void *printHello(void *id){

	long numeroHilo;

	numeroHilo = (long) id;

	printf("Hola desde el hilo %ld\n", numeroHilo);

	//pthread_exit(NULL);
}

void createThreads(int NUM_THREADS){

	long *taskids[NUM_THREADS];
	pthread_t threads[NUM_THREADS];
	int t, rc;

	for (t = 0; t < NUM_THREADS; t++){
		taskids[t] = (long *) malloc(sizeof(long));
		*taskids[t] = t;
		printf("Creando hilo %ld\n", t);
		rc = pthread_create(&threads[t], NULL, printHello, (void *) taskids[t]);
		if (rc){
			printf("error al crear hilo..\n");
		}
	}

	printf("Hola soy el hilo maestro(?)\n");

	int i;
	for(i=0; i< NUM_THREADS; i++)
	{
	    pthread_join(threads[i], NULL);
	}

}

void explore(char *directory){

	DIR *dp;
	struct dirent *ep;

	dp = opendir(directory);

	if(!dp){
		perror("No fue posible abrir el directorio\n");
		exit(-1);
	}

	List *to_explore = (List *) malloc(sizeof(List));

	while((ep = readdir(dp)) != NULL){	

		// Ignore hiden directories
		if(ep->d_name[0] != '.'){

			struct stat statbuffer;

			if(stat(ep->d_name, &statbuffer) == -1){
				perror("No se pudo obtener la información del directorio\n");
				exit(-1);
			}

			if(S_ISDIR(statbuffer.st_mode)) add(to_explore, ep->d_name);
		}
	}

}

int main(int argc, char *argv[])
{

	/*
	 * Por defecto los valores de las opciones son inicializados mediante la 
	 * función init_options con 1, directorio de trabajo actual y "salida" 
	 * respectivamente.
	 *
	*/
	Options *options = (Options *) malloc(sizeof(Options));
	init_options(options);

	parseArgs(options, argc, argv);

	createThreads(options->concurrency);

	explore(options->directory);

	return 0;
}