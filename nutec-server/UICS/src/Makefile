#CMD = $(shell find ../ -name cmd.conf -exec readlink -f {} \;)
#STATE = $(shell find ../ -name state.conf -exec readlink -f {} \;)
BASE = /opt/nutec
checkenv:
ifndef CONFPATH
cfgpath := /opt/nutec/config
else
cfgpath := $(CONFPATH)
endif

all:
	g++ -DCONFPATH='"$(cfgpath)"' -g -std=c++11 -I. -c *.cpp
	ar rcs libuics.a *.o
clean:
	rm -f *.o
	rm -f libuics.a
install:
	cp libuics.a $(BASE)/lib
	cp uics.h $(BASE)/lib
	cp ../conf/cmd.cfg $(cfgpath)/cmd.conf
	cp ../conf/state.cfg $(cfgpath)/state.conf
	cp *.h $(BASE)/include
