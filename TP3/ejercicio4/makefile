all: ejercicio4 consultar consumir
ejercicio4:  ejercicio4.o
	gcc -o ej4 ejercicio4.o -lpthread -lrt
ejercicio4.o:  ejercicio4.c
	gcc -c ejercicio4.c 
consultar: consultor.o
	gcc -o consultar consultor.o -lpthread -lrt
consultor.o: consultor.c 
	gcc -c consultor.c 
consumir: consumidor.o
	gcc -o consumir consumidor.o -lpthread -lrt
consumidor.o: consumidor.c 
	gcc -c consumidor.c 
clean:
	rm *.o