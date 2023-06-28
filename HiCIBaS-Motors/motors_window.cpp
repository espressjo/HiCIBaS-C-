#include <iostream>
#include "motors_window.h"
#include <vector>

MotorsWindow::MotorsWindow()
: m_HBox_status(Gtk::ORIENTATION_HORIZONTAL),
m_VBox_Col_alt(Gtk::ORIENTATION_VERTICAL),
m_VBox_Col_az(Gtk::ORIENTATION_VERTICAL),
m_HBox_encoder_alt(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_encoder_az(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_enabled_az(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_enabled_alt(Gtk::ORIENTATION_HORIZONTAL),
m_HButtonBox(Gtk::ORIENTATION_HORIZONTAL),
m_HButtonBoxScript(Gtk::ORIENTATION_HORIZONTAL),
l_move_az("Az: "),
l_move_alt("Alt: "),
move("move"),
l_move_ctrl("Relative Telescope Movement"),
m_HBox_move(Gtk::ORIENTATION_VERTICAL),
m_HBox_move_label(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_move_ctrl(Gtk::ORIENTATION_HORIZONTAL),
m_VBox_lim(Gtk::ORIENTATION_VERTICAL),
m_HBox_lim_row1(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_lim_row2(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_lim_row3(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_lim_row4(Gtk::ORIENTATION_HORIZONTAL),
l_lim_lower_i(""),
l_lim_upper_i(""),
l_lim_right_i(""),
l_lim_left_i(""),
l_lim_launch_i(""),
l_lim_az_zero_i(""),
l_lim_alt_zero_i(""),
l_lim_lower("Lower: "),
l_lim_upper("Upper: "),
l_lim_right("Right: "),
l_lim_left("Left: "),
l_lim_launch("Launch: "),
l_lim_az_zero("Az-0: "),
l_lim_alt_zero("Alt-0: "),
lbl_alt("Altitude"),
lbl_az("Azimuth"),
lbl_alt_encoder("Encoder: 0     "),
lbl_az_encoder("Encoder: 0      "),
lbl_alt_degree("°      : 0.00"),
lbl_az_degree("°      : 0.00"),
lbl_az_enabled ("Enabled "),
lbl_alt_enabled("Enabled "),
lbl_az_moving("Moving   "),
lbl_alt_moving("Moving   "),
l_lim_header("Limit Switch status")


  
{   //set the IP adress of HiCIBaS main software.
    //This should eventually be done from commandline or from the gui.
	motor_status = STOPPED;
	//Script to move the telescope.
	timeout = 60;
	script = "/opt/HiCIBaS/python/moteurs.py";
	ui_get_string("/opt/HiCIBaS/config/HiCIBaS.conf","MSCRIPT",&script);
	//look in the config file
	printf("Using %s as the motor script\n",script.c_str());
	
    //set the main window attributes
    set_title("HiCIBaS Telescope Manager");
    set_border_width(5);
    set_default_size(220, 350);
    //add(m_VBox);//add m_VBox inside the window
	m_VBox_main = get_box();
	
	//::::::::::::::::::::::::::
	//:::   Pack the boxes   :::
	//::::::::::::::::::::::::::        
	
	m_VBox_main->pack_start(m_HBox_status,Gtk::PACK_SHRINK);
    m_VBox_main->pack_start(sep1,Gtk::PACK_SHRINK);
	sep1.set_margin_top(10);
	sep1.set_margin_bottom(10);
	sep6.set_margin_top(10);
	sep6.set_margin_bottom(10);
	sep7.set_margin_top(10);
	sep7.set_margin_bottom(10);
	m_VBox_main->pack_start(m_HBox_move,Gtk::PACK_SHRINK);//motor rel. movement
	m_VBox_main->pack_start(sep6,Gtk::PACK_SHRINK);
	m_VBox_main->pack_start(m_VBox_lim,Gtk::PACK_SHRINK);
	m_VBox_main->pack_start(sep7,Gtk::PACK_SHRINK);
	m_VBox_main->pack_start(m_ProgressBar,Gtk::PACK_SHRINK);
    //movement box
	m_HBox_move.pack_start(m_HBox_move_label);
	m_HBox_move.pack_start(m_HBox_move_ctrl);
	
	m_VBox_Col_alt.set_margin_start(5);
    m_VBox_Col_az.set_margin_start(5);
	
	//::::::::::::::::::::::::::::::::
	//:::   set the progress bar   :::
	//::::::::::::::::::::::::::::::::
	m_ProgressBar.set_margin_end(5);
	m_ProgressBar.set_halign(Gtk::ALIGN_CENTER);
	m_ProgressBar.set_valign(Gtk::ALIGN_CENTER);
	m_ProgressBar.set_size_request(50, -1);
	//m_ProgressBar.pulse();
	m_ProgressBar.set_text("Waiting for motor to start");
	m_ProgressBar.set_show_text(false);
	
	//::::::::::::::::::::::::::::::::::::::::
    //:::   Set the Motors encoder status  :::
	//::::::::::::::::::::::::::::::::::::::::    
    m_HBox_status.pack_start(m_VBox_Col_alt,Gtk::PACK_SHRINK);
    m_HBox_status.pack_start(sep4,Gtk::PACK_SHRINK);//m_Separator
	
	sep4.set_margin_left(10);
	sep4.set_margin_right(10);
	
    m_HBox_status.pack_end(sep5,Gtk::PACK_SHRINK);//m_Separator
    m_HBox_status.pack_end(m_VBox_Col_az,Gtk::PACK_SHRINK);
    m_VBox_Col_alt.pack_start(lbl_alt,Gtk::PACK_SHRINK);
    m_VBox_Col_alt.pack_start(m_HBox_encoder_alt,Gtk::PACK_SHRINK);
	m_VBox_Col_alt.pack_start(m_HBox_degree_alt,Gtk::PACK_SHRINK);
    m_VBox_Col_alt.pack_start(m_HBox_enabled_alt,Gtk::PACK_SHRINK);
	m_VBox_Col_alt.pack_start(m_HBox_moving_alt,Gtk::PACK_SHRINK);
    
	m_VBox_Col_az.pack_start(lbl_az,Gtk::PACK_SHRINK);
    m_VBox_Col_az.pack_start(m_HBox_encoder_az,Gtk::PACK_SHRINK);
	m_VBox_Col_az.pack_start(m_HBox_degree_az,Gtk::PACK_SHRINK);
    m_VBox_Col_az.pack_start(m_HBox_enabled_az,Gtk::PACK_SHRINK);
    m_VBox_Col_az.pack_start(m_HBox_moving_az,Gtk::PACK_SHRINK);
	
	
	//moving / enabled status led
	m_HBox_enabled_az.pack_start(lbl_az_enabled,Gtk::PACK_SHRINK);
    m_HBox_enabled_az.pack_start(led_az_enable,Gtk::PACK_SHRINK);
    led_az_enable.deactivate();
	m_HBox_moving_az.pack_start(lbl_az_moving,Gtk::PACK_SHRINK);
	m_HBox_moving_az.pack_start(led_az_moving,Gtk::PACK_SHRINK);
	led_az_moving.deactivate();
	
	m_HBox_enabled_alt.pack_start(lbl_alt_enabled,Gtk::PACK_SHRINK);
    m_HBox_enabled_alt.pack_start(led_alt_enable,Gtk::PACK_SHRINK);
    led_alt_enable.deactivate();
	m_HBox_moving_alt.pack_start(lbl_alt_moving,Gtk::PACK_SHRINK);
	m_HBox_moving_alt.pack_start(led_alt_moving,Gtk::PACK_SHRINK);
	led_alt_moving.deactivate();
    
	//encoder / degree
	m_HBox_encoder_az.pack_start(lbl_az_encoder,Gtk::PACK_SHRINK);
    m_HBox_encoder_alt.pack_start(lbl_alt_encoder,Gtk::PACK_SHRINK);
    m_HBox_degree_az.pack_start(lbl_az_degree,Gtk::PACK_SHRINK);
    m_HBox_degree_alt.pack_start(lbl_alt_degree,Gtk::PACK_SHRINK);
	//::::::::::::::::::::::::::::::::::
    //:::   Set the Motors Movement  :::
	//:::::::::::::::::::::::::::::::::: 
	e_az.set_width_chars(6);
	e_alt.set_width_chars(6);
	m_HBox_move_label.pack_start(l_move_ctrl);
    m_HBox_move_ctrl.pack_start(l_move_alt,Gtk::PACK_SHRINK);
	m_HBox_move_ctrl.pack_start(e_alt,Gtk::PACK_SHRINK);
	m_HBox_move_ctrl.pack_start(l_move_az,Gtk::PACK_SHRINK);
	m_HBox_move_ctrl.pack_start(e_az,Gtk::PACK_SHRINK);
	
	m_HBox_move_ctrl.pack_end(move,Gtk::PACK_SHRINK);
    //:::::::::::::::::::::::::::::::::
	//:::   Set lim switch status   :::
	//:::::::::::::::::::::::::::::::::
	m_VBox_lim.pack_start(l_lim_header,Gtk::PACK_SHRINK);
	led_lim_switch.set_label("Limit Switch Active");
	m_VBox_lim.pack_start(led_lim_switch);
	m_VBox_lim.pack_start(m_HBox_lim_row1,Gtk::PACK_SHRINK);
	m_VBox_lim.pack_start(m_HBox_lim_row2,Gtk::PACK_SHRINK);
	m_VBox_lim.pack_start(m_HBox_lim_row3,Gtk::PACK_SHRINK);
	m_VBox_lim.pack_start(m_HBox_lim_row4,Gtk::PACK_SHRINK);
	//row #1
	m_HBox_lim_row1.pack_start(l_lim_lower,Gtk::PACK_SHRINK);
	m_HBox_lim_row1.pack_start(l_lim_lower_i,Gtk::PACK_SHRINK);
	m_HBox_lim_row1.pack_start(l_lim_upper,Gtk::PACK_SHRINK);
	m_HBox_lim_row1.pack_start(l_lim_upper_i,Gtk::PACK_SHRINK);
	//row #2
	m_HBox_lim_row2.pack_start(l_lim_left,Gtk::PACK_SHRINK);
	m_HBox_lim_row2.pack_start(l_lim_left_i,Gtk::PACK_SHRINK);
	m_HBox_lim_row2.pack_start(l_lim_right,Gtk::PACK_SHRINK);
	m_HBox_lim_row2.pack_start(l_lim_right_i,Gtk::PACK_SHRINK);
	//row #3
	m_HBox_lim_row3.pack_start(l_lim_alt_zero,Gtk::PACK_SHRINK);
	m_HBox_lim_row3.pack_start(l_lim_alt_zero_i,Gtk::PACK_SHRINK);
	m_HBox_lim_row3.pack_start(l_lim_az_zero,Gtk::PACK_SHRINK);
	m_HBox_lim_row3.pack_start(l_lim_az_zero_i,Gtk::PACK_SHRINK);
	//row #4
	m_HBox_lim_row4.pack_start(l_lim_launch,Gtk::PACK_SHRINK);
	m_HBox_lim_row4.pack_start(l_lim_launch_i,Gtk::PACK_SHRINK);
	
	l_lim_lower_i.set_width_chars(9);
	l_lim_upper_i.set_width_chars(9);
	l_lim_right_i.set_width_chars(9);
	l_lim_left_i.set_width_chars(9);
	l_lim_launch_i.set_width_chars(9);
	l_lim_az_zero_i.set_width_chars(9);
	l_lim_alt_zero_i.set_width_chars(9);
	l_lim_lower.set_width_chars(8);
	l_lim_upper.set_width_chars(8);
	l_lim_right.set_width_chars(8);
	l_lim_left.set_width_chars(8);
	l_lim_launch.set_width_chars(8);
	l_lim_az_zero.set_width_chars(8);
	l_lim_alt_zero.set_width_chars(8);
	Gdk::RGBA font_color;
    font_color.set_rgba(51/255.0, 204/255.0, 51/255.0,1);
	l_lim_upper_i.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
	l_lim_lower_i.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
	l_lim_left_i.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
	l_lim_right_i.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
	l_lim_launch_i.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
	l_lim_az_zero_i.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
	l_lim_alt_zero_i.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
	
    //::::::::::::  connect some signals ::::::::::::::::::
    m_connection_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,
              &MotorsWindow::p_bar), 500 );
    move.signal_clicked().connect( sigc::mem_fun(*this,&MotorsWindow::on_button_move));

    //::::::::::::::::::::::::::::::::::::::::::::::::::::::
  
    //Try to fetch (update) all the scripts name from py_manager. You can always 
    //use the update button if the connection was not established at startup.


    show_all_children();
	m_InfoBar.hide();
	//testing !!!
	
}

MotorsWindow::~MotorsWindow()
{
}
bool MotorsWindow::p_bar()
{
	
	if (motor_status==STARTED){
		timeout-=1;
		if (timeout<0)
		{
			motor_status = TIMEOUT;
		}
		m_ProgressBar.set_show_text(true);
		m_ProgressBar.pulse();
		}
	else if (motor_status==TIMEOUT)
	{
		timeout = 60;
		motor_status = STOPPED;
		//print error message
		print_message("A timeout has occurred. Manually execute the motors.py script and check the output.","[Warning !!!]");
		m_ProgressBar.set_show_text(false);
		m_ProgressBar.set_fraction(0);
	}
	else{
		m_ProgressBar.set_show_text(false);
		m_ProgressBar.set_fraction(0);
	}

	return true;
}
void MotorsWindow::set_az(int encoder,float degree)
/*
 * Set the the text for azimuth encoder field.
 */ 
{
	char buff[20];
	memset(buff,0,20);
	sprintf(buff,"°: %.2f",degree);
	lbl_az_degree.set_text(std::string(buff));
	memset(buff,0,20);
	sprintf(buff,"Encoder: %d",encoder);	
	lbl_az_encoder.set_text(std::string(buff));
}
void MotorsWindow::set_az(double degree)
/*
 * Set the the text for azimuth encoder field.
 */ 
{
	char buff[20];
	memset(buff,0,20);
	sprintf(buff,"°: %.2f",degree);
	lbl_az_degree.set_text(std::string(buff));
	int encoder=static_cast<int>(degree*10000.0);
	memset(buff,0,20);
	sprintf(buff,"Encoder: %d",encoder);	
	lbl_az_encoder.set_text(std::string(buff));
}
void MotorsWindow::set_alt(double degree)
/*
 * Set the the text for azimuth encoder field.
 */ 
{
	char buff[20];
	memset(buff,0,20);
	sprintf(buff,"°: %.2f",degree);
	lbl_alt_degree.set_text(std::string(buff));
	int encoder=static_cast<int>(degree*10000.0);
	memset(buff,0,20);
	sprintf(buff,"Encoder: %d",encoder);	
	lbl_alt_encoder.set_text(std::string(buff));
}
void MotorsWindow::set_alt(int encoder,float degree)
/*
 * Set the the text for altitude encoder field.
 */ 
{
	char buff[20];
	memset(buff,0,20);
	sprintf(buff,"°: %.2f",degree);
	lbl_alt_degree.set_text(std::string(buff));
	memset(buff,0,20);
	sprintf(buff,"Encoder: %d",encoder);	
	lbl_alt_encoder.set_text(std::string(buff));	
}
//void MotorsWindow::set_az_moving()
/*
 * Set the proper status for azimuth motor
 */ 
//{
//	Gdk::RGBA font_color;
 //   font_color.set_rgba(255/255.0, 0/255.0, 0/255.0,1);
 //   m_value_state_az.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
//	m_value_state_az.set_text("Moving");
//}
//void MotorsWindow::set_alt_moving()
/*
 * Set the proper status for altitude motor
 */ 
//{
//	Gdk::RGBA font_color;
//    font_color.set_rgba(255/255.0, 0/255.0, 0/255.0,1);
///    m_value_state_alt.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
//	m_value_state_alt.set_text("Moving");
//}
/*
void MotorsWindow::set_az_stopped()
/*
 * Set the proper status for azimuth motor
 */ 
//{
//	Gdk::RGBA font_color;
//    font_color.set_rgba(51/255.0, 204/255.0, 51/255.0,1);
//    m_value_state_az.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
//	m_value_state_az.set_text("Stopped");
//}
//void MotorsWindow::set_alt_stopped()
/*
 * Set the proper status for altitude motor
 */ 
//{
//	Gdk::RGBA font_color;
//    font_color.set_rgba(51/255.0, 204/255.0, 51/255.0,1);
 //   m_value_state_alt.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
//	m_value_state_alt.set_text("Stopped");
//}

void MotorsWindow::update_lim_switch(uint8_t compressed)
/*
 * Description
 * -----------
 * 	Given the compressed lim. switch status. Usually fetch 
 * 	by the get_status function, this function will trigger
 * 	the proper state for each individual switch.
 * 
 * Note:
 * -----
 * 	Make sure you cast compressed as uint8_t.
 */ 
{
	printf("Compressed: %u\n",compressed);
	
	if ((compressed & 1) ==1){
		l_lim_upper_i.set_text("Activated");
	}
	else{
		l_lim_upper_i.set_text("");
	}
	if ((compressed & 2) == 2 ){
		l_lim_lower_i.set_text("Activated");
	}
	else{
		l_lim_lower_i.set_text("");
	}
	if ((compressed & 4) == 4){
		l_lim_right_i.set_text("Activated");
	}
	else{
		l_lim_right_i.set_text("");
	}
	if ((compressed & 8) == 8){
		l_lim_left_i.set_text("Activated");
	}
	else{
		l_lim_left_i.set_text("");
	}
	if ((compressed & 16) == 16){
		l_lim_launch_i.set_text("Activated");
	}
	else{
		l_lim_launch_i.set_text("");
	}
	if ((compressed & 32) == 32){
		l_lim_az_zero_i.set_text("Activated");
	}
	else{
		l_lim_az_zero_i.set_text("");
	}
	if ((compressed & 64) == 64){
		l_lim_alt_zero_i.set_text("Activated");
	}
	else{
		l_lim_alt_zero_i.set_text("");
	}
	

}
bool MotorsWindow::HiCIBaS_get_status()
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
		update_lim_switch(shm_tel->shmp->limswitch);
		set_az(shm_tel->shmp->moteur_2,shm_tel->shmp->az);
		set_alt(shm_tel->shmp->moteur_1,shm_tel->shmp->alt);
		
		//:::::::::::::::::::::::::::::
		//::: limit switch status   :::
		//:::::::::::::::::::::::::::::
		
		if (shm_tel->shmp->lim_activte){led_lim_switch.activate();}
		else{led_lim_switch.activate_red();}
		
		//::::::::::::::::::::::::::::::::::::
		//:::   manage the motors status   :::
		//::::::::::::::::::::::::::::::::::::
		std::cout<<"Moving!!!!!: "<<shm_tel->shmp->alt_moving<<std::endl;
		if (shm_tel->shmp->alt_moving)
		{
			led_alt_moving.activate();
		}
		else{
			led_alt_moving.deactivate();
		}
		
		if (shm_tel->shmp->az_moving)
		{
			led_az_moving.activate();
		}
		else{
			led_az_moving.deactivate();
		}
		
		if (shm_tel->shmp->nutec_enable)
		{
			led_alt_enable.activate();
		}
		else{
			led_alt_enable.deactivate();
		}
		if (shm_tel->shmp->rm8_enable)
		{
			led_az_enable.activate();
		}
		else{
			led_az_enable.deactivate();
		}
				
		if (shm_tel->shmp->alt_moving || shm_tel->shmp->az_moving) {
			motor_status = MOVING;
		}
		else if (motor_status!=STARTED)
		{
			motor_status = STOPPED;
		}
		return true;
	}//end of local update
	
	//::::::::::::::::::::::::::::::::::::
	//:::   Remote Update via socket   :::
	//::::::::::::::::::::::::::::::::::::
	/*
	//get the lim. switch status
	std::string resp="";
	//fetch limit switch status
	int ret = snd_cmd("getstatus -lim",&resp,panel_configuration.tcpip,panel_configuration.socket_timeout);
	std::cout<<"[1] return: "<<ret<<", resp: "<<resp<<std::endl;
	if (ret==CONNECTION_P){
		display_disconnected();
		return true;
	}
	
	display_connected();
	if(ret!=OK || !isNumeric(resp)){
		return true;
	}
	update_lim_switch((uint8_t)std::atoi(resp.c_str()));
	//fetch motor encoder
	
	
	
	resp = "";
	ret = snd_cmd("getstatus -tcs",&resp,panel_configuration.tcpip,panel_configuration.socket_timeout);
	if (ret==CONNECTION_P){
		display_disconnected();
		return true;
	}
	if(ret!=OK){
		return true;
	}
	std::vector<std::string> ra_dec;
	ra_dec = split(resp,';');
	if (ra_dec.size()!=2){
		std::cout<<"Ouch!"<<std::endl;
		return true;
	}
	if (isNumeric(ra_dec[0]) && isNumeric(ra_dec[1])){
		set_az(std::stod(ra_dec[0].c_str()));
		set_alt(std::stod(ra_dec[1].c_str()));
	}
	
	
	ret = snd_cmd("getstatus -motors",&resp,panel_configuration.tcpip,panel_configuration.socket_timeout);
	

	if (ret==CONNECTION_P){
		display_disconnected();
		return true;
	}
	if(ret!=OK){
		return true;
	}
	if (isNumeric(resp))
	{
		uint8_t motors = static_cast<uint8_t>(std::atoi(resp.c_str()));
		if ((motors & 1) ==1){led_alt_moving.activate();}
		else{led_alt_moving.deactivate();}
		//<0> nutec moving, <1> rm8 moving,<2> nutec enable, <3> rm8 enable
		if ((motors  & 2) ==2){led_az_moving.activate();}
		else{led_az_moving.deactivate();}
		
		if ((motors & 4) ==4){led_alt_enable.activate();}
		else{led_alt_enable.deactivate();}
		//<0> nutec moving, <1> rm8 moving,<2> nutec enable, <3> rm8 enable
		if ((motors  & 8) ==8){led_az_enable.activate();}
		else{led_az_enable.deactivate();}
		
		
		//if ((motors & 128) ==128){led_lim_switch.activate();}
		//else{led_lim_switch.activate_red();}
		
		if ((motors & 1)==1 || (motors & 2 )==2) {
			motor_status = MOVING;
		}
		else if (motor_status!=STARTED)
		{
			motor_status = STOPPED;
		}
		return true;
	}
	 */ 
	return true;
}
std::vector<std::string> MotorsWindow::split(std::string txt,char sep)
{
	std::string buff="";
	std::vector<std::string> array{};
	for (auto &c: txt){
		if (c==sep){
			array.push_back(buff);
			buff="";
			continue;
			}
		buff+=c;
		} 
	if (buff.compare("")!=0){
		array.push_back(buff);
		}
	return array;
	
}
bool MotorsWindow::isNumeric(Glib::ustring number)
/*
 * Check if string is a float/int/double
 */ 
{
	
	std::string _number = std::string(number);
	for (auto &c: _number){
		if (!isdigit(c) && c!='.' && c!='-'){return false;}
	}
	return true;
}
bool MotorsWindow::isNumeric(std::string number)
/*
 * Check if string is a float/int/double
 */ 
{
	
	std::string _number = std::string(number);
	for (auto &c: _number){
		if (!isdigit(c) && c!='.' && c!='-'){return false;}
	}
	return true;
}
void MotorsWindow::on_button_move()
{
	
	Glib::ustring _alt = e_alt.get_text();
	Glib::ustring _az = e_az.get_text();
	

	if (!isNumeric(_alt) || !isNumeric(_az))
	{
		set_info_message("Wrong Argument");
		return ;
	}
	
	//erase the entry values.
	e_alt.set_text("");
	e_az.set_text("");
	double alt_d=0,az_d=0;
	if (_alt.compare("")==0){alt_d=0;}
	else{alt_d = std::atof(_alt.c_str());}
	if (_az.compare("")==0){az_d=0;}
	else{az_d = std::atof(_az.c_str());}
	//Make a move with the telescope

	int ret = move_telescope(alt_d,az_d);
	
	if (ret!=OK)
	{
		set_info_message("Failed Script");
	}
	else {
		motor_status = STARTED;
	}
}

int MotorsWindow::move_telescope(float alt,float az){
	std::string resp="";
	std::string cmd_rm8="",cmd_nutec="";
	cmd_rm8 = "move position "+std::to_string( static_cast<int>(az*10000.0)  );
	cmd_nutec = "move position "+std::to_string( static_cast<int>(alt*10000.0)  );

	int ret=0;
	//snd_cmd_ip(std::string cmd,std::string *value_returned,int port,int udp_port,std::string host,bool tcpip=true,int timeout=2)
	ret+= snd_cmd_ip(cmd_rm8,&resp,7565,7665,HiCIBaS_ip,panel_configuration.tcpip,panel_configuration.socket_timeout);
	ret+= snd_cmd_ip(cmd_nutec,&resp,7555,7655,HiCIBaS_ip,panel_configuration.tcpip,panel_configuration.socket_timeout);
	return ret;
}