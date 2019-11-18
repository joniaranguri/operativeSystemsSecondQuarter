all: prc1 prc2
prc1:  proceso1.o
	gcc -o prc1 proceso1.o -lrt
proceso1.o:  proceso1.c
	gcc -c proceso1.c 
prc2:  proceso2.o
	gcc -o prc2 proceso2.o  -lrt
proceso2.o:  proceso2.c
	gcc -c proceso2.c 
clean:
	rm *.o