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
	
	//get the port number and log path

	if (ui_get_string(string(INITPATH)+"/rm8.conf","LOG",&lpath)!=0)
	{return -1;}
	
	
	rm8log.setPath(lpath);
    rm8log.writeto("STARTUP");
	
	if (connect(&handle)!=0)
	{
		std::cout<<"Unable to connect to the controller"<<std::endl;
	}
	//::::::::::::::::::::::::::::::::::::
	//:::   start the message handler  :::
	//::::::::::::::::::::::::::::::::::::
	std::thread t_msg(&msgHandler::run,&msgH);
    t_msg.detach();
    sleep(1);
	
	std::thread t_status(&status_t,&handle);
    t_status.detach();
    sleep(1);
	
	
	state_handler sHandler(&handle);
	
	sHandler.s_config->add_callback("ioserial",serialio);
	sHandler.s_config->add_callback("p_status",p_status);
	
	
	
	sleep(1);
    sHandler.run();
	
	return 0;
}

