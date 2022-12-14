LGTK =  `pkg-config --libs gtkmm-3.0`
IGTK =  `pkg-config --cflags gtkmm-3.0`
TARGET = hi_script_manager

socket:
	cd ../socket_gui && make 
all: socket
	g++ -std=c++11 -c mainwindow.cpp main.cpp -I../socket_gui $(IGTK)
	g++ -std=c++11 *.o ../socket_gui/socket_.o -o $(TARGET) $(LGTK)
clean:
	rm -f *.o
	cd ../socket_gui && make clean
install:
	cp $(TARGET) /opt/HiCIBaS/bin
