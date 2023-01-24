#include <iostream>
#include "status_window.h"
#include <vector>

StatusWindow::StatusWindow()
  
{   //set the IP adress of HiCIBaS main software.
    //This should eventually be done from commandline or from the gui.
    HiCIBaS_ip="localhost";
	//Script to move the telescope.
	
    //set the main window attributes
    set_title("HiCIBaS Status Server");
    set_border_width(5);
    set_default_size(300, 200);
    //add(m_VBox);//add m_VBox inside the window
	m_VBox_main = get_box();
	
	//::::::::::::::::::::::::::
	//:::   Pack the boxes   :::
	//::::::::::::::::::::::::::        
	
	
	//::::::::::::::::::::::::::::::::::
    //:::   Set the Motors Movement  :::
	//:::::::::::::::::::::::::::::::::: 

    //:::::::::::::::::::::::::::::::::
	//:::   Set lim switch status   :::
	//:::::::::::::::::::::::::::::::::

   
    //::::::::::::  connect some signals ::::::::::::::::::
    //m_connection_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,
    //          &MainWindow::status), 1000 );

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::
  
    //Try to fetch (update) all the scripts name from py_manager. You can always 
    //use the update button if the connection was not established at startup.


    show_all_children();
	m_InfoBar.hide();

}

StatusWindow::~StatusWindow()
{
}

bool StatusWindow::HiCIBaS_get_status()
/*
 * Description
 * -----------
 *      Get status which fetch which script is running or stopped.
 *      It also display the connected or disconnect label in the 
 *      status bar. 
 * 
 */ 
{	//get the lim. switch status
	std::string shm="";
	if (snd_cmd("getshm",&shm,panel_configuration.tcpip,panel_configuration.socket_timeout)==CONNECTION_P){
		display_disconnected();
		return true;
	}
	
	
	
	return true;
}
