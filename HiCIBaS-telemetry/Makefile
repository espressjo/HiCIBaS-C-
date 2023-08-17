LGTK =  `pkg-config --libs gtkmm-3.0`
IGTK =  `pkg-config --cflags gtkmm-3.0`
TARGET = HiCIBaS-Telemetry

INCL = -I../HiCIBaS-GUI \
       -I../socket_gui \
       -I../UICS/src \
       -I../src \
       -I../led-widget


SOBJS = ../socket_gui/socket_.o \
         ../socket_gui/udp_client_socket.o \
	../UICS/src/b64.o \
	../src/telemetry.o
GOBJS = ../HiCIBaS-GUI/HiCIBaS_window.o \
	../HiCIBaS-GUI/shared_telemetry.o \
	../HiCIBaS-GUI/ui_config_file.o \
	../led-widget/ledwidget.o

socket: $(SOBJS)

gui: $(GOBJS)

%.o: %.cpp
	g++ $(CFLAGS) $(INCL) -c $< -o $@




window: 
	cd ../HiCIBaS-GUI && make all
all: socket window gui
	g++ -std=c++11 -c telemetry_control.cpp astro_stuff.cpp telemetry_window.cpp main.cpp $(INCL) $(IGTK)
	g++ -std=c++11 *.o $(SOBJS) $(GOBJS) $(INCL) -o $(TARGET) $(LGTK)
clean:
	rm -f *.o
install:
	cp $(TARGET) /opt/HiCIBaS/bin
	cp *.glade /opt/HiCIBaS/etc
