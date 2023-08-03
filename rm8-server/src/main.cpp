#include <iostream>
#include <string>
#include "insthandle.h"
#include "uics.h"
#include "rm8_config.h"
#include "rm8.h"
using namespace std;


#ifndef INITPATH
#define INITPATH "/opt/rm8/config"
#endif

Log rm8log;


int main(int argc, char *argv[])
{
	instHandle handle;
	std::string lpath="";
	msgHandler msgH;
	udp_msgHandler udp_msgH;//for udp protocol
	//set the handle default values.
	handle.position = 0;
	handle.drive_enabled = false;
	handle.lim_p = false;
	handle.lim_n = false;
	handle.lim_home = false;
	handle.active = false;
	handle.moving = false;
	handle.mst = 0;
	
	
	//get the port number and log path

	if (ui_get_string(string(INITPATH)+"/rm8.conf","LOG",&lpath)!=0)
	{return -1;}
	
	
	rm8log.setPath(lpath);
    rm8log.writeto("STARTUP");
	
	//set the handle->active flag to false
	
	if (connect(&handle)!=0)
	{
		std::cout<<"Unable to connect to the controller"<<std::endl;
	}
	//::::::::::::::::::::::::::::::::::::
	//:::   start the message handler  :::
	//::::::::::::::::::::::::::::::::::::
	std::thread t_msg(&msgHandler::run,&msgH);
    t_msg.detach();
    std::thread t_msg_udp(&udp_msgHandler::run,&udp_msgH);
    t_msg_udp.detach();
    sleep(1);
	
	
	std::thread t_status_motor(&motor_status_t,&handle);
    t_status_motor.detach();
    sleep(1);
	
	std::thread t_status(&status_t,&handle);
    t_status.detach();
    sleep(1);
	
	state_handler sHandler(&handle);
	
	sHandler.s_config->add_callback("ioserial",serialio);
	sHandler.s_config->add_callback("usb",usb);
	sHandler.s_config->add_callback("abort",abort);
	sHandler.s_config->add_callback("get_pos",read_position);
	sHandler.s_config->add_callback("move",loop);
	sHandler.s_config->add_callback("move_no_return",loop_no_return);
	sHandler.s_config->add_callback("get_low_speed",get_low_speed);
	sHandler.s_config->add_callback("get_high_speed",get_high_speed);
	sHandler.s_config->add_callback("set_low_speed",set_low_speed);
	sHandler.s_config->add_callback("set_high_speed",set_high_speed);
	sHandler.s_config->add_callback("set_acceleration",set_acceleration);
	sHandler.s_config->add_callback("get_acceleration",get_acceleration);
	sHandler.s_config->add_callback("clear_limit_error",clear_limit);
	sHandler.s_config->add_callback("isMoving",isMoving);
	sHandler.s_config->add_callback("enable_drive",enable_drive);
	sHandler.s_config->add_callback("disable_drive",disable_drive);
	//set_acceleration
	sleep(1);
    sHandler.run();
	
	return 0;
}


