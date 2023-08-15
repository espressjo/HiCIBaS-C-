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
all-gui: folder
	cd ./HiCIBaS-Guiding && make all
	cd ./HiCIBaS-Motors && make all
	cd ./HiCIBaS-telemetry && make all
	cd ./HiCIBaS-Script && make all
uics: 
	cd ./UICS && make cfgpath=$(CONFPATH) clean all install
all: folder
	cd ./src && make cfgpath=$(CONFPATH) basepath=$(BASE) all
	cd ./nutec-server && make all
	cd ./rm8-server && make all
clean:
	cd ./UICS && make clean
	cd ./nutec-server && make clean
	cd ./rm8-server && make clean
	@cd ./socket_gui && rm -f *.o
	@cd ./py_manager && rm -f *.o
	@cd ./py_proc && rm -f *.o
	cd ./src && make clean
	@cd ./HiCIBaS-Script && make clean
	@cd ./HiCIBaS-telemetry && make clean
	@cd ./HiCIBaS-Motors && make clean
install-gui:
	@cd ./HiCIBaS-Script && make install
	@cd ./HiCIBaS-telemetry && make install
	@cd ./HiCIBaS-Motors && make install
	@cd ./HiCIBaS-Guiding && make install
install:
	cd ./src && make install
	cd ./rm8-server && make install
	cd ./nutec-server && make install
	cp ./python/*.py /opt/HiCIBaS/python
	cp ./python/script/*.py /opt/HiCIBaS/scripts
	cp ./config/network.cfg /opt/HiCIBaS/config/network.conf
	cp ./config/HiCIBaS.cfg /opt/HiCIBaS/config/HiCIBaS.conf
	cp ./config/scripts.cfg $(BASE)/config/scripts.txt
	cp ./config/fine.cfg $(BASE)/config/fine.cam
	cp ./config/coarse.cfg $(BASE)/config/coarse.cam
install-service:
	cp ./config/hicibas.service /etc/systemd/system/hicibas.service
	cp ./nutec-server/config/nutec.service /etc/systemd/system/nutec.service
	cp ./rm8-server/config/rm8.service /etc/systemd/system/rm8.service
