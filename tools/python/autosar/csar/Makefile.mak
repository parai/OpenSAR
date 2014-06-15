

all:
	gcc -c can.c -o can.o
	ar -r libcan.a can.o