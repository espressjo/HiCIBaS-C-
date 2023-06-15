BASE = /opt/rm8
uics:
	cd ./UICS && make clean all install
all: uics
	echo done
install:
	cp ./config/cmd.cfg $(BASE)/config/cmd.conf
	cp ./config/state.cfg $(BASE)/config/state.conf
	cp ./config/rm8.cfg $(BASE)/config/rm8.conf
