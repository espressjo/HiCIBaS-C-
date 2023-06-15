INCL = -I/opt/rm8/include

LIB = -L/opt/rm8/lib

TARGET = rm8

BIN = /opt/rm8/bin

CFLAGS = -std=c++11

OBJS = 	main.o \
	rm8_config.o \
	rm8.o

RM8 : ArcusPerformaxDriver.c
	gcc -c ArcusPerformaxDriver.c -o ArcusPerformaxDriver.o -lusb-1.0

ff : $(OBJS)


%.o: %.cpp
	g++ $(CFLAGS) $(INCL) -c $< -o $@ -lrm8



all: RM8 ff
	g++ $(CFLAGS) $(OBJS) ArcusPerformaxDriver.o -o $(TARGET) $(INCL) $(LIB) -luics -pthread -lusb-1.0
clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f *.so
install:
	cp $(TARGET) $(BIN)/$(TARGET)

