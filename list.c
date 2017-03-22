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

void init_list(List *l){
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

void addRes(ResList *l, ResElement *r){

	if(!l->first){
		l->first = r;
		l->last = r;
	}
	else{
		l->last->next = r;
		r->prev = l->last;
		l->last = r;
	}

	l->size++;
}

char *get(List *l){

	if(l->size == 0){
		printf("No hay directorios \n");
		return;
	}
	else {
		Element *tmp = l->first;

		if(l->size == 1){
			l->first = NULL;
			l->last = NULL;
			printf("Tenia tamaño uno y se supone que esta en tmp %ld\n", tmp->name);
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

			l->size--;
			return tmp->name;
		}
	}
}


bool empty(List *l){
	
	if(l->size) return false;
	else return true;
}