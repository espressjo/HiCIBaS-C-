BASE = /opt/nutec

CONFPATH := /opt/nutec/config
INITPATH := /opt/nutec/config

folders:
	mkdir -p /opt/nutec
	mkdir -p /opt/nutec/python
	mkdir -p /opt/nutec/etc
	mkdir -p /opt/nutec/bin
	mkdir -p /opt/nutec/config
	mkdir -p /opt/nutec/include
	mkdir -p /opt/nutec/lib
	mkdir -p /opt/nutec/log
all: folders
	cd ./UICS/src && make all install
	cp ./src/insthandle.h /opt/nutec/include
	cd ./src && make all
	cp ./etc/*.py /opt/nutec/python
install:
	cd ./src && make install
	#cp ./config/nutec.service /etc/systemd/system/nutec.service
	cp ./config/cmd.cfg $(BASE)/config/cmd.conf
	cp ./config/state.cfg $(BASE)/config/state.conf
	cp ./config/nutec.cfg $(BASE)/config/nutec.conf

clean:
	cd ./src && make clean
