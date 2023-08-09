#include <iostream>
#include "telemetry_window.h"
#include <vector>

TelemetryWindow::TelemetryWindow()
: m_HBox(Gtk::ORIENTATION_HORIZONTAL),
m_VBox1(Gtk::ORIENTATION_VERTICAL),
m_VBox2(Gtk::ORIENTATION_VERTICAL),
l_r_scripts("Running Script: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_s_scripts("Stopped Script: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_limswitch("Lim. switchs: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_devices("Devices: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_alt("Alt.: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_az("Az.: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_moteur1("Moteur1: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_moteur2("Moteur2: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_ra("RA: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_dec("DEC: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_t1("TTM: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_t2("RM8: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_t3("Nutec: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_t4("O. bench: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_t5("T5: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_t6("T6: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_h1("H1: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_h2("H2: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_h3("H3: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_h4("H4: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_h5("H5: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_h6("H6: ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER),
l_moteur("Moteur (e): ",Gtk::ALIGN_START,Gtk::ALIGN_CENTER)
{   //set the IP adress of HiCIBaS main software.
    //This should eventually be done from commandline or from the gui.
	
    set_title("HiCIBaS Telemetry");
    set_border_width(5);
    set_default_size(300, 150);
	
	/*
	HiCIBaS_socket_timeout = 800;//set the derived HiCIBaS_connection timeout for socket communication.
    panel_configuration.port = 5555;
	panel_configuration.port_udp = 6555;
    panel_configuration.ip="localhost";
    panel_configuration.tcpip = true;
	panel_configuration.local = false;
    panel_configuration.polling_time = 1000;
    panel_configuration.socket_timeout = 800;
	*/
	
    //add(m_VBox);//add m_VBox inside the window
	m_VBox_main = get_box();
	/*
	connection_status_timeout = panel_configuration.polling_time;
	std::cout<<connection_status_timeout<<std::endl;
	*/
	
	//::::::::::::::::::::::::::
	//:::   Pack the boxes   :::
	//::::::::::::::::::::::::::        
	m_VBox_main->pack_start(m_HBox);
	m_HBox.pack_start(m_VBox1);
	m_HBox.pack_start(separator,Gtk::PACK_SHRINK);
	m_HBox.pack_end(m_VBox2);
	
	m_VBox1.pack_start(l_r_scripts);
	m_VBox1.pack_start(l_s_scripts);
	m_VBox1.pack_start(l_limswitch);
	m_VBox1.pack_start(l_devices);
	m_VBox1.pack_start(l_alt);
	m_VBox1.pack_start(l_az);
	m_VBox1.pack_start(l_moteur1);
	m_VBox1.pack_start(l_moteur2);
	m_VBox2.pack_start(l_t1);
	m_VBox2.pack_start(l_t2);
	m_VBox2.pack_start(l_t3);
	m_VBox2.pack_start(l_t4);
	m_VBox2.pack_start(l_t5);
	m_VBox2.pack_start(l_t6);
	m_VBox2.pack_start(l_h1);
	m_VBox2.pack_start(l_h2);
	m_VBox2.pack_start(l_h3);
	m_VBox2.pack_start(l_h4);
	m_VBox2.pack_start(l_h5);
	m_VBox2.pack_start(l_h6);
	m_VBox2.pack_start(l_moteur);
	m_VBox1.pack_start(l_ra);
	m_VBox1.pack_start(l_dec);
	
	
	
	

	
	
	/*
	TelemetryWindow::m_connection_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,
              &TelemetryWindow::HiCIBaS_get_status), 1000 );
	*/
   
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
	ret = snd_cmd("gettelemetry",&resp,panel_configuration.tcpip,panel_configuration.socket_timeout);
	
	if (ret==CONNECTION_P){
		display_disconnected();
		shm_tel->shmp->connected = false;
		return true;
	}
	//:::::::::::::::::::::::::::::::::::::::::
	//:::   Try to strip the first number   :::
	//:::::::::::::::::::::::::::::::::::::::::
	std::string buff="";
	for (auto &c:resp){
		if (c==' '){break;}
		buff+=c;
	}
	bool T=true;
	for (auto &c:buff){
		T = T && isdigit(c);
	}
	if (!T){
		set_info_message("information received is gibberish");
		shm_tel->shmp->connected = false;
		return true;
	}
	
	length = std::atoi(buff.c_str());
	//::::::::::::::::::::::::::::::::::
	//:::   strip the message part   :::
	//::::::::::::::::::::::::::::::::::
	resp = resp.substr(buff.length()+1,resp.length());
	
	
	if (islb64DecodeAlloc(resp.c_str(),length,&decoded,&outlength)!=0)
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
	if ((tlm->moteur & 2) ==2){shm_tel->shmp->az_moving = true;}
	else {shm_tel->shmp->az_moving = false;}
	if ((tlm->moteur & 1) ==1){shm_tel->shmp->alt_moving = true;}
	else {shm_tel->shmp->alt_moving = false;}
	
	if ((tlm->moteur & 4) ==4){shm_tel->shmp->nutec_enable = true;}
	else {shm_tel->shmp->nutec_enable = false;}
	
	if ((tlm->moteur & 8) ==8){shm_tel->shmp->rm8_enable = true;}
	else {shm_tel->shmp->rm8_enable = false;}
	
	if ((tlm->devices & 128) ==128 ){shm_tel->shmp->lim_activte = true;}
	else{shm_tel->shmp->lim_activte = false;}
	
	
	if ((tlm->devices & 1) ==1){shm_tel->shmp->cam1 = true;}//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	else {shm_tel->shmp->cam1 = false;}
	
	if ((tlm->devices & 2) ==2){shm_tel->shmp->cam2 = true;}//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	else {shm_tel->shmp->cam2 = false;}
	if ((tlm->devices & 4) ==4){shm_tel->shmp->cam3 = true;}//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	else {shm_tel->shmp->cam3 = false;}
	if ((tlm->devices & 8) ==8){shm_tel->shmp->TTM = true;}//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	else {shm_tel->shmp->TTM = false;}
	if ((tlm->devices & 16) ==16){shm_tel->shmp->source_calibration = true;}//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	else {shm_tel->shmp->source_calibration = false;}
	
	if ((tlm->devices & 32) ==32){shm_tel->shmp->nutec_active = true;}//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	else {shm_tel->shmp->nutec_active = false;}
	
	if ((tlm->devices & 64) ==64){shm_tel->shmp->rm8_active = true;}//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	else {shm_tel->shmp->rm8_active = false;}
	
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
	shm_tel->shmp->s_scripts = tlm->s_scripts;
	shm_tel->shmp->r_scripts = tlm->r_scripts;
	shm_tel->shmp->moteur = tlm->moteur;
	shm_tel->shmp->connected = true;
	//::::::::::::::::::::::::::::::::::
	//:::   Now lets update the UI   :::
	//::::::::::::::::::::::::::::::::::
	
	l_r_scripts.set_text("Running Script: "+std::to_string(tlm->r_scripts));
	l_s_scripts.set_text("Stopped Script: "+std::to_string(tlm->s_scripts));
	l_limswitch.set_text("Lim. switchs: "+std::to_string(tlm->limswitch));
	l_devices.set_text("Devices: "+std::to_string(tlm->devices));
	l_alt.set_text("Alt.: "+std::to_string(tlm->alt));
	l_az.set_text("Az.: "+std::to_string(tlm->az));
	l_moteur1.set_text("Moteur1: "+std::to_string(tlm->moteur_1));
	l_moteur2.set_text("Moteur2: "+std::to_string(tlm->moteur_2));
	l_ra.set_text("RA: "+std::to_string(tlm->RA));
	l_dec.set_text("DEC: "+std::to_string(tlm->DEC));
	l_t1.set_text("TTM: "+std::to_string(tlm->T1));
	l_t2.set_text("RM8: "+std::to_string(tlm->T2));
	l_t3.set_text("Nutec: "+std::to_string(tlm->T4));
	l_t4.set_text("O. bench: "+std::to_string(tlm->T3));
	l_t5.set_text("T5: "+std::to_string(tlm->T5));
	l_t6.set_text("T6: "+std::to_string(tlm->T6));
	l_h1.set_text("H1: "+std::to_string(tlm->H1));
	l_h2.set_text("H2: "+std::to_string(tlm->H2));
	l_h3.set_text("H3: "+std::to_string(tlm->H3));
	l_h4.set_text("H4: "+std::to_string(tlm->H4));
	l_h5.set_text("H5: "+std::to_string(tlm->H5));
	l_h6.set_text("H6: "+std::to_string(tlm->H6));
	l_moteur.set_text("moteur (e): "+std::to_string(tlm->moteur));
	//display connection status
	display_connected();
	return true;
}
