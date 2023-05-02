#include "getstatus.h"

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
			sprintf(buff,"%u",handle->tcs->tcs_tel->limswitch);
			c->respond(std::string(buff));
			continue;
		}
		if ((*c)["-tcs"].compare("")!=0){
			char buff[20];
			memset(buff,0,20);
			sprintf(buff,"%f;%f",handle->tcs->tcs_tel->alt,handle->tcs->tcs_tel->az);
			c->respond(std::string(buff));
			continue;
		}
		if ((*c)["-devices"].compare("")!=0){
			char buff[10];
			memset(buff,0,10);
			sprintf(buff,"%u",handle->tcs->tcs_tel->devices);
			c->respond(std::string(buff));
			continue;
		}
		if ((*c)["-telemetry"].compare("")!=0){
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
			//:::   encode to send to client   :::
			//::::::::::::::::::::::::::::::::::::
			std::string e_tlm = encode_telemetry(tlm);
			c->respond(e_tlm);
			continue;
		}
		if ((*c)["-ptelemetry"].compare("")!=0){
			/*
			 * Print Telemetry (for troubleshouting purposes)
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
			
			//:::::::::::::::::::::::::
			//:::  send to client   :::
			//:::::::::::::::::::::::::
			std::string e_tlm = "";
			e_tlm+="\nAltitude: "+std::to_string(tlm.alt)+"\n";
			e_tlm+="Azimuth: "+std::to_string(tlm.az)+"\n";
			e_tlm+="DEC: "+std::to_string(tlm.DEC)+"\n";
			e_tlm+="RA: "+std::to_string(tlm.RA)+"\n";
			e_tlm+="Devices: "+std::to_string(tlm.devices)+"\n";
			e_tlm+="Lim. Switch: "+std::to_string(tlm.limswitch)+"\n";
			e_tlm+="Moteur 1: "+std::to_string(tlm.moteur_1)+"\n";
			e_tlm+="Moteur 2: "+std::to_string(tlm.moteur_2)+"\n";
			e_tlm+="Running Scripts: "+std::to_string(tlm.r_scripts)+"\n";
			e_tlm+="Stopped Scripts: "+std::to_string(tlm.s_scripts)+"\n";

			e_tlm+="H1: "+std::to_string(tlm.H1)+"\n";
			e_tlm+="H2: "+std::to_string(tlm.H2)+"\n";
			e_tlm+="H3: "+std::to_string(tlm.H3)+"\n";
			e_tlm+="H4: "+std::to_string(tlm.H4)+"\n";
			e_tlm+="H5: "+std::to_string(tlm.H5)+"\n";
			e_tlm+="H6: "+std::to_string(tlm.H6)+"\n";
			e_tlm+="T1: "+std::to_string(tlm.T1)+"\n";
			e_tlm+="T2: "+std::to_string(tlm.T2)+"\n";
			e_tlm+="T3: "+std::to_string(tlm.T3)+"\n";
			e_tlm+="T4: "+std::to_string(tlm.T4)+"\n";
			e_tlm+="T5: "+std::to_string(tlm.T5)+"\n";
			e_tlm+="T6: "+std::to_string(tlm.T6)+"\n";
			c->respond(e_tlm);
			continue;
		}
		
		
		msg="";
		msg+="limswitch: "+std::to_string(static_cast<int>(handle->tcs->tcs_tel->limswitch))+"\n";	
		msg+=std::string("Limit switch ")+((handle->lim_online) ? "online" : "offline")+"\n";
		msg+="alt: "+std::to_string(handle->tcs->tcs_tel->alt)+"\n";
		msg+="az: "+std::to_string(handle->tcs->tcs_tel->az)+"\n";
		msg+="Devices: "+std::to_string(handle->tcs->tcs_tel->devices)+"\n";
		c->respond(msg);
       }
}