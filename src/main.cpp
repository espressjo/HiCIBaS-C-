#include <iostream>
#include "insthandle.h"
#include "config_file.h"
#include "uics.h"
#include "python_cmd.h"
#include "shared_tcs.h"

using namespace std;

#define READY "ready"

#ifndef INITPATH
#define INITPATH "/opt/HiCIBaS/config"
#endif
/*
 void getUniqueId(instHandle *handle,cmd *cc)//getUniqueId

{
    int fd = create_socket(6041);
    cmd *c = new cmd;
    while (1) {
        c->recvCMD(fd);
        sndMsg(c->sockfd,std::to_string(handle->head.ID));
       }
}
*/

void getStatus(instHandle *handle)
{
	int fd = create_socket(5557);
	cmd *c = new cmd;
	string msg="";
    while (1) {
        c->recvCMD(fd);
		
		msg="";
		msg+="ra: "+std::to_string(handle->tcs->shmp->ra)+"\n";
		msg+="dec: "+std::to_string(handle->tcs->shmp->dec)+"\n";
		sndMsg(c->sockfd,msg);
       }
	
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
	state_handler sHandler(&handle);
    
    
    //:::::::::::::::::::::::::
    //::: Function Callback :::
    //:::::::::::::::::::::::::
    
	sHandler.s_config->add_callback("python",python_cmd);
	//::::::::::::::::::::::::::::::::
	//::: Create the shared memory :::
	//::::::::::::::::::::::::::::::::
	shared_tcs *tcs = new shared_tcs(1);
	handle.tcs = tcs;
    //:::::::::::::::::::::::::
    //::: Python script Set :::
    //:::::::::::::::::::::::::
    
	//py_manager *Py = new py_manager("/home/hicibas-clone/anaconda3/bin/python");
	py_manager *Py = new py_manager("/home/espressjo/miniconda3/bin/python");

    Py->add_python_script("/home/hicibas-clone/Desktop/Hicibas_motors_fall_2022-master/ids_cam.py");
    Py->add_python_script("/home/hicibas-clone/Desktop/Hicibas_motors_fall_2022-master/pre_launch.py");
	Py->add_python_script("/home/espressjo/test_argument.py");
    handle.py = Py;
    
    //:::::::::::::::::::::::::
    //::: Start the threads :::
    //:::::::::::::::::::::::::
    
	std::thread t_get_status(&getStatus,&handle);
    t_get_status.detach();
	
	
    std::thread t_msg(&msgHandler::run,&msgH);
    t_msg.detach();
    
    sleep(1);//make sure all the thread are started!!
    
    //start the main loop
	sHandler.run();

    return 0;
}



