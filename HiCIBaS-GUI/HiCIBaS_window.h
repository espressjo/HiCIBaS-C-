#ifndef GTKMM_MAINWINDOW_H
#define GTKMM_MAINWINDOW_H

#include <gtkmm.h>
#include <string>
#include "socket_.h"
#include <gtkmm/statusbar.h>

class HiCIBaS_connection 
{
public:
    HiCIBaS_connection(std::string ip,int port);
    bool snd_cmd(std::string cmd,std::string *value_returned);
    //The following values are kept public so we can 
    //update them as we go.

     int socket_timeout;//timeout in second
     std::string HiCIBaS_ip;
     int HiCIBaS_port;
     bool HiCIBaS_is_tcpip;
     bool HiCIBaS_is_local;
    
};

typedef struct config_t{
    std::string ip;
    int port;
    int polling_time;
    bool tcpip;
    bool local;
    int socket_timeout;
    };
    
class HiCIBaSWindow : public Gtk::Window, public HiCIBaS_connection
{
public:
        HiCIBaSWindow();
        virtual ~HiCIBaSWindow();
        Gtk::Box *get_box();
        Gtk::Toolbar *get_toolbar();
        Gtk::Statusbar *get_statusbar();
        config_t panel_configuration;
protected:
    //Signal handlers:
    
    Gtk::Statusbar statusBar;
    Gtk::Toolbar toolbar;
    Gtk::ToolButton cfg_button;
    Gtk::Box m_VBox;
    Gtk::Separator separator;
    
private:
    
    int status_bar_flag;
    Gtk::Button tryMe;
    void display_connected();
    void display_disconnected();
    int connection_status_timeout;
    bool get_status();//function to update the connection status bar
    sigc::connection m_connection_timeout; //status timeout signal
    void on_button_config();
    
    
};
 
class ConfigWindow : public Gtk::Dialog
{
public:
      ConfigWindow(config_t *cfg_t);
      virtual ~ConfigWindow();

protected:
    //Signal handlers:
    //Gtk::Box m_VBox;
    config_t *cfg_t;
    
    Gtk::Entry e_port,e_ip,e_polling,e_socket_timeout;
    Gtk::Label l_port,l_ip,l_polling,l_comm,l_server,l_socket_timeout;
    Gtk::Box m_HBox_port,m_HBox_ip,m_HBox_polling,m_HBox_buttons,m_HBox_server,m_HBox_comm,m_HBox_socket_timeout;
    Gtk::RadioButton rbtn_udp, rbtn_tcp,rbtn_local,rbtn_remote;
    void on_button_cancel();
    void on_button_set();
    
    Gtk::Button m_button_cancel,m_button_set;

};

#endif //GTKMM_MAINWINDOW_H
