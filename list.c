/*
 * 			Thread-project
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *
 * Descripción: Implementación de procedimientos y métodos que manejan el uso
 * de listas en el programa.
 *
 * Universidad Simón Bolívar
 * Caracas, Venezuela
*/

#include "list.h"

void init_node(Node *n, void *content){
  n->next = NULL;
  n->prev = NULL;
  n->content = content;
}

void init_list(List *l){
  l->first = NULL;
  l->last = NULL;
  l->size = 0;
}

void add(List *l, Node *n){
  if(!l->first){
    l->first = n;
    l->last = n;
  }
  else{
    l->last->next = n;
    n->prev = l->last;
    l->last = n;
  }

  l->size++;
}

Node *get(List *l){

  Node *tmp;

  if(l->size == 0) printf("No hay elementos en la lista\n");

  else {
 		tmp = l->first;

 		if(l->size == 1){
 			l->first = NULL;
 			l->last = NULL;
 		}

    else{
      if (!tmp->prev){
        l->first = tmp->next;
        l->first->prev = NULL;
      }
      else if (!tmp->next){
        l->last = tmp->prev;
        l->last->next = NULL;
      }
      else{
        tmp->prev->next = tmp->next;
 				tmp->next->prev = tmp->prev;
 			}

 		}
    l->size--;
 	}
  return tmp;
}

bool empty(List *l){
  if (l->size == 0) return true;
  else return false;
}
