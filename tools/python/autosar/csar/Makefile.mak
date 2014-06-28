

all:
	gcc -c can.c -o can.o
	ar -r libcan.a can.o
	"D:\Program Files (x86)\Microsoft Visual Studio\VC98\Bin\NMAKE" /f Makefile