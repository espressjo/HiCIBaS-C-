BASE = /opt/HiCIBaS
CONFPATH := /opt/HiCIBaS/config
folder:
	mkdir -p $(BASE)/bin
	mkdir -p $(BASE)/config
	mkdir -p $(BASE)/etc
	mkdir -p $(BASE)/include
	mkdir -p $(BASE)/lib
	mkdir -p $(BASE)/python
	mkdir -p $(BASE)/scripts

all: folder
	cd ./src && make cfgpath=$(CONFPATH) basepath=$(BASE) all
	@#cd ./HiCIBaS-Script && make all
	@#cd ./HiCIBaS-telemetry && make all
	@#cd ./HiCIBaS-Motors && make all
clean:
	cd ./UICS && make clean
	@#cd ./socket_gui && make clean
	cd ./src && make clean
	@#cd ./HiCIBaS-Script && make all
	@#cd ./HiCIBaS-telemetry && make all
	@#cd ./HiCIBaS-Motors && make all
install:
	cd ./src && make install
	cp ./python/*.py /opt/HiCIBaS/python
	cp ./python/script/*.py /opt/HiCIBaS/scripts
	@#cd ./HiCIBaS-Script && make install
	@#cd ./HiCIBaS-telemetry && make install
	@#cd ./HiCIBaS-Motors && make install
	cp ./config/network.cfg /opt/HiCIBaS/config/network.conf
	cp ./config/HiCIBaS.cfg /opt/HiCIBaS/config/HiCIBaS.conf
	cp ./config/scripts.cfg $(BASE)/config/scripts.txt
	cp ./config/fine.cfg $(BASE)/config/fine.cam
	cp ./config/coarse.cfg $(BASE)/config/coarse.cam
	@#cp ./python/ui_guiding.py /opt/HiCIBaS/scripts
	@#cp ./etc/dummy.fits /opt/HiCIBaS/etc
	@#cd ./python && make install
	@#cd ./nutec-server/etc && cp *.py /opt/HiCIBaS/python
	@#cd ./rm8-server/etc && cp rm8.py /opt/HiCIBaS/python	
