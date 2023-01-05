LGTK =  `pkg-config --libs gtkmm-3.0`
IGTK =  `pkg-config --cflags gtkmm-3.0`
TARGET = hi_script_manager
INCL = -I../HiCIBaS-GUI

window: 
	cd ../HiCIBaS-GUI && make all
socket:
	cd ../socket_gui && make 
all: socket window
	g++ -std=c++11 -c scriptwindow.cpp main.cpp -I../socket_gui $(INCL) $(IGTK)
	g++ -std=c++11 *.o ../socket_gui/socket_.o ../HiCIBaS-GUI/HiCIBaS_window.o $(INCL) -o $(TARGET) $(LGTK)
clean:
	rm -f *.o
	cd ../socket_gui && make clean
install:
	cp $(TARGET) /opt/HiCIBaS/bin
