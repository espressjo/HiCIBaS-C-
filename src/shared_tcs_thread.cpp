#include "shared_tcs_thread.h"

void getshm(instHandle *handle)
{
	int fd = create_socket(5558);
	cmd *c = new cmd;
	string msg="";
	std::string e_tlm = "";
    while (1) {
		
        c->recvCMD(fd);
		/*
		e_size = islb64EncodeAlloc((const char*)handle->tcs->tcs_tel,sizeof(handle->tcs->tcs_tel),&e_shm);
		if (e_size==0){
			e_shm = nullptr;
			continue;
		}
		msg = std::string(e_shm);
		c->respond(msg);
		e_shm = nullptr;
		*/
		telemetry tlm{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		//:::::::::::::::::::::::::::::::
		//:::   Setup the telemetry   :::
		//:::::::::::::::::::::::::::::::
		tlm.limswitch = handle->tcs->tcs_tel->limswitch;
		tlm.devices = handle->tcs->tcs_tel->devices;
		//check if labjack is ON
		if (handle->lim_online){tlm.devices|=0b10000000;}
		else {tlm.devices&=0b01111111;}
		tlm.alt = handle->tcs->tcs_tel->alt;
		tlm.az = handle->tcs->tcs_tel->az;
		
		//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		//:::   encode the running script and stopped script.   :::
		//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		tlm.r_scripts = encode_scripts(handle->py->whos_running(),handle->py_config_file);
		tlm.s_scripts = encode_scripts(handle->py->whos_finished(),handle->py_config_file);
		
		//::::::::::::::::::::::::::::::::::::
		//:::   encode and send to client   :::
		//::::::::::::::::::::::::::::::::::::
		
		std::string e_tlm = encode_telemetry(tlm);
		c->respond(e_tlm);
		//sndMsg(c->sockfd,e_tlm);
		}//while
}
