#ifndef GTKMM_MOTORWINDOW_H
#define GTKMM_MOTORWINDOW_H

#include "HiCIBaS_window.h"
#include <gtkmm.h>
#include <string>
#include "socket_.h"
#include <gtkmm/statusbar.h>
#include <vector>
#include <gtkmm/radiobutton.h>
#include "ledwidget.h"
#include "ui_config_file.h"
#include "motor_status.h"
#include <iomanip>
#include <sstream>
#include <stdlib.h>
enum motor_s{STARTED=0,MOVING,STOPPED,TIMEOUT};

class MotorsWindow : public HiCIBaSWindow
{
public:
  MotorsWindow();
  virtual ~MotorsWindow();

protected:
    
	Gtk::Separator sep1,sep2,sep3,sep4,sep5,sep6,sep7;
    Gtk::Box 	*m_VBox_main,m_HBox_status,m_VBox_Col_alt,m_HButtonBoxScript,
				m_VBox_Col_az,m_HBox_encoder_alt,m_HButtonBox,
				m_HBox_encoder_az,m_HBox_enabled_az,m_HBox_enabled_alt,m_HBox_move,
				m_HBox_move_ctrl,m_HBox_move_label;
	Gtk::HBox m_HBox_degree_az,m_HBox_degree_alt,m_HBox_moving_az,m_HBox_moving_alt;
	//label for movement 
    Gtk::Label 	lbl_alt,lbl_az,lbl_alt_encoder,lbl_az_encoder,lbl_alt_degree,lbl_az_degree,
				lbl_az_enabled,lbl_alt_enabled,lbl_az_moving,lbl_alt_moving;
	
	//lim. switch label
	Gtk::Label 	l_lim_header,l_lim_upper,l_lim_lower,l_lim_right,l_lim_left,
				l_lim_launch,l_lim_az_zero,l_lim_alt_zero;
	//lim. switch box
	Gtk::Box m_VBox_lim,m_HBox_lim_row1,m_HBox_lim_row2,m_HBox_lim_row3,m_HBox_lim_row4;
	
	Gtk::Button move,abort;
	Gtk::ProgressBar m_ProgressBar;
	Gtk::Entry e_az,e_alt;
	Gtk::Label l_move_az,l_move_alt,l_move_ctrl;
    Gtk::RadioButton rbtn_steps,rbtn_degree;

    Gtk::ToolButton cfg_motor_button;
    Gtk::ToolButton terminal_rm8,terminal_nutec;
	void on_button_move();
    void on_button_abort();
    void on_button_motor_config();
    void on_button_terminal_rm8();
    void on_button_terminal_nutec();
	ledWidget led_az_enable,led_alt_enable,led_az_moving,led_alt_moving;
    ledWidget led_upper,led_lower,led_left,led_right,led_az_zero,led_alt_zero,led_launch;
    
private:
	motor_s motor_status;
	void update_lim_switch(uint8_t compressed);
    bool HiCIBaS_get_status();
	void set_az(int32_t encoder,float degree);
	void set_alt(int32_t encoder,float degree);
	void set_alt(int encoder);
	void set_az(int encoder);
	void set_az(double degree);
	void set_alt(double degree);
	int NUTEC_TCP,NUTEC_UDP,RM8_TCP,RM8_UDP;
	bool isNumeric(Glib::ustring number);
	bool isNumeric(std::string number);
	std::vector<std::string> split(std::string,char sep);
	int move_telescope(Glib::ustring alt,Glib::ustring az);
    sigc::connection m_connection_timeout;//status timeout signal
	bool p_bar();
	int timeout;
};

typedef struct {
    
    double nutec_speed;
    double nutec_acc;
    double nutec_dec;
    int rm8_low_speed;
    int rm8_acc;
    int rm8_high_speed;
    
    }config_motor_t;
class ConfigMotor : public Gtk::Dialog
{
public:
      ConfigMotor(config_motor_t *cfg_t);
      virtual ~ConfigMotor();
      bool canceled;  
protected:
    //Signal handlers:
    //Gtk::Box m_VBox;
    config_motor_t *cfg_t;
    
    Gtk::Entry e_nutec_speed,e_nutec_acc,e_nutec_dec,e_rm8_low_speed,e_rm8_acc,e_rm8_high_speed;
    Gtk::Label l_nutec_speed,l_nutec_acc,l_nutec_dec,l_rm8_low_speed,l_rm8_acc,l_rm8_high_speed;
    Gtk::VBox v_nutec,v_rm8; //m_HBox_port,m_HBox_ip,m_HBox_polling,m_HBox_buttons,m_HBox_server,m_HBox_comm,m_HBox_socket_timeout;
    Gtk::HBox h_nutec_1,h_nutec_2,h_nutec_3,h_rm8_1,h_rm8_2,h_rm8_3,m_HBox_buttons,m_HBox_settings;

    void on_button_cancel();
    void on_button_set();
    
    Gtk::Button m_button_cancel,m_button_set;

};


#endif 

