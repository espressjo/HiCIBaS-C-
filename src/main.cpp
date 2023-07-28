#include <iostream>
#include "insthandle.h"
#include "config_file.h"
#include "uics.h"
#include "python_cmd.h"
#include "shared_tcs.h"
#include "ljack.h"
#include "telemetry.h"
#include "py_scripts_config.h"
#include "getstatus.h"
#include "shared_tcs_thread.h"
#include "motor_status.h"
using namespace std;

#define READY "ready"

#ifndef INITPATH
#define INITPATH "/opt/HiCIBaS/config"
#endif


void pTCS(instHandle *handle,cmd *cc)
{
	std::cout<<"Moteur 1: "<<handle->tcs->tcs_tel->moteur_1<<std::endl;
	return;
}


Log HiLog;

int main(int argc, char *argv[])
{
   
    //start the log
    instHandle handle;
    init_handler("/opt/HiCIBaS/config/HiCIBaS.conf",&handle);//init the insthandler and load the config file
    
    //init the log system
    HiLog.setPath(handle.path.log);
    HiLog.writetoVerbose("HiCIBaS STARTUP");//log the startup stamp
    
    //declare some UICS object
    msgHandler msgH; 
	udp_msgHandler udp_msgH;//for udp protocol
	state_handler sHandler(&handle);
    
    
    //:::::::::::::::::::::::::
    //::: Function Callback :::
    //:::::::::::::::::::::::::
    
	sHandler.s_config->add_callback("python",python_cmd);
	sHandler.s_config->add_callback("pTCS",pTCS);

	//::::::::::::::::::::::::::::::::
	//::: Create the shared memory :::
	//::::::::::::::::::::::::::::::::
	shared_tcs *tcs = new shared_tcs(1);
	handle.tcs = tcs;
	

    
	//::::::::::::::::::::::::::::::::::::::
    //:::   Set the python interpreter   :::
	//::::::::::::::::::::::::::::::::::::::
	
	//py_manager *Py = new py_manager("/home/hicibas-clone/anaconda3/bin/python");
	std::string interpreter = "";
	if (getInterpreter("/opt/HiCIBaS/config/scripts.txt",&interpreter)!=0)
	{
		std::cout<<"Unable to find the script config file!"<<std::endl;
	return 0;	
	}
	py_manager *Py = new py_manager(interpreter);
	
	//:::::::::::::::::::::::::
    //::: Python script Set :::
    //:::::::::::::::::::::::::
	std::vector<std::string> myScripts = get_scripts("/opt/HiCIBaS/config/scripts.txt");
	for (auto s : myScripts)
	{
		
		Py->add_python_script(s);
	}

    handle.py = Py;
    
    
	//:::::::::::::::::::::::::
    //::: Start the threads :::
    //:::::::::::::::::::::::::
	std::thread t_get_status(&getStatus,&handle);
    t_get_status.detach();
	
	std::thread t_get_shm(&getshm,&handle);
    t_get_shm.detach();
	
	
    std::thread t_msg(&msgHandler::run,&msgH);
    t_msg.detach();//read_limits
	//std::thread t_msg_udp(&udp_msgHandler::run,&udp_msgH);
    //t_msg_udp.detach();//read_limits
	
	std::thread t_motor_status(&motor_status_t,&handle);
    t_motor_status.detach();//read_limits
	
    std::thread t_temp(&read_temps_t,&handle);
    t_temp.detach();//read_limits
    sleep(1);//make sure all the thread are started!!
    
    //start the main loop
	sHandler.run();

    return 0;
}



