#include "motor_status.h"

void delay(int ms)
/*!
  *\brief Delay in ms
  * Create a delay. ms is delay time in millisecond
  */
{
   usleep(ms*1000);
}

void motor_status_t(instHandle *handle)
{
	
while(1){
	
	delay(1000);
	std::string resp_nutec="",resp_rm8="";
	//int snd_cmd(std::string cmd,std::string *value_returned,int port,int udp_port,std::string host,bool tcpip=true,int timeout=2);
	nutec_telemetry tlm_nu;
	rm8_telemetry tlm_rm8;
	int ret_nutec = snd_cmd("status -get",&resp_nutec,7555,7655,"localhost",true,1);
	int ret_rm8 = snd_cmd("status -get",&resp_rm8,7565,7665,"localhost",true,1);
	if (ret_nutec==OK)
	{
		
		//if successfully decoded
		if (decode_nutec(resp_nutec,&tlm_nu)==0){
			if (tlm_nu.active){
				handle->tcs->tcs_tel->devices = (handle->tcs->tcs_tel->devices | 32);}
				else {handle->tcs->tcs_tel->devices = (handle->tcs->tcs_tel->devices & 223);}
				
				if (tlm_nu.enabled){handle->tcs->tcs_tel->moteur = handle->tcs->tcs_tel->moteur | 4;}
				else {handle->tcs->tcs_tel->moteur = handle->tcs->tcs_tel->moteur & 251;}
				
				if (tlm_nu.moving){handle->tcs->tcs_tel->moteur = handle->tcs->tcs_tel->moteur | 1;}
				else {handle->tcs->tcs_tel->moteur = handle->tcs->tcs_tel->moteur & 254;}
				handle->tcs->tcs_tel->moteur_1 = static_cast<int32_t>(tlm_nu.position);
				if (tlm_nu.lim_p){handle->tcs->tcs_tel->limswitch = handle->tcs->tcs_tel->limswitch | 1;}
				else{handle->tcs->tcs_tel->limswitch = handle->tcs->tcs_tel->limswitch & 254;}
				if (tlm_nu.lim_n){handle->tcs->tcs_tel->limswitch = handle->tcs->tcs_tel->limswitch | 2;}
				else{handle->tcs->tcs_tel->limswitch = handle->tcs->tcs_tel->limswitch & 253;}
		}
	}
	else{
		handle->tcs->tcs_tel->devices = (handle->tcs->tcs_tel->devices & 223);
	
	}
	if (ret_rm8==OK)
	{
		
		//if successfully decoded
		if (decode_rm8(resp_rm8,&tlm_rm8)==0){
			
			//active
			if (tlm_rm8.active){handle->tcs->tcs_tel->devices = (handle->tcs->tcs_tel->devices | 64);}
			else {handle->tcs->tcs_tel->devices = (handle->tcs->tcs_tel->devices & 191);}
			//enabled
			if (tlm_rm8.drive_enabled){handle->tcs->tcs_tel->moteur = handle->tcs->tcs_tel->moteur | 8;}
			else {handle->tcs->tcs_tel->moteur = handle->tcs->tcs_tel->moteur & 247;}
			//moving
			if (tlm_rm8.moving){handle->tcs->tcs_tel->moteur = handle->tcs->tcs_tel->moteur | 4;}
			else {handle->tcs->tcs_tel->moteur = handle->tcs->tcs_tel->moteur & 251;}
			//encoder
			handle->tcs->tcs_tel->moteur_2 = static_cast<int32_t>(tlm_rm8.position);
			//lim +
			if (tlm_rm8.lim_p){handle->tcs->tcs_tel->limswitch = handle->tcs->tcs_tel->limswitch | 4;}
			else{handle->tcs->tcs_tel->limswitch = handle->tcs->tcs_tel->limswitch & 251;}
			//lim -
			if (tlm_rm8.lim_n){handle->tcs->tcs_tel->limswitch = handle->tcs->tcs_tel->limswitch | 8;}
			else{handle->tcs->tcs_tel->limswitch = handle->tcs->tcs_tel->limswitch & 247;}
			//lim home
			if (tlm_rm8.lim_home){handle->tcs->tcs_tel->limswitch = handle->tcs->tcs_tel->limswitch | 32;}
			else{handle->tcs->tcs_tel->limswitch = handle->tcs->tcs_tel->limswitch & 223;}
		}
	}
	else{
		handle->tcs->tcs_tel->devices = (handle->tcs->tcs_tel->devices & 191);
	
	}
	
	
	}//while	
	
}

int decode_nutec(std::string recv,nutec_telemetry *tlm)
{
	int ret=0;
	nutec_telemetry *tlm_dummy;
	size_t length=0,outlength=0;
	std::string buff="";
	bool T=false;
	char *decoded=nullptr;
	
	for (auto &c:recv){
			if (c==' '){break;}
			buff+=c;
	}
	T=true;
	for (auto &c:buff){
		T = T && isdigit(c);
	}
	if (!T){
		return -1;
	}
	length = std::atoi(buff.c_str());
	recv = recv.substr(buff.length()+1,recv.length());
	
	if (islb64DecodeAlloc(recv.c_str(),length,&decoded,&outlength)!=0)
	{
		return -1;
	}
	tlm_dummy = (nutec_telemetry*)decoded;
	
	tlm->position = tlm_dummy->position;
	tlm->enabled = tlm_dummy->enabled ;
	tlm->active = tlm_dummy->active;
	tlm->moving = tlm_dummy->moving;
	tlm->lim_p = tlm_dummy->lim_p;
	tlm->lim_n = tlm_dummy->lim_n;
	tlm->phase_error = tlm_dummy->phase_error;

	
	return 0;
}

int decode_rm8(std::string recv,rm8_telemetry *tlm)
{
	int ret=0;
	rm8_telemetry *tlm_dummy;
	size_t length=0,outlength=0;
	std::string buff="";
	bool T=false;
	char *decoded=nullptr;
	
	for (auto &c:recv){
			if (c==' '){break;}
			buff+=c;
	}
	T=true;
	for (auto &c:buff){
		T = T && isdigit(c);
	}
	if (!T){
		return -1;
	}
	length = std::atoi(buff.c_str());
	recv = recv.substr(buff.length()+1,recv.length());
	
	if (islb64DecodeAlloc(recv.c_str(),length,&decoded,&outlength)!=0)
	{
		return -1;
	}
	tlm_dummy = (rm8_telemetry*)decoded;
	
	
	tlm->position = tlm_dummy->position;
	tlm->drive_enabled = tlm_dummy->drive_enabled ;
	tlm->active = tlm_dummy->active;
	tlm->moving = tlm_dummy->moving;
	tlm->lim_p = tlm_dummy->lim_p;
	tlm->lim_n = tlm_dummy->lim_n;
	tlm->lim_home = tlm_dummy->lim_home;
	
	return 0;
}