LGTK =  `pkg-config --libs gtkmm-3.0`
IGTK =  `pkg-config --cflags gtkmm-3.0`

TARGET = HiCIBaS-Guiding
CC = g++
INCL = -I../HiCIBaS-GUI \
       -I../socket_gui \
       -I../UICS/src \
       -I../src

CFLAGS = -std=c++11

SOBJS = ../socket_gui/socket_.o \
         ../socket_gui/udp_client_socket.o

GOBJS = ../HiCIBaS-GUI/HiCIBaS_window.o \
	../HiCIBaS-GUI/shared_telemetry.o \
	../HiCIBaS-GUI/ui_config_file.o

srccpp = guiding_window.o \
      main.o


%.o: %.cpp
	$(CC) $(CFLAGS) $(INCL) -c $< -o $@ $(IGTK)

src : $(srccpp)
socket: $(SOBJS)
gui: $(GOBJS)

all: socket gui src
	g++ -std=c++11 *.o $(SOBJS) $(GOBJS) $(INCL) -o $(TARGET) $(LGTK)
clean:
	rm -f *.o
	rm -f $(TARGET)
install:
	cp $(TARGET) /opt/HiCIBaS/bin
