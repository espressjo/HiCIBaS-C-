
#ifndef cfgpath
cfgpath = /opt/HiCIBaS/config
#endif
#ifndef basepath
basepath = /opt/HiCIBaS
#endif

INCL = -I$(basepath)/include \
	-I../py_manager \
	-I../py_proc \
        -I../HiCIBaS-GUI \
	-I../nutec-server/src \
	-I../rm8-server/src/	
TARGET = HiCIBaS
CFLAGS = -std=c++11

uics: 
	@cd ../UICS/src && make cfgpath=$(cfgpath) all
	@cp ../UICS/src/libuics.a $(basepath)/lib
	@cp ../UICS/src/*.h $(basepath)/include
	@cp ./insthandle.h $(basepath)/include #override insthandler with the one in ./src


OBJS = 	../HiCIBaS-GUI/ui_config_file.o \
	ljack.o \
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

clean:
	@rm -f *.o
	@rm -f HiCIBaS
	@cd ../UICS/src && make clean

install:
	@cp ../config/cmd.cfg $(basepath)/config/cmd.conf
	@cp ../config/state.cfg $(basepath)/config/state.conf
	@cp ../config/HiCIBaS.cfg $(basepath)/config/HiCIBaS.conf
	@cp ../config/network.cfg $(basepath)/config/network.conf
	@cp $(TARGET) $(basepath)/bin
all: uics ff py
	g++ -std=c++11 $(OBJS) $(POBJS) -o $(TARGET) $(INCL) -L$(basepath)/lib -luics -pthread -lLabJackM
