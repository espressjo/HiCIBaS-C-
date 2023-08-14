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
gui: folder
	cd ./HiCIBaS-Guiding && make all
	cd ./HiCIBaS-Motors && make all
	cd ./HiCIBaS-telemetry && make all
	cd ./HiCIBaS-Script && make all
all: folder
	cd ./src && make cfgpath=$(CONFPATH) basepath=$(BASE) all
	cd ./nutec-server && make all
	cd ./rm8-server && make all
	@#cd ./HiCIBaS-Script && make all
	@#cd ./HiCIBaS-telemetry && make all
	@#cd ./HiCIBaS-Motors && make all
clean:
	cd ./UICS && make clean
	cd ./nutec-server && make clean
	cd ./rm8-server && make clean
	@#cd ./socket_gui && make clean
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
	cp ./config/hicibas.service /etc/systemd/system/hicibas.service
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
