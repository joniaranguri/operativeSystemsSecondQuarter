all: ejercicio3 consultar consumir
ejercicio3:  ejercicio3.o
	gcc -o ej3 ejercicio3.o -lpthread -lrt
ejercicio3.o:  ejercicio3.c
	gcc -c ejercicio3.c 
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