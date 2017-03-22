/*
 * 			Thread-list
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *  
 * Universidad Simón Bolívar
 * Caracas, Venezuela
*/

/*
 * Estructura que almacena los argumentos de ejecución del programa, i.e. número
 * de hilos, directorio y nombre del archivo de salida.
 *
*/
typedef struct inargs{
	int concurrency;
	char directory[4096];
	char out[256];
} Inargs;

/*
 * Estructura para la lista de directorios a ser explorados. Almacena el nombre 
 * del directorio, un apuntador al directorio anterior y uno al siguiente.
 *
*/
typedef struct element{
	char* name;
	struct element *prev;
	struct element *next;
} Element;

/*
 *
 *
*/
typedef struct reselement{
	long id;
	char *path;
	int fcount;
	int bcount;
	struct reselement *prev;
	struct reselement *next;

} ResElement;

/*
 * Implementación de una lista. Esta es la lista de directorios a ser explorados
 * por los hilos trabajadores. Almacena el primer elemento, el último y su tamaño.
 *
*/
typedef struct list{
	Element *first;
	Element *last;
	int size;

} List;

/*
 *
 *
 *
 *
*/
typedef struct reslist{
	ResElement *first;
	ResElement *last;
	int size;

} ResList;

/*
 *
 *
 *
 *
*/
typedef struct threadstruct{
	long id;
	int status;
	char *dir;
	List *dirlist;
	ResList *reslist;
	pthread_mutex_t dirmutex;
	pthread_mutex_t resmutex;
	pthread_mutex_t stamutex;
	struct threadstruct *tsarray;

} threadstruct;

/*	
 * Ejecuta pager localizado en /usr/bin/ para visualizar el manual del programa
 * que contiene información acerca de la sintaxis, descripción de párametros, etc.
 *
*/
void help(void);

/*	
 * Se ejecuta cuando se invoca el programa con un comando inválido.
 *
*/
void usage(void);

/*	
 * Inicializa la estructura que almacenará los argumentos introducidos al invocar 
 * el programa con valores por defecto.
 *
*/
void init_inputargs(Inargs *in);

/*	
 * Analiza la sintaxis de la invocación del programa mediante la línea de comandos.
 *	
*/
void parseArgs(Inargs *in, int argc, char *argv[]);


/*	
 * Función que ejecuta el hilo principal o maestro para explorar el directorio
 * pasado mediante la línea de comandos añadiendo los directorios encontrados a 
 * list.
 *	
*/
void explore(char *directory, List *list);