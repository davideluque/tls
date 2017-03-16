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

void init_element(Element *e, char* dname){
	e->name = dname;
	e->prev = NULL;
	e->next = NULL;
}

void init_lista(List *l){
	l->first = NULL;
	l->last = NULL;
	l->size = 0;
}

void add(List *l, char* dname){

	Element *e = (Element *) malloc(sizeof(Element));
	init_element(e, dname);

	if(!l->first){
		l->first = e;
		l->last = e;
	}
	else{
		l->last->next = e;
		e->prev = l->last;
		l->last = e;
	}

	l->size++;

}