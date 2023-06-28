INCL = -I/opt/HiCIBaS/include \
	-I../py_manager \
	-I../py_proc \
        -I../HiCIBaS-GUI \
	-I../nutec-server/src \
	-I../rm8-server/src/	
TARGET = HiCIBaS
CFLAGS = -std=c++11
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


OBJS = 	../HiCIBaS-GUI/ui_config_file.o \
	lim_switch.o \
	config_file.o \
	python_cmd.o \
	telemetry.o \
	shared_tcs.o \
	py_scripts_config.o \
	getstatus.o \
	shared_tcs_thread.o \
	socket_.o \
	udp_client_socket.o \
	utility_socket.o \
	motor_status.o \
	main.o

POBJS = ../py_manager/py_manager.o \
     ../py_proc/python_proc.o

ff : $(OBJS)
py : $(POBJS)

%.o: %.cpp
	g++ $(CFLAGS) $(INCL) -c $< -o $@



HiCIBaS: ff py
	g++ -std=c++11 $(OBJS) $(POBJS) -o $(TARGET) $(INCL) -L/opt/HiCIBaS/lib -luics -pthread -lLabJackM
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
	#cp ../config/scripts.txt /opt/HiCIBaS/scripts
all:folders uics HiCIBaS
	echo "done!"
