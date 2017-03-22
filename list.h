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
bool empty(List *l);