mlib:
	cd ../ && make clean
	cd ../ && make all
all: mlib
	g++ -std=c++11 -c test.cpp threaded_f1.cpp -I./ -I../src
	g++ -std=c++11 -L/home/noboru/Documents/UdeM/HxRG-SERVER2/UICS/lib -o uics -I. -I../src -I../lib  main.cpp test.o threaded_f1.o -luics -lpthread
clean:
	rm -f *.o
