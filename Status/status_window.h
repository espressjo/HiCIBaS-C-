#ifndef GTKMM_SCRIPTWINDOW_H
#define GTKMM_SCRIPTWINDOW_H

#include "HiCIBaS_window.h"
#include <gtkmm.h>
#include <string>
#include "socket_.h"
#include <gtkmm/statusbar.h>
#include <vector>
#include "uics.h"

class StatusWindow : public HiCIBaSWindow
{
public:
  StatusWindow();
  virtual ~StatusWindow();

protected:
    
    Gtk::Box 	*m_VBox_main;
	//label for movement 

private:

    bool HiCIBaS_get_status();

    sigc::connection m_connection_timeout;//status timeout signal
};

#endif //GTKMM_MAINWINDOW_H
