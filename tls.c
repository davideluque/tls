/*
 * 			Thread-list
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *  
 * Universidad Simón Bolívar
 * Caracas, Venezuela
*/

// #include "includes.h"
#include "tls.h"

void help(void){
	execlp("/usr/bin/pager", "/usr/bin/pager", "help");
}

void usage(void){
	printf("uso: tls [-h] | [-n i] [-d directorio] [salida]\n");
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
				// Verificar que el argumento sea válido (un número) o enviar a
				// usage.
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

	printf("concurrencia: %d\n", options->concurrency);
	printf("directorio: %s\n", options->directory);
	printf("salida: %s\n", options->out);

	//for (index = optind; index < argc; index++)
	//  	printf("Argumento opcional %s\n", argv[index]);

	return;

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



	return 0;
}