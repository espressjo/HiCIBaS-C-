LGTK =  `pkg-config --libs gtkmm-3.0`
IGTK =  `pkg-config --cflags gtkmm-3.0`
CFLAGS = -std=c++11
TARGET = exampleWindow

SOBJS = ../socket_gui/socket_.o \
	 ../socket_gui/udp_client_socket.o

socket: $(SOBJS)

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

shared_telemetry.o: shared_telemetry.cpp
	g++ $(CFLAGS) -c $< -o $@
all: socket shared_telemetry.o 
	g++ $(CFLAGS) -c HiCIBaS_window.cpp main.cpp -I../socket_gui $(IGTK)
	g++ $(CFLAGS) *.o $(SOBJS)  -o $(TARGET) $(LGTK)
clean:
	rm -f *.o
	rm -f $(TARGET)
install:
	cp $(TARGET) /opt/HiCIBaS/bin
