folder:
	mkdir -p /opt/HiCIBaS/python
	mkdir -p /opt/HiCIBaS/scripts
RM:
	cd ./RM8_driver/src && make clean all install
LJM:
	./install.sh
.PHONY: LJM
all:folder LJM
	cp *.py /opt/HiCIBaS/python
	cp ./script/*py /opt/HiCIBaS/scripts
	@echo ""
	@echo "::::::::::::::::::::::::::::::::::::::::::::::::::::::"
	@echo ":::   Update PYTHONPATH with /opt/HiCIBaS/python   :::"
	@echo "::::::::::::::::::::::::::::::::::::::::::::::::::::::"
	@echo ""
install:
	#@cd ./RM8_driver/src && make install
	#@cp ./python/*.py /opt/HiCIBaS/python
	#@cp ./python/scripts/*.py /opt/HiCIBaS/scripts
	#cp ./script/*py /opt/HiCIBaS/scripts	
