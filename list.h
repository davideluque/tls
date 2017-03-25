/*
 * 			Thread-list
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *
 * Universidad Simón Bolívar
 * Caracas, Venezuela
*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
<<<<<<< Updated upstream
 * Estructura para las listas.
 *
=======
 * Estructura que se almacena en las lista.
 * pre: node anterior en la lista.
 * next: node siguiente en la lista.
 * content: almacena contenido necesario.
>>>>>>> Stashed changes
*/
typedef struct node{
	struct node *prev;
	struct node *next;
  void *content;
} Node;

<<<<<<< Updated upstream
=======
/* 
*/
>>>>>>> Stashed changes
typedef struct directory{
  char dir[4096];
} Directory;

<<<<<<< Updated upstream
=======
/* 
*/
>>>>>>> Stashed changes
typedef struct information{
  long id;
  char path[4096];
  int fcount;
  int bcount;
} Information;

<<<<<<< Updated upstream
=======
/* 
*/
>>>>>>> Stashed changes
typedef struct list{
  Node *first;
  Node *last;
  int size;
} List;

<<<<<<< Updated upstream
void init_node(Node *n, void *content);

void init_list(List *l);

bool empty(List *l);

=======
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
>>>>>>> Stashed changes
Node *get(List *l);
