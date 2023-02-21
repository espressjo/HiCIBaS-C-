#include <iostream>
#include "telemetry_window.h"
#include <vector>

TelemetryWindow::TelemetryWindow()

{   //set the IP adress of HiCIBaS main software.
    //This should eventually be done from commandline or from the gui.
    HiCIBaS_ip="localhost";
	
    set_title("HiCIBaS Telemetry");
    set_border_width(5);
    set_default_size(300, 150);
    //add(m_VBox);//add m_VBox inside the window
	m_VBox_main = get_box();
	
	//::::::::::::::::::::::::::
	//:::   Pack the boxes   :::
	//::::::::::::::::::::::::::        
	
	
   
    show_all_children();
	
	
}

TelemetryWindow::~TelemetryWindow()
{

}

int TelemetryWindow::get_info(std::string info,std::string *msg,size_t *msglenght)
{
	if (info.length()<3){return -1;}
	std::string buff="";
	int i=0;
	for (auto &c:info)
	{
		if (c==' '){break;}
		else if (isdigit(c)){
			i+=1;
			buff+=c;
		}
		else {break;}
	}
	if (buff.length()<1){return -1;}
	
	*msglenght = static_cast<size_t>(std::atoi(buff.c_str()));
	*msg = info.substr(i+1,info.length());
	if (msg->length()<1){return -1;}
	return 0;
}

bool TelemetryWindow::HiCIBaS_get_status()
/*
 * Description
 * -----------
 *      Get status which fetch which script is running or stopped.
 *      It also display the connected or disconnect label in the 
 *      status bar. 
 * 
 */ 
{	
	std::string resp="",msg="";
	char *decoded=nullptr;
	size_t outlength=0,length=0;
	int ret=0;
	
	//:::::::::::::::::::::::::::::::::::::
	//:::   fetch limit switch status   :::
	//:::::::::::::::::::::::::::::::::::::
	ret = snd_cmd("getstatus -telemetry",&resp,panel_configuration.tcpip,panel_configuration.socket_timeout);
	if (ret==CONNECTION_P){
		display_disconnected();
		shm_tel->shmp->connected = false;
		return true;
	}
	
	display_connected();
	
	if (islb64DecodeAlloc(msg.c_str(),length,&decoded,&outlength)!=0)
	{
		//may want to display ainfo message
		set_info_message("information received is gibberish");
		shm_tel->shmp->connected = false;
		return true;
	}
	//:::::::::::::::::::::::::::::::::::::::
	//:::   We update the shared memory   :::
	//:::::::::::::::::::::::::::::::::::::::
	telemetry *tlm = (telemetry*)decoded;
	shm_tel->shmp->alt = tlm->alt;
	shm_tel->shmp->az = tlm->az;
	if (tlm->devices & 1 ==1){shm_tel->shmp->cam1 = true;}//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	else {shm_tel->shmp->cam1 = false;}
	if (tlm->devices & 2 ==2){shm_tel->shmp->cam2 = true;}//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	else {shm_tel->shmp->cam2 = false;}
	if (tlm->devices & 4 ==4){shm_tel->shmp->cam3 = true;}//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	else {shm_tel->shmp->cam3 = false;}
	if (tlm->devices & 8 ==8){shm_tel->shmp->TTM = true;}//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	else {shm_tel->shmp->TTM = false;}
	if (tlm->devices & 16 ==16){shm_tel->shmp->source_calibration = true;}//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	else {shm_tel->shmp->source_calibration = false;}
	if (tlm->devices & 32 ==32){shm_tel->shmp->alt_moving = true;}//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	else {shm_tel->shmp->alt_moving = false;}
	if (tlm->devices & 64 ==64){shm_tel->shmp->az_moving = true;}//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	else {shm_tel->shmp->az_moving = false;}
	shm_tel->shmp->moteur_1 = tlm->moteur_1;
	shm_tel->shmp->moteur_2 = tlm->moteur_2;
	shm_tel->shmp->T1 = tlm->T1;
	shm_tel->shmp->T2 = tlm->T2;
	shm_tel->shmp->T3 = tlm->T3;
	shm_tel->shmp->T4 = tlm->T4;
	shm_tel->shmp->T5 = tlm->T5;
	shm_tel->shmp->T6 = tlm->T6;
	shm_tel->shmp->H1 = tlm->H1;
	shm_tel->shmp->H2 = tlm->H2;
	shm_tel->shmp->H3 = tlm->H3;
	shm_tel->shmp->H4 = tlm->H4;
	shm_tel->shmp->H5 = tlm->H5;
	shm_tel->shmp->H6 = tlm->H6;
	shm_tel->shmp->RA = tlm->RA;
	shm_tel->shmp->DEC = tlm->DEC;
	shm_tel->shmp->connected = true;
	
	return true;
}
