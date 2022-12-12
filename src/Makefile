folders:
	mkdir -p /opt/HiCIBaS
	mkdir -p /opt/HiCIBaS/config
	mkdir -p /opt/HiCIBaS/bin
	mkdir -p /opt/HiCIBaS/log
	mkdir -p /opt/HiCIBaS/lib
	mkdir -p /opt/HiCIBaS/include
uics:
	cd ../UICS/src && make clean all install
	cp ../UICS/lib/libuics.a /opt/HiCIBaS/lib
	cp ../UICS/lib/uics.h /opt/HiCIBaS/include
	cp ../UICS/src/*.h /opt/HiCIBaS/include
	cp ./insthandle.h /opt/HiCIBaS/include #override insthandler with the one in ./src
HiCIBaS:
	g++ -std=c++11 -c main.cpp -I/opt/HiCIBaS/include 
	cd ../py_proc && make all
	cd ../py_manager && make all
	g++ -std=c++11 *.o ../py_manager/py_manager.o ../py_proc/python_proc.o -o HiCIBaS -I../py_proc -I../py_manager -I/opt/HiCIBaS/include -L/opt/HiCIBaS/lib -luics -pthread
clean:
	rm -f *.o
	rm -f HiCIBaS
	cd ../UICS/src && make clean
	rm -f /opt/HiCIBaS/lib/libuics.a
	rm -f /opt/HiCIBaS/include/*.h
install:
	cp ../config/cmd.cfg /opt/HiCIBaS/config/cmd.conf
	cp ../config/state.cfg /opt/HiCIBaS/config/state.conf
	cp ../config/HiCIBaS.cfg /opt/HiCIBaS/config/HiCIBaS.conf
all:folders uics HiCIBaS
	echo "done!"
