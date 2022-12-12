#include <iostream>
#include "insthandle.h"
#include "config_file.h"
#include "uics.h"
#include "python_cmd.h"

using namespace std;
#define READY "ready"
#ifndef INITPATH
#define INITPATH "/opt/HxRG-ENG/config"
#endif

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
    
    //:::::::::::::::::::::::::
    //::: Python script Set :::
    //:::::::::::::::::::::::::
    
    py_manager *Py = new py_manager("/home/espressjo/.miniconda3/bin/python");
    Py->add_python_script("/home/espressjo/script1.py");
    Py->add_python_script("/home/espressjo/script2.py");
    Py->add_python_script("/home/espressjo/script3.py");
    handle.py = Py;
    
    //:::::::::::::::::::::::::
    //::: Start the threads :::
    //:::::::::::::::::::::::::
    
    std::thread t_msg(&msgHandler::run,&msgH);
    t_msg.detach();
    
    sleep(1);//make sure all the thread are started!!
    
    //start the main loop
	sHandler.run();

    return 0;
}



