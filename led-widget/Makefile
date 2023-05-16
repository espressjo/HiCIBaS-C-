LGTK =  `pkg-config --libs gtkmm-3.0`
IGTK =  `pkg-config --cflags gtkmm-3.0`

CFLAGS = -std=c++11

all:
	g++ $(CFLAGS) -c ledwidget.cpp $(IGTK)
clean:
	rm -f *.o
