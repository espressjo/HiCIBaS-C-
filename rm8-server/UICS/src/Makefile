BASE = /opt/rm8

checkenv:
ifndef CONFPATH
cfgpath := /opt/rm8/config
else
cfgpath := $(CONFPATH)
endif

INCL = -I./
CFLAGS = -g \
	 -std=c++11
OBJS:  b64.o \
       fhtable.o \
       inst_log.o \
       inst_time.o \
       msghandler.o \
       state_handler.o \
       udp_msghandler.o \
       uics_join.o \
       uics_cmds.o \
       uics_sndmsg.o \
       uics_state.o \
       uics_sockets.o
	
%.o: %.cpp
	g++ -DCONFPATH='"$(cfgpath)"' $(CFLAGS) $(INCL) -c $< -o $@

all: OBJS
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
