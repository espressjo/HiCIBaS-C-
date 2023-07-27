INCL = -I/opt/nutec/include
LIB = -L/opt/nutec/lib
TARGET = nutec
BIN = /opt/nutec/bin

CFLAGS = -std=c++11

OBJS = 	main.o \
	serial.o \
	nutec.o \
	status_cmd.o \
	nutec_config.o

ff : $(OBJS)


%.o: %.cpp
	g++ $(CFLAGS) $(INCL) -c $< -o $@



all: ff 
	g++ $(CFLAGS) $(OBJS) -o $(TARGET) $(INCL) $(LIB) -luics -pthread
clean:
	rm -f *.o
	rm -f $(TARGET)
install:
	cp $(TARGET) $(BIN)/$(TARGET)
