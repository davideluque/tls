/*
 * 			Thread-list
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *  
 * Universidad Simón Bolívar
 * Caracas, Venezuela
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
 * Inicializa el elemento que va en la lista con el nombre del directorio y
 * apuntadores al anterior y siguiente elemento.
 *
*/
void init_element(Element *e, char* dname);

/*	
 * Inicializa la lista de directorios a ser explorados.
 *
*/
void init_list(List *l);

/*
 * Función para añadir un elemento que contiene el directorio a ser explorado a
 * la lista l.
 *
*/
void add(List *l, char *dname);

/*
 *
 *
 *
*/
void addRes(ResList *l, ResElement *r);

/*
 *
 *
 *
*/
char *get(List *l);

/*
 *
 *
 *
*/
bool empty(List *l);

