INCL = -I/opt/HiCIBaS/include \
		-I../py_manager \
		-I../py_proc 
TARGET = HiCIBaS
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
	g++ -std=c++11 -c lim_switch.cpp $(INCL)
	g++ -std=c++11 -c shared_tcs.cpp $(INCL)
	g++ -std=c++11 -c config_file.cpp $(INCL) 
	g++ -std=c++11 -c python_cmd.cpp $(INCL) 
	g++ -std=c++11 -c main.cpp $(INCL)
	cd ../py_proc && make all
	cd ../py_manager && make all
	g++ -std=c++11 *.o ../py_manager/py_manager.o ../py_proc/python_proc.o -o $(TARGET) $(INCL) -L/opt/HiCIBaS/lib -luics -pthread -lLabJackM
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
	cp $(TARGET) /opt/HiCIBaS/bin
all:folders uics HiCIBaS
	echo "done!"
