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
