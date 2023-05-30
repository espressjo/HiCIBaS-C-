#ifndef GTKMM_TELEMETRY_H
#define GTKMM_TELEMETRY_H

#include "HiCIBaS_window.h"
#include <gtkmm.h>
#include <string>
#include "socket_.h"
#include <gtkmm/statusbar.h>
#include <vector>
#include "b64.h"
#include "telemetry.h"

class TelemetryWindow : public HiCIBaSWindow
{
public:
  TelemetryWindow();
  virtual ~TelemetryWindow();

protected:
    
    Gtk::Box 	*m_VBox_main;
	Gtk::Box m_VBox1,m_VBox2;
	Gtk::Box m_HBox;
    Gtk::Separator separator; 
	Gtk::Label l_limswitch,l_devices,l_alt,l_r_scripts,l_s_scripts,l_az,l_moteur1,l_moteur2,
	l_t1,l_t2,l_t3,l_t4,l_t5,l_t6,l_h1,l_h2,l_h3,l_h4,l_h5,l_h6,l_ra,l_dec,l_moteur;
	/*
	uint8_t limswitch;//<0> upper, <1> lower , <2> right, <3> left, <4> launch, <5> AZ-0, <6> ALT-0
	uint8_t devices;//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>,limswitch<7>
	float alt;//moteur altitude in degree
	float az;//moteru azimuth in degree
	uint16_t moteur_1;//encoder moteur #1
	uint16_t moteur_2;//encoder moteur #2
	int8_t T1;//Temperature #1
	int8_t T2;//Temperature #2
	int8_t T3;//Temperature #3
	int8_t T4;//Temperature #4
	int8_t T5;//Temperature #5
	int8_t T6;//Temperature #6
	uint8_t H1;//heater #1
	uint8_t H2;//heater #2
	uint8_t H3;//heater #3
	uint8_t H4;//heater #4
	uint8_t H5;//heater #5
	uint8_t H6;//heater #6
	float RA;//RA of the last astrometry measurement
	float DEC;//DEC of the last astrometry measurment
	uint32_t r_scripts;//Running Script.
	uint32_t s_scripts;//Stopped Script.
	*/
	
private:
	int get_info(std::string info,std::string *msg,size_t *msglenght);
    bool HiCIBaS_get_status();
	int connection_status_timeout;
    sigc::connection m_connection_timeout;//status timeout signal
	
};

#endif //
