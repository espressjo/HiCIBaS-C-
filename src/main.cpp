#include <iostream>
#include "insthandle.h"
#include "config_file.h"
#include "uics.h"
#include "python_cmd.h"
#include "shared_tcs.h"
#include "lim_switch.h"
#include "telemetry.h"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>

using namespace std;

#define READY "ready"

#ifndef INITPATH
#define INITPATH "/opt/HiCIBaS/config"
#endif

void read_limits(instHandle *handle)
{
	
	lim_switch mylim("T4","Ethernet",440010529);	
	if (!mylim.connected)
	{
		handle->lim_online=false;
		return ;
	}
	else{
		handle->lim_online=true;
		}
	int err=0;
	while(1)
	{
		err = mylim.read_lim_switch();
		if (err!=0){break;}
		handle->tcs->shmp->limits = mylim.compress();
		std::cout<<"Compress: "<<std::to_string(static_cast<int>(handle->tcs->shmp->limits))<<std::endl;
		std::this_thread::sleep_for (std::chrono::seconds(1));	
	}
	handle->lim_online=false;
	return ;
}




void getStatus(instHandle *handle)
{
	int fd = create_socket(5557);
	cmd *c = new cmd;
	string msg="";
    while (1) {
        c->recvCMD(fd);
		//if we only want the lim. switch status
		if ((*c)["-lim"].compare("")!=0){
			char buff[12];
			memset(buff,0,12);
			sprintf(buff,"%u",handle->tcs->shmp->limits);
			c->respond(std::string(buff));
			continue;
		}
		if ((*c)["-tcs"].compare("")!=0){
			char buff[20];
			memset(buff,0,20);
			sprintf(buff,"%f;%f",handle->tcs->shmp->alt,handle->tcs->shmp->az);
			c->respond(std::string(buff));
			continue;
		}
		if ((*c)["-devices"].compare("")!=0){
			char buff[10];
			memset(buff,0,10);
			sprintf(buff,"%u",handle->tcs->shmp->devices);
			c->respond(std::string(buff));
			continue;
		}
		if ((*c)["-telemetry"].compare("")!=0){
			telemetry tlm{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
			std::string e_tlm = encode_telemetry(tlm);
			c->respond(e_tlm);
			continue;
		}
		
		
		
		msg="";
		msg+="limits: "+std::to_string(static_cast<int>(handle->tcs->shmp->limits))+"\n";	
		msg+=std::string("Limit switch ")+((handle->lim_online) ? "online" : "offline")+"\n";
		msg+="alt: "+std::to_string(handle->tcs->shmp->alt)+"\n";
		msg+="az: "+std::to_string(handle->tcs->shmp->az)+"\n";
		msg+="Devices: "+std::to_string(handle->tcs->shmp->devices)+"\n";
		c->respond(msg);
       }
}
void getshm(instHandle *handle)
{
	int fd = create_socket(5558);
	cmd *c = new cmd;
	string msg="";
	char *e_shm=nullptr;
	size_t e_size=0;
    while (1) {
        c->recvCMD(fd);
		e_size = islb64EncodeAlloc((const char*)handle->tcs->shmp,sizeof(handle->tcs->shmp),&e_shm);
		if (e_size==0){
			e_shm = nullptr;
			continue;
		}
		msg = std::string(e_shm);
//		handle->tcs->shmp
		c->respond(msg);
		e_shm = nullptr;
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
	udp_msgHandler udp_msgH;//for udp protocol
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
	handle.lim_online = false;
    //:::::::::::::::::::::::::
    //::: Python script Set :::
    //:::::::::::::::::::::::::
    
	//py_manager *Py = new py_manager("/home/hicibas-clone/anaconda3/bin/python");
	py_manager *Py = new py_manager("/home/espressjo/miniconda3/bin/python");

    //Py->add_python_script("/home/hicibas-clone/Desktop/Hicibas_motors_fall_2022-master/ids_cam.py");
    //Py->add_python_script("/home/hicibas-clone/Desktop/Hicibas_motors_fall_2022-master/pre_launch.py");
    Py->add_python_script("/home/espressjo/test2.py");
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
	std::thread t_msg_udp(&udp_msgHandler::run,&udp_msgH);
    t_msg_udp.detach();//read_limits
	
	
	
    std::thread t_lim(&read_limits,&handle);
    t_lim.detach();//read_limits
    sleep(1);//make sure all the thread are started!!
    
    //start the main loop
	sHandler.run();

    return 0;
}



