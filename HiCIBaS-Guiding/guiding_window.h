#ifndef GTKMM_GUIDINGWINDOW_H
#define GTKMM_GUIDINGWINDOW_H

#include "HiCIBaS_window.h"
#include <gtkmm.h>
#include <string>
#include "socket_.h"
#include <gtkmm/statusbar.h>
#include <vector>
#include "py_manager.h"
#include "py_scripts_config.h"
#include "ledwidget.h"
#include "cam_config_window.h"
#include <chrono>
#include <thread>


enum script_states {IDLE_SCRIPT=0,MOVE,CAPTURE,TMP};

class GuidingWindow : public HiCIBaSWindow
{
public:
  GuidingWindow();
  virtual ~GuidingWindow();

protected:
    
    Gtk::Box 	*m_VBox_main;

	Gtk::Box m_HBox_Button1,m_HBox_Button2,m_HBox_Status,m_HBox_StatusV1,m_HBox_StatusV2;
	Gtk::HBox b_star1,b_star2,b_star3,b_star4;
	Gtk::Button capture,download,move_target,coarse_guiding,btn_create_tmp;
	Gtk::Label l_ra,l_dec,l_star1,l_star2,l_star3,l_star4;
	Gtk::ProgressBar m_ProgressBar;
	Gtk::Entry guiding_x,guiding_y,target_x,target_y;
	Gtk::Separator sep1,sep2,sep3,sep4,sep5;
	void on_button_capture();
	void on_button_readoutput();
	void on_button_center();
	void on_button_create_tmp();
	void on_button_coarse();
    void on_button_coarse_cam_config();
    void on_button_fine_cam_config();
	ledWidget led_guiding_coarse,led_guiding_fine;
    Gtk::ToolButton cfg_coarse_cam_button,cfg_fine_cam_button;
private:
    bool is_Int(Gtk::Entry *S); 
	bool HiCIBaS_get_status();
	py_manager *Py;
	std::string pyScript;
	bool uiRunning;
    script_states state;
	bool str_contains(std::string script,std::string active_scripts);
};

#endif 

