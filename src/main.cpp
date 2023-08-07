#include <iostream>
#include "insthandle.h"
#include "config_file.h"
#include "uics.h"
#include <stdlib.h>
#include "python_cmd.h"
#include "shared_tcs.h"
#include "ljack.h"
#include "telemetry.h"
#include "py_scripts_config.h"
#include "getstatus.h"
#include "shared_tcs_thread.h"
#include "motor_status.h"
#include "heater_loop.h"

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
void close_server()
/*
 * Close the main server. Usefull if the program is 
 * Daemonized.
 */ 
{

	int fd = create_socket(1555);
	cmd *cc = new cmd;

    while (1) {
		
        cc->recvCMD(fd);  
        if ( (*cc)["-server"].compare("")!=0 ){
        exit(0);}
    }  
}

Log HiLog;

int main(int argc, char *argv[])
{
   
    //start the log
    instHandle handle;
    init_handler("/opt/HiCIBaS/config/HiCIBaS.conf",&handle);//init the insthandler and load the config file
    handle.heater.ttm_setpoint = -999;
    handle.heater.nutec_setpoint = -999;
    handle.heater.ccam_setpoint = -999;
    //init the log system
    HiLog.setPath(handle.path.log);
    HiLog.writetoVerbose("HiCIBaS STARTUP");//log the startup stamp
    
    //declare some UICS object
    msgHandler msgH; 
	udp_msgHandler udp_msgH;//for udp protocol
	state_handler sHandler(&handle);
    
    if (ui_get_int("/opt/HiCIBaS/config/HiCIBaS.conf","TTM_SP",&handle.heater.ttm_setpoint)!=0)
    {
        std::cout<<"Unable to set heater setpoint [TTM]"<<std::endl;
    }
    if (ui_get_int("/opt/HiCIBaS/config/HiCIBaS.conf","CCAM_SP",&handle.heater.ccam_setpoint)!=0)
    {
        std::cout<<"Unable to set heater setpoint [Coarse Cam]"<<std::endl;
    }
    if (ui_get_int("/opt/HiCIBaS/config/HiCIBaS.conf","NUTEC_SP",&handle.heater.nutec_setpoint)!=0)
    {
        std::cout<<"Unable to set heater setpoint [nutec]"<<std::endl;
    }
    //:::::::::::::::::::::::::
    //::: Function Callback :::
    //:::::::::::::::::::::::::
    
	sHandler.s_config->add_callback("python",python_cmd);
	sHandler.s_config->add_callback("pTCS",pTCS);
    //heater_cmd
    sHandler.s_config->add_callback("heater",heater_cmd);

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
	
	//close_server
    std::thread t_close(&close_server);
    t_close.detach();//read_limits
    std::thread t_msg(&msgHandler::run,&msgH);
    t_msg.detach();//read_limits
	std::thread t_msg_udp(&udp_msgHandler::run,&udp_msgH);
    t_msg_udp.detach();
	
	std::thread t_motor_status(&motor_status_t,&handle);
    t_motor_status.detach();//read_limits
	
    std::thread t_temp(&read_temps_t,&handle);
    t_temp.detach();//read_limits
    sleep(1);//make sure all the thread are started!!
    
    //start the main loop
	sHandler.run();

    return 0;
}



