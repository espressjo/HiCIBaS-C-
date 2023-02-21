#ifndef GTKMM_TELEMETRY_H
#define GTKMM_TELEMETRY_H

#include "HiCIBaS_window.h"
#include <gtkmm.h>
#include <string>
#include "socket_.h"
#include <gtkmm/statusbar.h>
#include <vector>
#include "b64.h"
#include "telemetry.h"

class TelemetryWindow : public HiCIBaSWindow
{
public:
  TelemetryWindow();
  virtual ~TelemetryWindow();

protected:
    
    Gtk::Box 	*m_VBox_main;
	//label for movement 
  
private:
	int get_info(std::string info,std::string *msg,size_t *msglenght);
    bool HiCIBaS_get_status();
    sigc::connection m_connection_timeout;//status timeout signal
	
};

#endif //
