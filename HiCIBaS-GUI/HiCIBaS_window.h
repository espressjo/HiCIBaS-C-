#ifndef GTKMM_HICIBASWINDOW_H
#define GTKMM_HICIBASWINDOW_H

#include <gtkmm.h>
#include <string>
#include "socket_.h"
#include <gtkmm/statusbar.h>
#include "udp_client_socket.h"

#define OK 0
#define NOK -1
#define CONNECTION_P -2

class HiCIBaS_connection 
{
public:
    HiCIBaS_connection(std::string ip,int tcpip_port,int udp_port);
    int snd_cmd(std::string cmd,std::string *value_returned,bool tcpip=true,int timeout=1);
    virtual std::vector<std::string> split_semi_colon(std::string txt);

    //The following values are kept public so we can 
    //update them as we go.

     int socket_timeout;//timeout in second
     std::string HiCIBaS_ip;
     int HiCIBaS_tcpip_port;
	 int HiCIBaS_udp_port;
     bool HiCIBaS_is_tcpip;
     bool HiCIBaS_is_local;
	 int HiCIBaS_socket_timeout;
    
};

typedef struct config_t{
    std::string ip;
    int port;
	int port_udp;
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
    void display_connected();
    void display_disconnected();
    virtual bool HiCIBaS_get_status();//function to update the connection status bar
protected:
    //Signal handlers:
    
    Gtk::Statusbar statusBar;
    Gtk::Toolbar toolbar;
    Gtk::ToolButton cfg_button;
    Gtk::Box m_VBox;
	Gtk::Box m_HBox_status;
    Gtk::Separator separator;
	//information bar
    void on_infobar_response(int response);
	Gtk::InfoBar m_InfoBar;
	Gtk::Label m_Message_Label;
	void set_info_message(std::string msg);

private:
    
    int status_bar_flag;
    Gtk::Button tryMe;
    int connection_status_timeout;
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
    
    Gtk::Entry e_port,e_port_udp,e_ip,e_polling,e_socket_timeout;
    Gtk::Label l_port,l_port_udp,l_ip,l_polling,l_comm,l_server,l_socket_timeout;
    Gtk::Box m_HBox_port,m_HBox_ip,m_HBox_polling,m_HBox_buttons,m_HBox_server,m_HBox_comm,m_HBox_socket_timeout;
    Gtk::RadioButton rbtn_udp, rbtn_tcp,rbtn_local,rbtn_remote;
    void on_button_cancel();
    void on_button_set();
    
    Gtk::Button m_button_cancel,m_button_set;

};

#endif //GTKMM_MAINWINDOW_H
