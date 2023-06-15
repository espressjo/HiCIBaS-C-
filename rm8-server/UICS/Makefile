all:
	echo "You may want to export CONFPATH to select where config file will be"
	cd ./src && make clean all install	
clean:
	cd ./src && make clean
	rm -f ./lib/*
	cd ./example/ && make clean
documentation:
	cd ./doc && doxygen Doxyfile
	cd ./doc/html && scp * venus:~/www/doc/uics
install:
	#doxygen doc.conf
	#cd ./doc/latex && make
	#cp ./doc/latex/refman.pdf ./uics.pdf
	cd ./src/ && make install
example:
	cd ./example && make all
