BASE = /opt/HiCIBaS
CONFPATH := /opt/nutec/config
INITPATH := /opt/nutec/config
folder:
	mkdir -p $(BASE)/bin
	mkdir -p $(BASE)/config
	mkdir -p $(BASE)/etc
	mkdir -p $(BASE)/include
	mkdir -p $(BASE)/lib
all:folder
	cd ./src && make all
	cd ./HiCIBaS-Script && make all
	cd ./HiCIBaS-telemetry && make all
	cd ./HiCIBaS-Motors && make all
clean:
	cd ./socket_gui && make clean
	cd ./src && make all
	cd ./HiCIBaS-Script && make all
	cd ./HiCIBaS-telemetry && make all
	cd ./HiCIBaS-Motors && make all
install:
	cd ./src && make install
	cd ./HiCIBaS-Script && make install
	cd ./HiCIBaS-telemetry && make install
	cd ./HiCIBaS-Motors && make install
	cp ./config/network.cfg /opt/HiCIBaS/config/network.conf
	cp ./python/ui_guiding.py /opt/HiCIBaS/scripts
	cp ./etc/dummy.fits /opt/HiCIBaS/etc
	cd ./python && make install
	cd ./nutec-server/etc && cp *.py /opt/HiCIBaS/python
	cd ./rm8-server/etc && cp rm8.py /opt/HiCIBaS/python	
