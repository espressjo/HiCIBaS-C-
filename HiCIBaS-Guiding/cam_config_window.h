#ifndef CAM_CONFIG_WINDOW_H
#define CAM_CONFIG_WINDOW_H


//#include "HiCIBaS_window.h"
#include <gtkmm.h>
#include <string>
#include "socket_.h"
#include <vector>
#include "ui_config_file.h"
#include "motor_status.h"



typedef struct {
    std::string exp;
    std::string fps;
    std::string gain;
    std::string boost;
    }cam_param;
    
class ConfigCam : public Gtk::Dialog
{
public:
      ConfigCam(std::string cam,std::string *cmd,cam_param param);
      virtual ~ConfigCam();

protected:

    Gtk::CheckButton btn_boost;
    Gtk::Entry entry_exp,entry_fps,entry_gain; //,e_nutec_acc,e_nutec_dec,e_rm8_low_speed,e_rm8_acc,e_rm8_high_speed;
    Gtk::Label lbl_exp,lbl_fps,lbl_gain,lbl_boost;
    Gtk::HBox hbox_btn; 
    Gtk::HBox hb1,hb2,hb3,hb4;

    void on_button_cancel();
    void on_button_set();
    
    Gtk::Button btn_cancel,btn_set;
private:
    std::string cam_name;
    std::string script;
    std::string *cmd_;
};
#endif