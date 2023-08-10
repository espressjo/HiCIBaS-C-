LGTK =  `pkg-config --libs gtkmm-3.0`
IGTK =  `pkg-config --cflags gtkmm-3.0`

TARGET = HiCIBaS-Guiding
CC = g++
INCL = -I../HiCIBaS-GUI \
       -I../led-widget \
       -I../socket_gui \
       -I../UICS/src \
       -I../py_manager \
       -I../py_proc \
       -I../rm8-server/src \
       -I../nutec-server/src \
       -I../src
PYTHON = ../py_proc/python_proc.o \
	 ../py_manager/py_manager.o \
	 ../src/py_scripts_config.o
CFLAGS = -std=c++11

SOBJS = ../socket_gui/socket_.o \
         ../socket_gui/udp_client_socket.o

GOBJS = ../HiCIBaS-GUI/HiCIBaS_window.o \
	../HiCIBaS-GUI/shared_telemetry.o \
	../HiCIBaS-GUI/ui_config_file.o \
	../led-widget/ledwidget.o

srccpp = guiding_window.o \
	 cam_config_window.o \
      main.o
pymanager: $(PYTHON)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCL) -c $< -o $@ $(IGTK)

src : $(srccpp)
socket: $(SOBJS)
gui: $(GOBJS)

all: socket pymanager gui src 
	g++ -std=c++11 *.o $(SOBJS) $(GOBJS) $(PYTHON) $(INCL) -o $(TARGET) $(LGTK)
clean:
	rm -f *.o
	rm -f $(TARGET)
install:
	cp $(TARGET) /opt/HiCIBaS/bin
	cp ../python/script/move.py /opt/HiCIBaS/scripts
