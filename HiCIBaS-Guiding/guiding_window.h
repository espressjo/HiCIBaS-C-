#ifndef GTKMM_GUIDINGWINDOW_H
#define GTKMM_GUIDINGWINDOW_H

#include "HiCIBaS_window.h"
#include <gtkmm.h>
#include <string>
#include "socket_.h"
#include <gtkmm/statusbar.h>
#include <vector>



class GuidingWindow : public HiCIBaSWindow
{
public:
  GuidingWindow();
  virtual ~GuidingWindow();

protected:
    
    Gtk::Box 	*m_VBox_main;
	//label for movement 
    //Gtk::Label 	;
	Gtk::Box m_HBox_Button,m_HBox_Status,m_HBox_StatusV1,m_HBox_StatusV2;
	Gtk::Button capture,download;
	//lim. switch label
	Gtk::Label l_ra,l_dec;
	//lim. switch box
	//Gtk::Box m_VBox_lim,m_HBox_lim_row1,m_HBox_lim_row2,m_HBox_lim_row3,m_HBox_lim_row4;
	Gtk::Separator sep1,sep2,sep3;
	//Gtk::Button move;
	//Gtk::ProgressBar m_ProgressBar;
	//Gtk::Entry e_az,e_alt;
	//Gtk::Label l_move_az,l_move_alt,l_move_ctrl;
	//void on_button_move();
	void on_button_capture();
	void on_button_download();
private:
	//std::string script; 
	//motor_s motor_status;
	bool HiCIBaS_get_status();
	//std::vector<std::string> split(std::string,char sep);
	//int move_telescope(float alt,float az);
    //sigc::connection m_connection_timeout;//status timeout signal
	//bool p_bar();
};

#endif 
