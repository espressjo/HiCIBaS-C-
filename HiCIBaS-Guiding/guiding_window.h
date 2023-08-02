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

class GuidingWindow : public HiCIBaSWindow
{
public:
  GuidingWindow();
  virtual ~GuidingWindow();

protected:
    
    Gtk::Box 	*m_VBox_main;

	Gtk::Box m_HBox_Button1,m_HBox_Button2,m_HBox_Status,m_HBox_StatusV1,m_HBox_StatusV2;
	Gtk::HBox b_star1,b_star2,b_star3,b_star4;
	Gtk::Button capture,download,move_target,coarse_guiding,fine_guiding;
	Gtk::Label l_ra,l_dec,l_star1,l_star2,l_star3,l_star4;
	Gtk::ProgressBar m_ProgressBar;
	Gtk::Entry guiding_x,guiding_y,target_x,target_y;
	Gtk::Separator sep1,sep2,sep3,sep4,sep5;
	void on_button_capture();
	void on_button_readoutput();
	void on_button_center();
	void on_button_fine();
	void on_button_coarse();
	ledWidget led_guiding_coarse,led_guiding_fine;
    bool is_Int(Gtk::Entry S);
private:

	bool HiCIBaS_get_status();
	py_manager *Py;
	std::string pyScript;
	bool uiRunning;
	bool str_contains(std::string script,std::string active_scripts);
	//bool is_Int(Gtk::Entry S);
};

#endif 

