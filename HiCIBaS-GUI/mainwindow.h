#ifndef GTKMM_MAINWINDOW_H
#define GTKMM_MAINWINDOW_H

#include <gtkmm.h>
#include <string>
#include "socket_.h"
#include <gtkmm/statusbar.h>

class HiCIBaS_connection : public socket_
{

    HiCIBaS_connection(std::string ip,int port);
    // : socket_(ip,port)
    
    
};
/*
HiCIBaS_connection::HiCIBaS_connection(std::string ip,int port) : socket_(ip,port)
{
        
    
}
*/
class MainWindow : public Gtk::Window
{
public:
  MainWindow();
  virtual ~MainWindow();

protected:
    //Signal handlers:

    Gtk::Statusbar statusBar;
    Gtk::Box m_VBox;

private:
    int status_bar_flag;
    int connection_status_timeout;
    bool connection_status();//function to update the connection status bar
    Gdk::RGBA connection_font_color;
    std::string HiCIBaS_ip;//IP address of HiCIBaS's server
    sigc::connection m_connection_timeout; //status timeout signal
};

#endif //GTKMM_MAINWINDOW_H