#include <iostream>
#include "guiding_window.h"
#include <vector>

GuidingWindow::GuidingWindow()
:capture("Capture"),
download("Read Output"),
l_ra("RA: 0.000"),
l_dec("DEC: 0.000"),
l_star1("Target X: "),
l_star2("Target Y: "),
l_star3("Guiding X: "),
l_star4("Guiding Y: "),
move_target("Center Target"),
coarse_guiding("Coarse Guiding"),
fine_guiding("Fine Guiding"),
m_HBox_Status(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_StatusV1(Gtk::ORIENTATION_VERTICAL),
m_HBox_StatusV2(Gtk::ORIENTATION_VERTICAL),
m_HBox_Button2(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_Button1(Gtk::ORIENTATION_HORIZONTAL)
  
{   //set the IP adress of HiCIBaS main software.
    //This should eventually be done from commandline or from the gui.
	
	//Script to move the telescope.
	//script = "/opt/HiCIBaS/guiding.py";
    //set the main window attributes
	uiRunning=false;
    set_title("HiCIBaS Guiding Manager");
    set_border_width(5);
    set_default_size(320, 200);
	
    //add(m_VBox);//add m_VBox inside the window
	m_VBox_main = get_box();
	m_VBox_main->pack_start(m_HBox_Status,Gtk::PACK_SHRINK);
	m_VBox_main->pack_start(sep1,Gtk::PACK_SHRINK);
	m_VBox_main->pack_start(m_HBox_Button1,Gtk::PACK_SHRINK);
	m_VBox_main->pack_start(m_HBox_Button2,Gtk::PACK_SHRINK);
	//::::::::::::::::::::::::::
	//:::   Pack the boxes   :::
	//::::::::::::::::::::::::::        
	m_HBox_Button1.pack_start(capture,Gtk::PACK_SHRINK);
	m_HBox_Button1.pack_start(sep2,Gtk::PACK_SHRINK);
	m_HBox_Button1.pack_start(download,Gtk::PACK_SHRINK);
	m_HBox_Button1.pack_start(sep4,Gtk::PACK_SHRINK);
	m_HBox_Button1.pack_start(move_target,Gtk::PACK_SHRINK);
	
	m_HBox_Button2.pack_start(coarse_guiding,Gtk::PACK_SHRINK);
	m_HBox_Button2.pack_start(sep5,Gtk::PACK_SHRINK);
	m_HBox_Button2.pack_start(fine_guiding,Gtk::PACK_SHRINK);
	
	m_HBox_Status.pack_start(m_HBox_StatusV1,Gtk::PACK_SHRINK);
	m_HBox_Status.pack_start(sep3,Gtk::PACK_SHRINK);
	m_HBox_Status.pack_end(m_HBox_StatusV2,Gtk::PACK_SHRINK);
	
	m_HBox_StatusV1.pack_start(l_ra);
	m_HBox_StatusV1.pack_start(l_dec);
	m_HBox_StatusV1.pack_start(led_guiding_coarse);
	m_HBox_StatusV1.pack_start(led_guiding_fine);
	l_ra.set_halign(Gtk::ALIGN_START);
	l_dec.set_halign(Gtk::ALIGN_START);
	led_guiding_coarse.set_halign(Gtk::ALIGN_START);
	led_guiding_fine.set_halign(Gtk::ALIGN_START);
	led_guiding_coarse.deactivate();
	led_guiding_fine.deactivate();
	led_guiding_coarse.set_label("Coarse guiding");
	led_guiding_fine.set_label("Fine guiding");
	
	target_x.set_width_chars(5);
	target_y.set_width_chars(5);
	center_x.set_width_chars(5);
	center_y.set_width_chars(5);
	center_x.set_text("800");
	center_y.set_text("553");
	b_star1.pack_start(l_star1,Gtk::PACK_SHRINK);
	b_star1.pack_start(target_x,Gtk::PACK_SHRINK);
	m_HBox_StatusV2.pack_start(b_star1,Gtk::PACK_SHRINK);
	b_star2.pack_start(l_star2,Gtk::PACK_SHRINK);
	b_star2.pack_start(target_y,Gtk::PACK_SHRINK);
	m_HBox_StatusV2.pack_start(b_star2,Gtk::PACK_SHRINK);
	b_star3.pack_start(l_star3,Gtk::PACK_SHRINK);
	b_star3.pack_start(center_x,Gtk::PACK_SHRINK);
	m_HBox_StatusV2.pack_start(b_star3,Gtk::PACK_SHRINK);
	b_star4.pack_start(l_star4,Gtk::PACK_SHRINK);
	b_star4.pack_start(center_y,Gtk::PACK_SHRINK);
	m_HBox_StatusV2.pack_start(b_star4,Gtk::PACK_SHRINK);
	//center_x,center_y,target_x,target_y
	
	//::::::::::::::::::::::::::::::::
	//:::   set the progress bar   :::
	//::::::::::::::::::::::::::::::::
	m_ProgressBar.set_margin_end(5);
	m_ProgressBar.set_halign(Gtk::ALIGN_CENTER);
	m_ProgressBar.set_valign(Gtk::ALIGN_CENTER);
	m_ProgressBar.set_size_request(50, -1);
	//m_ProgressBar.pulse();
	m_ProgressBar.set_text("Downloading...");
	m_ProgressBar.set_show_text(false);
	m_VBox_main->pack_end(m_ProgressBar,Gtk::PACK_SHRINK);
	


	
    //::::::::::::  connect some signals ::::::::::::::::::

    capture.signal_clicked().connect( sigc::mem_fun(*this,&GuidingWindow::on_button_capture));
	download.signal_clicked().connect( sigc::mem_fun(*this,&GuidingWindow::on_button_readoutput));
	fine_guiding.signal_clicked().connect( sigc::mem_fun(*this,&GuidingWindow::on_button_fine));
	coarse_guiding.signal_clicked().connect( sigc::mem_fun(*this,&GuidingWindow::on_button_coarse));
	move_target.signal_clicked().connect( sigc::mem_fun(*this,&GuidingWindow::on_button_center));
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::
  
    //Try to fetch (update) all the scripts name from py_manager. You can always 
    //use the update button if the connection was not established at startup.
	std::string interpreter="";
	pyScript = "/opt/HiCIBaS/scripts/ui_guiding.py";
	if (getInterpreter("/opt/HiCIBaS/config/scripts.txt",&interpreter)!=0)
	{
		std::cout<<"Unable to find the script config file!"<<std::endl;
		//return 0;	
	}
	std::cout<<"interpreter: "<<interpreter<<std::endl;
	Py = new py_manager(interpreter);
	Py->add_python_script(pyScript);
	

    show_all_children();
	//testing !!!
	
}

GuidingWindow::~GuidingWindow()
{
}

void GuidingWindow::on_button_capture()
{
	printf("Sending request to capture FOV.\n");
	std::string tmp_stopped_script="";
	if (snd_cmd("python script "+pyScript+" arg1 --capture-fov -run",&tmp_stopped_script,panel_configuration.tcpip,panel_configuration.socket_timeout)!=OK)
	{	
		printf("Request sent, but we did not receive OK from server.\n");
		set_info_message("OK not received.");
	}
	
	else{
		//lets flag the ui_guiding.py script is running.
		uiRunning=true;
	}
	printf("OK received from server.\n");
	
}
void GuidingWindow::on_button_readoutput()
{
	std::string stderr="",stdout="",msg="";
	Py->get_stderr(pyScript,&stderr);
	Py->get_stdout(pyScript,&stdout);
	msg = "stdout:\n"+stdout+"\nstderr:\n"+stderr;
	
	print_message(msg,"[stdout/stderr]");
	
}
void GuidingWindow::on_button_center()
{
		
	printf("Center target\n.");
	
}
void GuidingWindow::on_button_coarse()
{
		if (led_guiding_coarse.isActive()){
		led_guiding_coarse.deactivate();}
	else{
		led_guiding_coarse.activate();}
	printf("coarse guiding\n.");
	
}
void GuidingWindow::on_button_fine()
{
	if (led_guiding_fine.isActive()){
		led_guiding_fine.deactivate();}
	else{
		led_guiding_fine.activate();}
	printf("fine guiding\n.");
	
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
		std::string tmp_stopped_script="";
		
		if (snd_cmd("python -whos_stopped",&tmp_stopped_script,panel_configuration.tcpip,panel_configuration.socket_timeout)!=OK)
		{	//if we don't have connection return
			display_disconnected();
			return true;
		}
		else {
			display_connected();
		}
		//check if ui_guiding has stop running and was started
		if (str_contains(pyScript,tmp_stopped_script) && uiRunning)
		{
			uiRunning=false;
			std::string tmp="";
			if (snd_cmd("python script "+pyScript+" -stdout",&tmp,panel_configuration.tcpip,panel_configuration.socket_timeout)!=OK)
			{	
				set_info_message("NOK received.");
				m_ProgressBar.set_show_text(false);
				m_ProgressBar.set_fraction(0);
				return true;
			}
			if (tmp.length()<10){
				m_ProgressBar.set_show_text(false);
				m_ProgressBar.set_fraction(0);
				std::string stderr_remote="";
				if (snd_cmd("python script "+pyScript+" -stderr",&stderr_remote,panel_configuration.tcpip,panel_configuration.socket_timeout)!=OK)
				{
					set_info_message("NOK received (stderr).");
					return true;
				}
				if (stderr_remote.compare("")!=0){
					print_message(stderr_remote,"[Failed]");
					return true;
				}
				else{
					print_message("No star found.","[Warning!!!]");
				}
				
				
				return true;
			}
			std::vector<std::string> argv;
			//lets execute a local python script the xpa-display the fake image,
			argv.push_back("--data="+tmp);
			argv.push_back("--display");			
			Py->run(pyScript,argv);
			m_ProgressBar.set_show_text(false);
			m_ProgressBar.set_fraction(0);
			printf("Image should be displayed by ds9...\n");
		}
		else if (!str_contains(pyScript,tmp_stopped_script) && uiRunning){
			m_ProgressBar.set_show_text(true);
			m_ProgressBar.pulse();
		}
	
		return true;
	}//else (tcp/ip

	return true;
}

bool GuidingWindow::str_contains(std::string script,std::string active_scripts){
	
if (active_scripts.find(script) != std::string::npos) {
    return true;
}
	return false;
}




