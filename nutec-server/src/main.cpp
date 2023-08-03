#include <iostream>
#include <string>
#include "insthandle.h"
#include "uics.h"
#include "nutec_config.h"
#include "nutec.h"
#include "status_cmd.h"
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
	udp_msgHandler udp_msgH;//for udp protocol
	//get the port number and log path

	if (ui_get_string(string(INITPATH)+"/nutec.conf","LOG",&lpath)!=0)
	{return -1;}
	if (ui_get_int(string(INITPATH)+"/nutec.conf","SERIAL",&serial_port)!=0)
	{return -1;}
	if (ui_get_int(string(INITPATH)+"/nutec.conf","BAUDRATE",&baudrate)!=0)
	{return -1;}
	//set the handle default values
	handle.serial_port = serial_port;
	handle.baudrate = baudrate;
	handle.position = 0;
	handle.xa0 = 0;
	handle.enabled = false;
	handle.active = false;//the serial communication is established, position is updated.
	handle.moving = false;
	handle.lim_p = false;
	handle.lim_n = false;
	handle.phase_error = false;
	
	handle.deduce_moving = false;
	
	nuteclog.setPath(lpath);
    nuteclog.writeto("STARTUP");
	
	//:::::::::::::::::::::::::::::::::::::::
	//:::   try to open the serial port   :::
	//:::::::::::::::::::::::::::::::::::::::
	handle.sport = serial();
	handle.sport.ouvrirport(serial_port,baudrate);
	
	if (handle.sport.status!=OPEN)
	{
		handle.active = false;
		nuteclog.writetoVerbose("Unable to open serial port /dev/ttyS"+std::to_string(serial_port));
	}
	else{
		handle.active = true;
		setup(&handle);
	}
	
	//::::::::::::::::::::::::::::::::::::
	//:::   start the message handler  :::
	//::::::::::::::::::::::::::::::::::::
	std::thread t_msg(&msgHandler::run,&msgH);
    t_msg.detach();
    std::thread t_msg_udp(&udp_msgHandler::run,&udp_msgH);
    t_msg_udp.detach();
    sleep(1);
	
	state_handler sHandler(&handle);
	
	sHandler.s_config->add_callback("ioserial",serial_cmd_io);
	sHandler.s_config->add_callback("serial",serial_cmd);
	sHandler.s_config->add_callback("abort",abort);
	sHandler.s_config->add_callback("get_pos",read_position);
	sHandler.s_config->add_callback("move_abs",move_abs);
	sHandler.s_config->add_callback("move",move);	
	sHandler.s_config->add_callback("move_no_return",move_no_return);
	sHandler.s_config->add_callback("set_speed",set_speed);
	sHandler.s_config->add_callback("get_speed",get_speed);	
	sHandler.s_config->add_callback("isMoving",isMoving);	
	sHandler.s_config->add_callback("enable_drive",enable);
	sHandler.s_config->add_callback("isEnabled",isEnabled);	
	sHandler.s_config->add_callback("disable_drive",disable);
	sHandler.s_config->add_callback("set_acc",set_acceleration);
	sHandler.s_config->add_callback("get_acc",get_acceleration);
    sHandler.s_config->add_callback("set_dec",set_deceleration);
	sHandler.s_config->add_callback("get_dec",get_deceleration);
	sHandler.s_config->add_callback("get_status_code",get_status_code);
	sHandler.s_config->add_callback("deduce_moving",deduce_moving);
	//:::::::::::::::::::::::::::::::::::::
	//:::   Start the position thread   :::
	//:::::::::::::::::::::::::::::::::::::
	std::thread t_position(&position_status_t,&handle);
    t_position.detach();
    sleep(1);
	std::thread t_status(&status_t,&handle);
    t_status.detach();
    sleep(1);
 
	sleep(1);
    sHandler.run();
	
	
	handle.sport.fermerport();
	return 0;
}

