#include <iostream>
#include <string>
#include "insthandle.h"
#include "uics.h"
#include "nutec_config.h"
#include "nutec.h"

using namespace std;


#ifndef INITPATH
#define INITPATH "/opt/nutec/config"
#endif

Log nuteclog;


int main(int argc, char *argv[])
{
	instHandle handle;
	std::string lpath="";
	int baudrate=9600,serial_port=0;
	msgHandler msgH;
	
	//get the port number and log path

	if (ui_get_string(string(INITPATH)+"/nutec.conf","LOG",&lpath)!=0)
	{return -1;}
	if (ui_get_int(string(INITPATH)+"/nutec.conf","SERIAL",&serial_port)!=0)
	{return -1;}
	if (ui_get_int(string(INITPATH)+"/nutec.conf","BAUDRATE",&baudrate)!=0)
	{return -1;}
	
	nuteclog.setPath(lpath);
    nuteclog.writeto("STARTUP");
	
	//:::::::::::::::::::::::::::::::::::::::
	//:::   try to open the serial port   :::
	//:::::::::::::::::::::::::::::::::::::::
	handle.sport = serial();
	handle.sport.ouvrirport(serial_port,baudrate);
	
	if (handle.sport.status!=OPEN)
	{
		nuteclog.writetoVerbose("Unable to open serial port /dev/ttyS"+std::to_string(serial_port));
	}
	else{
		
		setup(&handle);
	}
	
	//::::::::::::::::::::::::::::::::::::
	//:::   start the message handler  :::
	//::::::::::::::::::::::::::::::::::::
	std::thread t_msg(&msgHandler::run,&msgH);
    t_msg.detach();
    sleep(1);
	
	state_handler sHandler(&handle);
	
	sHandler.s_config->add_callback("ioserial",serial_cmd_io);
	sHandler.s_config->add_callback("get_pos",read_position);
	sHandler.s_config->add_callback("move_abs",move_abs);
	sHandler.s_config->add_callback("abort",abort);
	sHandler.s_config->add_callback("move",move);
	sHandler.s_config->add_callback("set_speed",set_speed);
	sHandler.s_config->add_callback("get_speed",get_speed);
	sHandler.s_config->add_callback("isMoving",isMoving);
	
	sleep(1);
    sHandler.run();
	handle.sport.fermerport();
	return 0;
}

