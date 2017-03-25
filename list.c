/*
 * 			Thread-list
 *
 * Autor: David Cabeza <13-10191@usb.ve>
 * Autor: Fabiola Martínez <13-10838@usb.ve>
 *
 * Universidad Simón Bolívar
 * Caracas, Venezuela
*/

#include "list.h"

<<<<<<< Updated upstream
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

=======

/*  init_node: procedimiento que se encarga de inicializar los atributos de un 
*   nodos.
*   Parametros de entrada:
*   - Node: objeto tipo Node que se inicializara.
*   - Content: contenido del nodo.
*/   
void init_node(Node *n, void *content){
  n->next = NULL;
  n->prev = NULL;
  n->content = content;
}

/*  init_list: procedimiento que se encargara de inicializara los valores de los 
*   elemento de una lista.
*   Parametros de entrada:
*   - List: lista a la cual se quiere inicializar.
*/
void init_list(List *l){
  l->first = NULL;
  l->last = NULL;
  l->size = 0;
}

/*  add: procedimiento que se encargara de añadir un elemento a la lista.
*   Parametros de entrada:
*   - Node: nodo que se quiere añadir a la lista.
*   - List: lista a la cual se añadira el nodo.
*/
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
=======
/*  get: procedimiento que se encargara de retornar un elemento tipo Node de la 
*   list.
*   Parametros de entrada:
*   - List: lista en la cual se quiere obtener un elemento tipo Node.
*/
>>>>>>> Stashed changes
Node *get(List *l){

  Node *tmp;

  if(l->size == 0){
	   printf("No hay nada\n");
<<<<<<< Updated upstream
     return;
=======
>>>>>>> Stashed changes
 	}

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
    return tmp;
 	}
}

<<<<<<< Updated upstream
=======
/*  empty: procedimiento que se encarga de verificar si la lista esta vacia.
*   Parametros de entrada:
*   - List: lista en la que se verificara si esta llena o vacia.
*/  
>>>>>>> Stashed changes
bool empty(List *l){
  if (l->size == 0){
    return true;
  }
  else {
    return false;
  }
}
