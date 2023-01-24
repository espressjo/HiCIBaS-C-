#ifndef GTKMM_SCRIPTWINDOW_H
#define GTKMM_SCRIPTWINDOW_H

#include "HiCIBaS_window.h"
#include <gtkmm.h>
#include <string>
#include "socket_.h"
#include <gtkmm/statusbar.h>
#include <vector>

class MotorsWindow : public HiCIBaSWindow
{
public:
  MotorsWindow();
  virtual ~MotorsWindow();

protected:
    
	Gtk::Separator sep1,sep2,sep3,sep4,sep5,sep6;
    Gtk::Box 	*m_VBox_main,m_HBox_status,m_VBox_Col_alt,m_HButtonBoxScript,
				m_VBox_Col_az,m_HBox_encoder_alt,m_HButtonBox,
				m_HBox_encoder_az,m_HBox_state_az,m_HBox_state_alt,m_HBox_move,
				m_HBox_move_ctrl,m_HBox_move_label;
	//label for movement 
    Gtk::Label 	label_alt,label_az,m_label_encoder_alt,m_label_encoder_az,m_label_state_az,m_label_state_alt, 
				m_value_encoder_az,m_value_state_az,m_value_encoder_alt,m_value_state_alt;
	
	//lim. switch label
	Gtk::Label 	l_lim_header,l_lim_upper,l_lim_upper_i,l_lim_lower,l_lim_lower_i,l_lim_right_i,l_lim_left_i,
				l_lim_launch_i,l_lim_az_zero_i,l_lim_alt_zero_i,l_lim_right,l_lim_left,
				l_lim_launch,l_lim_az_zero,l_lim_alt_zero;
	//lim. switch box
	Gtk::Box m_VBox_lim,m_HBox_lim_row1,m_HBox_lim_row2,m_HBox_lim_row3,m_HBox_lim_row4;
	
	Gtk::Button move;
	Gtk::Entry e_az,e_alt;
	Gtk::Label l_move_az,l_move_alt,l_move_ctrl;
	void on_button_move();
private:
	std::string script; 
	void update_lim_switch(uint8_t compressed);
	void set_alt_stopped();
	void set_az_stopped();
	void set_az_moving();
	void set_alt_moving();
    bool HiCIBaS_get_status();
	void set_az_encoder(float encoder);
	void set_alt_encoder(float encoder);
	bool isNumeric(Glib::ustring number);
	bool isNumeric(std::string number);
	std::vector<std::string> split(std::string,char sep);
	int move_telescope(float alt,float az);
    sigc::connection m_connection_timeout;//status timeout signal
};

#endif //GTKMM_MAINWINDOW_H
