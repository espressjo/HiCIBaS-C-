#include <iostream>
#include "guiding_window.h"
#include <vector>

GuidingWindow::GuidingWindow()
:capture("Capture"),
download("Download"),
l_ra("RA: 0.000"),
l_dec("DEC: 0.000"),
m_HBox_Status(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_StatusV1(Gtk::ORIENTATION_VERTICAL),
m_HBox_StatusV2(Gtk::ORIENTATION_VERTICAL),
m_HBox_Button(Gtk::ORIENTATION_HORIZONTAL)
  
{   //set the IP adress of HiCIBaS main software.
    //This should eventually be done from commandline or from the gui.
	
	//Script to move the telescope.
	//script = "/opt/HiCIBaS/guiding.py";
    //set the main window attributes
    set_title("HiCIBaS Guiding Manager");
    set_border_width(5);
    set_default_size(220, 200);
    //add(m_VBox);//add m_VBox inside the window
	m_VBox_main = get_box();
	m_VBox_main->pack_start(m_HBox_Status,Gtk::PACK_SHRINK);
	m_VBox_main->pack_start(sep1,Gtk::PACK_SHRINK);
	m_VBox_main->pack_start(m_HBox_Button,Gtk::PACK_SHRINK);
	
	//::::::::::::::::::::::::::
	//:::   Pack the boxes   :::
	//::::::::::::::::::::::::::        
	m_HBox_Button.pack_start(capture,Gtk::PACK_SHRINK);
	m_HBox_Button.pack_start(sep2,Gtk::PACK_SHRINK);
	m_HBox_Button.pack_end(download,Gtk::PACK_SHRINK);
	
	m_HBox_Status.pack_start(m_HBox_StatusV1,Gtk::PACK_SHRINK);
	m_HBox_Status.pack_start(sep3,Gtk::PACK_SHRINK);
	m_HBox_Status.pack_end(m_HBox_StatusV2,Gtk::PACK_SHRINK);
	
	m_HBox_StatusV1.pack_start(l_ra);
	m_HBox_StatusV1.pack_start(l_dec);
	
	


	
    //::::::::::::  connect some signals ::::::::::::::::::

    capture.signal_clicked().connect( sigc::mem_fun(*this,&GuidingWindow::on_button_capture));
	download.signal_clicked().connect( sigc::mem_fun(*this,&GuidingWindow::on_button_download));

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::
  
    //Try to fetch (update) all the scripts name from py_manager. You can always 
    //use the update button if the connection was not established at startup.


    show_all_children();
	//testing !!!
	
}

GuidingWindow::~GuidingWindow()
{
}

void GuidingWindow::on_button_capture()
{
	printf("Capture\n");
}
void GuidingWindow::on_button_download()
{
	printf("Download\n");
}



bool GuidingWindow::HiCIBaS_get_status()
/*
 * Description
 * -----------
 *      Get status which fetch which script is running or stopped.
 *      It also display the connected or disconnect label in the 
 *      status bar. 
 * 
 */ 
{	
	//:::::::::::::::::::::::::::::::::::::::::
	//:::   Case we update the UI locally   :::
	//:::::::::::::::::::::::::::::::::::::::::
	if (panel_configuration.local){
	
		if (!shm_tel->shmp->connected)
		{
			display_disconnected();
			return true;
		}
		display_connected();
		
		
		return true;
	}//end of local update
	else{
	//::::::::::::::::::::::::::::::::::::
	//:::   Remote Update via socket   :::
	//::::::::::::::::::::::::::::::::::::
	
	
		return true;
	}

	return true;
}






