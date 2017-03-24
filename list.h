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
 * Estructura para las listas.
 *
*/
typedef struct node{
	struct node *prev;
	struct node *next;
  void *content;
} Node;

typedef struct directory{
  char dir[4096];
} Directory;

typedef struct information{
  long id;
  char path[4096];
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

bool empty(List *l);

Node *get(List *l);
