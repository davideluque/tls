/*
 * 			Thread-project
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *
 * Descripción: Cabecera del archivo list.c que contiene las librerías
 * necesarias, prototipos de estructuras y de funciones.
 *
 * Universidad Simón Bolívar
 * Caracas, Venezuela
 * Marzo, 2017.
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * Estructura que se almacena en las listas.
 *
 * prev: node anterior en la lista.
 * next: node siguiente en la lista.
 * content: en este programa content puede ser Directory, Information o
 * Threadstruct
*/
typedef struct node{
	struct node *prev;
	struct node *next;
  void *content;
} Node;

/*
 * Estructura para almacenar los directorios a explorar en los nodos de las
 * listas.
*/
typedef struct directory{
  char dir[PATH_MAX];
} Directory;


/*
 * Estructura que almacena la información encontrada a lo largo de la
 * exploración de los directorios.
*/
typedef struct information{
  long id;
  char path[PATH_MAX];
  int fcount;
  int bcount;
} Information;

/*
 * Estructura que permite el uso de listas durante el programa.
*/
typedef struct list{
  Node *first;
  Node *last;
  int size;
} List;

/*  init_node: procedimiento que se encarga de inicializar los atributos de un
*   nodos.
*   Parametros de entrada:
*   - Node: objeto tipo Node que se inicializara.
*   - Content: contenido del nodo.
*/
void init_node(Node *n, void *content);

/*  init_list: procedimiento que se encargara de inicializara los valores de los
*   elemento de una lista.
*   Parametros de entrada:
*   - List: lista a la cual se quiere inicializar.
*/
void init_list(List *l);

/*  add: procedimiento que se encargara de añadir un elemento a la lista.
*   Parametros de entrada:
*   - Node: nodo que se quiere añadir a la lista.
*   - List: lista a la cual se añadira el nodo.
*/
void add(List *l, Node *n);

/*  get: procedimiento que se encargara de retornar un elemento tipo Node de la
*   list.
*   Parametros de entrada:
*   - List: lista en la cual se quiere obtener un elemento tipo Node.
*/
bool empty(List *l);

/*  empty: procedimiento que se encarga de verificar si la lista esta vacia.
*   Parametros de entrada:
*   - List: lista en la que se verificara si esta llena o vacia.
*/

Node *get(List *l);

bool empty(List *l);
