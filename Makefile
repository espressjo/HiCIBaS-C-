all:
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
	cp ./nutec-server && cp *.py /opt/HiCIBaS/python
	cp ./rm8-nutec && cp rm8.py /opt/HiCIBaS/python	
