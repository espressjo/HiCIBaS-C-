#ifndef TELEMETRY_CONTROL_H
#define TELEMETRY_CONTROL_H


//#include "HiCIBaS_window.h"
#include <gtkmm.h>
#include "ledwidget.h"
#include "HiCIBaS_window.h"
#include <string>
#include <iostream>
#include <vector>
#include "ui_config_file.h"
using namespace std;
/*
#include <string>
#include "socket_.h"
#include <vector>
#include "b64.h"
#include "telemetry.h"
#include "astro_stuff.h"
*/



class TelemetryControlWindow : public Gtk::Dialog, public HiCIBaS_connection
{
public:
  TelemetryControlWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,shared_telemetry *shm_tel);
  virtual ~TelemetryControlWindow();

protected:
    
    Gtk::Button *btn_heater_nutec_set,*btn_heater_nutec_off,*btn_heater_ttm_set,*btn_heater_ttm_off,*btn_cam_coarse_idle,
    *btn_cam_coarse_stop,*btn_cam_fine_idle,*btn_cam_fine_stop,*btn_cam_sh_idle,*btn_cam_sh_stop,*btn_close;
    ledWidget led_nutec,led_ttm,led_coarse,led_fine,led_sh;
    Gtk::Box *vbox_nutec,*vbox_ttm,*vbox_coarse,*vbox_fine,*vbox_sh;
    Gtk::Entry *entry_ttm,*entry_nutec;
    gboolean test(GtkWidget *widget, GdkEvent  *event, gpointer   user_data);
    bool on_delete_event( GdkEventAny* );
private:
    shared_telemetry *shm;
    sigc::connection m_connection_timeout;
    string strip_space(string w);
    string host;
int get_heater_val(string heater_return,string heater,string *value);

vector<string> split(string str,char sep);
    void on_btn_heater_nutec_set_clicked();
    void on_btn_heater_nutec_off_clicked();
    void on_btn_heater_ttm_set_clicked();
    void on_btn_heater_ttm_off_clicked();
    void on_btn_cam_coarse_idle_clicked();
    void on_btn_cam_coarse_stop_clicked();
    void on_btn_cam_fine_idle_clicked();
    void on_btn_cam_sh_idle_clicked();
    void on_btn_cam_sh_stop_clicked();
	void on_btn_cam_fine_stop_clicked();
	void on_btn_close_clicked();
    bool get_status();
    bool ON;
};

#endif //
