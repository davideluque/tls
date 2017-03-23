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