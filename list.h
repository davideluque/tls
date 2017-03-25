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
 * Estructura de nodos para las listas.
 *
*/
typedef struct node{
	struct node *prev;
	struct node *next;
  void *content;
} Node;

typedef struct directory{
  char dir[PATH_MAX];
} Directory;

typedef struct information{
  long id;
  char path[PATH_MAX];
  int fcount;
  int bcount;
} Information;

typedef struct list{
  Node *first;
  Node *last;
  int size;
} List;

void init_node(Node *n, void *content);

void init_list(List *l);

void add(List *l, Node *n);

Node *get(List *l);

bool empty(List *l);
