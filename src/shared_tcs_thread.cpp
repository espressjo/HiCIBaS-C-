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
		telemetry tlm{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
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
		
		tlm.moteur_1 = handle->tcs->tcs_tel->moteur_1;
		tlm.moteur_2 = handle->tcs->tcs_tel->moteur_2;
		tlm.DEC = handle->tcs->tcs_tel->DEC;
		tlm.RA = handle->tcs->tcs_tel->RA;
		tlm.H1 = handle->tcs->tcs_tel->H1;
		tlm.H2 =handle->tcs->tcs_tel->H2;
		tlm.H3 =handle->tcs->tcs_tel->H3;
		tlm.H4 =handle->tcs->tcs_tel->H4;
		tlm.H5 =handle->tcs->tcs_tel->H5;
		tlm.H6 =handle->tcs->tcs_tel->H6;
		tlm.T1 = handle->tcs->tcs_tel->T1;
		tlm.T2 =handle->tcs->tcs_tel->T2;
		tlm.T3 =handle->tcs->tcs_tel->T3;
		tlm.T4 =handle->tcs->tcs_tel->T4;
		tlm.T5 =handle->tcs->tcs_tel->T5;
		tlm.T6 =handle->tcs->tcs_tel->T6;
		tlm.moteur = handle->tcs->tcs_tel->moteur;
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
