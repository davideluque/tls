##
 # 			Thread-project
 #
 # Autor: David Cabeza <13-10191@usb.ve>
 # Autor: Fabiola Martínez <13-10838@usb.ve>
 #
 # Descripción: Este archivo facilita la compilación del programa tls.
 #
 # Universidad Simón Bolívar
 # Caracas, Venezuela
##

tls: main.o tls.o list.o
	gcc main.o tls.o list.o -o tls -lpthread

main.o: tls.o main.c
	gcc -c main.c

tls.o: tls.h tls.c
	gcc -c tls.c -o tls.o

list.o: list.h list.c
	gcc -c list.c -o list.o

clean:
	rm *.o tls
