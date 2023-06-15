TARGET = client

all:
	g++ -std=c++11 -c udp_client_socket.cpp -o udp_client_socket.o
	g++ -std=c++11 -c client.cpp -o client.o
	g++ -std=c++11 *.o -o $(TARGET)
clean:
	rm -f *.o
	rm -f $(TARGET)
