BASE = /opt/rm8

CONFPATH := /opt/rm8/config
INITPATH := /opt/rm8/config


folders:
	mkdir -p /opt/rm8
	mkdir -p /opt/rm8/etc
	mkdir -p /opt/rm8/bin
	mkdir -p /opt/rm8/config
	mkdir -p /opt/rm8/include
	mkdir -p /opt/rm8/python
	mkdir -p /opt/rm8/lib
	mkdir -p /opt/rm8/log

all: folders
	cd ./UICS/src && make all install
	cd ./src && make all 
clean:
	cd ./UICS/src && make clean
	cd ./src && make clean

install:
	cp ./config/cmd.cfg $(BASE)/config/cmd.conf
	cp ./config/state.cfg $(BASE)/config/state.conf
	cp ./config/rm8.cfg $(BASE)/config/rm8.conf
	cd ./src && make install
	cp ./config/rm8.service /etc/systemd/system/rm8.service
	cp ./etc/*.py /opt/rm8/python
