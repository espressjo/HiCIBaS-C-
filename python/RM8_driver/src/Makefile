all:
	gcc -c -fPIC ArcusPerformaxDriver.c -o ArcusPerformaxDriver.o -lusb-1.0
	gcc -c -fPIC rm8_driver.c -o rm8_driver.o
	gcc ArcusPerformaxDriver.o rm8_driver.o -shared -o librm8.so -L/usr/lib/x86_64-linux-gnu -lusb-1.0
clean:
	rm -f *.o 
	rm -f *.so
install:
	
	mkdir -p /opt/HiCIBaS/lib
	mkdir -p /opt/HiCIBaS/python
	cp librm8.so /opt/HiCIBaS/lib
	cp ../rm8.py /opt/HiCIBaS/python
