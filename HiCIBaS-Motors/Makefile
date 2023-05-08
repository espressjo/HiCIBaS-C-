LGTK =  `pkg-config --libs gtkmm-3.0`
IGTK =  `pkg-config --cflags gtkmm-3.0`
TARGET = HiCIBaS-Motors

INCL = -I../HiCIBaS-GUI \
       -I../socket_gui \
       -I../UICS/src \
       -I../src


SOBJS = ../socket_gui/socket_.o \
         ../socket_gui/udp_client_socket.o
GOBJS = ../HiCIBaS-GUI/HiCIBaS_window.o \
	../HiCIBaS-GUI/shared_telemetry.o \
	../HiCIBaS-GUI/ui_config_file.o

socket: $(SOBJS)

gui: $(GOBJS)

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@




window: 
	cd ../HiCIBaS-GUI && make all
all: socket window gui
	g++ -std=c++11 -c motors_window.cpp main.cpp $(INCL) $(IGTK)
	g++ -std=c++11 *.o $(SOBJS) $(GOBJS) $(INCL) -o $(TARGET) $(LGTK)
clean:
	rm -f *.o
install:
	cp $(TARGET) /opt/HiCIBaS/bin
