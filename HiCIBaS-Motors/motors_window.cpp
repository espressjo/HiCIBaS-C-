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
abort("Abort"),
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
l_lim_lower("Lower: "),
l_lim_upper("Upper: "),
l_lim_right("Right: "),
l_lim_left("Left: "),
l_lim_launch("Launch: "),
l_lim_az_zero("Az-0: "),
l_lim_alt_zero("Alt-0: "),
rbtn_steps("steps"),
rbtn_degree("°"),
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
l_lim_header("Limit Switch status"),
cfg_motor_button(Gtk::Stock::HARDDISK)


  
{   //set the IP adress of HiCIBaS main software.
    //This should eventually be done from commandline or from the gui.
	motor_status = STOPPED;
	//Script to move the telescope.
	timeout = 60;
	//script = "/opt/HiCIBaS/python/moteurs.py";
	//ui_get_string("/opt/HiCIBaS/config/HiCIBaS.conf","MSCRIPT",&script);
	//look in the config file
	//printf("Using %s as the motor script\n",script.c_str());
	
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
	m_HBox_move.pack_start(abort,Gtk::PACK_SHRINK);

	m_VBox_Col_alt.set_margin_start(5);
    m_VBox_Col_az.set_margin_start(5);

toolbar.append(cfg_motor_button);

    Gtk::RadioButton::Group G1;
	rbtn_degree.set_group(G1);
    rbtn_steps.set_group(G1);
    rbtn_steps.set_active();
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
    
    m_VBox_Col_alt.set_margin_left(30);
    m_VBox_Col_az.set_margin_right(30);
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
	m_HBox_move_ctrl.pack_start(rbtn_steps,Gtk::PACK_SHRINK);
    m_HBox_move_ctrl.pack_start(rbtn_degree,Gtk::PACK_SHRINK);
	m_HBox_move_ctrl.pack_end(move,Gtk::PACK_SHRINK);
    //:::::::::::::::::::::::::::::::::
	//:::   Set lim switch status   :::
	//:::::::::::::::::::::::::::::::::
	m_VBox_lim.pack_start(l_lim_header,Gtk::PACK_SHRINK);
	m_VBox_lim.pack_start(m_HBox_lim_row1,Gtk::PACK_SHRINK);
	m_VBox_lim.pack_start(m_HBox_lim_row2,Gtk::PACK_SHRINK);
	m_VBox_lim.pack_start(m_HBox_lim_row3,Gtk::PACK_SHRINK);
	m_VBox_lim.pack_start(m_HBox_lim_row4,Gtk::PACK_SHRINK);
	//row #1
	m_HBox_lim_row1.pack_start(l_lim_lower,Gtk::PACK_SHRINK);
	m_HBox_lim_row1.pack_start(led_lower,Gtk::PACK_SHRINK);
	m_HBox_lim_row1.pack_start(l_lim_upper,Gtk::PACK_SHRINK);
	m_HBox_lim_row1.pack_start(led_upper,Gtk::PACK_SHRINK);
	//row #2
	m_HBox_lim_row2.pack_start(l_lim_left,Gtk::PACK_SHRINK);
	m_HBox_lim_row2.pack_start(led_left,Gtk::PACK_SHRINK);
	m_HBox_lim_row2.pack_start(l_lim_right,Gtk::PACK_SHRINK);
	m_HBox_lim_row2.pack_start(led_right,Gtk::PACK_SHRINK);
	//row #3
	m_HBox_lim_row3.pack_start(l_lim_alt_zero,Gtk::PACK_SHRINK);
	m_HBox_lim_row3.pack_start(led_alt_zero,Gtk::PACK_SHRINK);
	m_HBox_lim_row3.pack_start(l_lim_az_zero,Gtk::PACK_SHRINK);
	m_HBox_lim_row3.pack_start(led_az_zero,Gtk::PACK_SHRINK);
	//row #4
	m_HBox_lim_row4.pack_start(l_lim_launch,Gtk::PACK_SHRINK);
	m_HBox_lim_row4.pack_start(led_launch,Gtk::PACK_SHRINK);
	
	
	l_lim_lower.set_width_chars(8);
	l_lim_upper.set_width_chars(8);
	l_lim_right.set_width_chars(8);
	l_lim_left.set_width_chars(8);
	l_lim_launch.set_width_chars(8);
	l_lim_az_zero.set_width_chars(8);
	l_lim_alt_zero.set_width_chars(8);
	
	
    //::::::::::::  connect some signals ::::::::::::::::::
    //m_connection_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,
              //&MotorsWindow::p_bar), 500 );
    move.signal_clicked().connect( sigc::mem_fun(*this,&MotorsWindow::on_button_move));
    abort.signal_clicked().connect( sigc::mem_fun(*this,&MotorsWindow::on_button_abort));
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::
    cfg_motor_button.signal_clicked().connect( sigc::mem_fun(*this,&MotorsWindow::on_button_motor_config));
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

void MotorsWindow::on_button_motor_config()
{
    /*
    panel_configuration.polling_time=connection_status_timeout;
    panel_configuration.port=HiCIBaS_tcpip_port;
	panel_configuration.port_udp=HiCIBaS_udp_port;
    panel_configuration.ip=HiCIBaS_ip;
    panel_configuration.local = HiCIBaS_is_local;
    panel_configuration.tcpip=HiCIBaS_is_tcpip;
    panel_configuration.socket_timeout = HiCIBaS_socket_timeout;
	std::cout<<"Ici2: "<<panel_configuration.socket_timeout<<std::endl;
    */
    config_motor_t panel;
    std::string b_nutec_speed="",b_nutec_acc="",b_nutec_dec="",b_rm8_low_speed="",b_rm8_high_speed="",b_rm8_acc="";
    if (snd_cmd_ip("get_speed -rpm",&b_nutec_speed,7555,7655,panel_configuration.ip,true,900)!=OK)
    {
        set_info_message("unable to fetch info");
        return; 
    }
    if (snd_cmd_ip("get_acc -rps",&b_nutec_acc,7555,7655,panel_configuration.ip,true,900)!=OK)
    {
        set_info_message("unable to fetch info");
        return; 
    }
    if (snd_cmd_ip("get_dec -rps",&b_nutec_dec,7555,7655,panel_configuration.ip,true,900)!=OK)
    {
        set_info_message("unable to fetch info");
        return; 
    }
    if (snd_cmd_ip("get_low_speed",&b_rm8_low_speed,7565,7665,panel_configuration.ip,true,900)!=OK)
    {
        set_info_message("unable to fetch info");
        return; 
    }
    if (snd_cmd_ip("get_high_speed",&b_rm8_high_speed,7565,7665,panel_configuration.ip,true,900)!=OK)
    {
        set_info_message("unable to fetch info");
        return; 
    }
    if (snd_cmd_ip("get_acceleration",&b_rm8_acc,7565,7665,panel_configuration.ip,true,900)!=OK)
    {
        set_info_message("unable to fetch info");
        return; 
    }
    
    panel.nutec_acc=std::stod(b_nutec_acc.c_str());
    panel.nutec_speed=std::stod(b_nutec_speed.c_str());
    panel.nutec_dec=std::stod(b_nutec_dec.c_str());
    panel.rm8_acc = std::atoi(b_rm8_acc.c_str());
    panel.rm8_high_speed =std::atoi(b_rm8_high_speed.c_str());
    panel.rm8_low_speed = std::atoi(b_rm8_low_speed.c_str());
    ConfigMotor conf(&panel);
    conf.run();
    
    if (conf.canceled)
    {return;}
    
    string buff="";
    if (snd_cmd_ip("set_acceleration acceleration "+std::to_string(panel.rm8_acc),&buff,7565,7665,panel_configuration.ip,true,900)!=OK)
    {
        set_info_message("unable to set info 1");
        return; 
    }
    if (snd_cmd_ip("set_low_speed speed "+std::to_string(panel.rm8_low_speed),&buff,7565,7665,panel_configuration.ip,true,900)!=OK)
    {
        set_info_message("unable to set info 2");
        return; 
    }
    if (snd_cmd_ip("set_high_speed speed "+std::to_string(panel.rm8_high_speed),&buff,7565,7665,panel_configuration.ip,true,900)!=OK)
    {
        set_info_message("unable to set info 3");
        return; 
    }
    if (snd_cmd_ip("set_acc rps "+std::to_string(panel.nutec_acc),&buff,7555,7655,panel_configuration.ip,true,900)!=OK)
    {
        set_info_message("unable to set info 4");
        return; 
    }
    if (snd_cmd_ip("set_dec rps "+std::to_string(panel.nutec_dec),&buff,7555,7655,panel_configuration.ip,true,900)!=OK)
    {
        set_info_message("unable to set info 4");
        return; 
    }
    if (snd_cmd_ip("set_speed rpm "+std::to_string(panel.nutec_speed),&buff,7555,7655,panel_configuration.ip,true,900)!=OK)
    {
        set_info_message("unable to set info 5");
        return; 
    }

    
}


void MotorsWindow::set_az(int encoder)
/*
 * Set the the text for azimuth encoder field.
 */ 
{
	char buff[20];
	memset(buff,0,20);
	double degree=static_cast<double>(encoder)/10000.0;
	
	sprintf(buff,"°: %.2f",degree);
	lbl_az_degree.set_text(std::string(buff));
	memset(buff,0,20);
	sprintf(buff,"Encoder: %d",encoder);	
	lbl_az_encoder.set_text(std::string(buff));
}
void MotorsWindow::set_alt(int encoder)
/*
 * Set the the text for azimuth encoder field.
 */ 
{
	char buff[20];
	memset(buff,0,20);
	double degree=static_cast<double>(encoder)/10000.0;
	
	sprintf(buff,"°: %.2f",degree);
	lbl_alt_degree.set_text(std::string(buff));
	memset(buff,0,20);
	sprintf(buff,"Encoder: %d",encoder);	
	lbl_alt_encoder.set_text(std::string(buff));
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
        led_upper.activate_red();
		
	}
	else{
		led_upper.deactivate();
	}
	if ((compressed & 2) == 2 ){
		led_lower.activate_red();
	}
	else{
		led_lower.deactivate();
	}
	if ((compressed & 4) == 4){
		led_right.activate_red();//.set_text("Activated");
	}
	else{
		led_right.deactivate();
	}
	if ((compressed & 8) == 8){
		led_left.activate_red();
	}
	else{
		led_left.deactivate();
	}
	if ((compressed & 16) == 16){
		led_launch.activate();//.set_text("Activated");
	}
	else{
		led_launch.deactivate();
	}
	if ((compressed & 32) == 32){
		led_az_zero.activate();
	}
	else{
		led_az_zero.deactivate();
	}
	if ((compressed & 64) == 64){
		led_alt_zero.activate();
	}
	else{
		led_alt_zero.deactivate();
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
    bool remote=false;
    std::string buff="";
    int connected = 0;
    nutec_telemetry tlm_nutec{};// = new nutec_telemetry;
    rm8_telemetry tlm_rm8{};
    
    //:::::::::::::::::::::::::::::
    //:::   Remote Connection   :::
    //:::::::::::::::::::::::::::::
    
    if (!panel_configuration.local)
    {
        remote=true;
        
        if (snd_cmd_ip("status -get",&buff,7555,7655,panel_configuration.ip,true,900)!=OK)
        {
            connected++;
        }
        else {
            if (decode_nutec(buff,&tlm_nutec)!=0)
            {
                set_info_message("Failed to decode Nutec");
                connected++;
                return true;
            }
        }
                                                                                                                        buff="";
        if (snd_cmd_ip("status -get",&buff,7565,7665,panel_configuration.ip,true,900)!=OK)
        {
                connected++;
                
        }
        else {
            
            if (decode_rm8(buff,&tlm_rm8)!=0)
            {
                set_info_message("Failed to decode RM8");
                connected++;
                return true;
            }
        }
        //display connection status
        if (connected>0){display_disconnected();}
        else {display_connected();}        
                                                 
    }
    //::::::::::::::::::::::::::::
    //:::   local Connection   :::
    //::::::::::::::::::::::::::::
    else {
        remote=false;
        if (!shm_tel->shmp->connected)
		{
			display_disconnected();
			return true;
		}
		display_connected();
        
        
    }
    //::::::::::::::::::::::::::
    //:::   Update display   :::
    //::::::::::::::::::::::::::

    if (remote)
    {   //::::::::::::::::::::::::::::::
        //:::   manage lim. switch   :::
        //::::::::::::::::::::::::::::::
           
        if (tlm_nutec.lim_n){led_lower.activate_red();}
        else{led_lower.deactivate();}
        if (tlm_nutec.lim_p){led_upper.activate_red();}
        else{led_upper.deactivate();}
		if (tlm_nutec.home){led_alt_zero.activate_red();}
        else{led_alt_zero.deactivate();}
		
        if (tlm_rm8.lim_n){led_left.activate_red();}
        else{led_left.deactivate();}
        if (tlm_rm8.lim_p){led_right.activate_red();}
        else{led_right.deactivate();}
        if (tlm_rm8.lim_home){led_az_zero.activate();}
        else{led_az_zero.deactivate();}
        //::::::::::::::::::::::::::::::
        
        //:::::::::::::::::::::::::
        //:::   Motor status   ::::
        //:::::::::::::::::::::::::
        if (tlm_nutec.moving){led_alt_moving.activate();}
        else {led_alt_moving.deactivate();}
        if (tlm_rm8.moving){led_az_moving.activate();}
        else {led_az_moving.deactivate();}
        if (tlm_nutec.enabled){led_alt_enable.activate();}
        else {led_alt_enable.deactivate();}
        if (tlm_rm8.drive_enabled){led_az_enable.activate();}
        else {led_az_enable.deactivate();}
        //:::::::::::::::::::::::::
        
        //::::::::::::::::::::::::::::::
        //:::   Set motor position   :::
        //::::::::::::::::::::::::::::::
        set_az(tlm_rm8.position);
        set_alt(tlm_nutec.position);
        
        
        if (tlm_nutec.moving || tlm_rm8.moving) {
			motor_status = MOVING;
		}
		else if (motor_status!=STARTED)
		{
			motor_status = STOPPED;
		}
        return true;
    }
    
    else {
        update_lim_switch(shm_tel->shmp->limswitch);
        set_az(shm_tel->shmp->moteur_2,shm_tel->shmp->az);
		set_alt(shm_tel->shmp->moteur_1,shm_tel->shmp->alt);
        //::::::::::::::::::::::::::::::::::::
		//:::   manage the motors status   :::
		//::::::::::::::::::::::::::::::::::::
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
        
    }
    
    
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
void MotorsWindow::on_button_abort()
{
    std::string resp_rm8="",resp_nutec="";
    int ret=0;
    ret+= snd_cmd_ip("abort",&resp_rm8,7565,7665,HiCIBaS_ip,panel_configuration.tcpip,panel_configuration.socket_timeout);
	ret+= snd_cmd_ip("abort",&resp_nutec,7555,7655,HiCIBaS_ip,panel_configuration.tcpip,panel_configuration.socket_timeout);
	if (ret!=0)
    {
        print_message("Unable to stop one of the motors","[Alert!!!]");
    }
    return;
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
	//e_alt.set_text("");
	//e_az.set_text("");
    
	
	//Make a move with the telescope

	int ret = move_telescope(_alt,_az);
	
	if (ret!=OK)
	{
		set_info_message("Failed Script");
	}
	else {
		motor_status = STARTED;
	}
}

int MotorsWindow::move_telescope(Glib::ustring alt,Glib::ustring az){
	std::string resp="";
	std::string cmd_rm8="",cmd_nutec="";
    bool degree=rbtn_degree.get_active();
    double alt_double=0,az_double=0;
	if (degree)
    {
        if (alt.compare("")==0){alt_double=0;}
        else{alt_double = std::atof(alt.c_str());}
        if (az.compare("")==0){az_double=0;}
        else{az_double = std::atof(az.c_str());}
        cmd_rm8 = "move position "+std::to_string( static_cast<int>(az_double*10000.0)  );
        cmd_nutec = "move position "+std::to_string( static_cast<int>(alt_double*10000.0)  );
    }
    else 
    {
        cmd_rm8 = "move position "+az  ;
        cmd_nutec = "move position "+alt  ;
    }
    
	int ret=0;
	//snd_cmd_ip(std::string cmd,std::string *value_returned,int port,int udp_port,std::string host,bool tcpip=true,int timeout=2)
	ret+= snd_cmd_ip(cmd_rm8,&resp,7565,7665,HiCIBaS_ip,panel_configuration.tcpip,panel_configuration.socket_timeout);
	ret+= snd_cmd_ip(cmd_nutec,&resp,7555,7655,HiCIBaS_ip,panel_configuration.tcpip,panel_configuration.socket_timeout);
	return ret;
}


string double_to_string(double n)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << n;
    return stream.str();
}

ConfigMotor::ConfigMotor(config_motor_t *cfg_t)
:l_nutec_speed("Nutec Speed (RPM): "),
l_nutec_acc("Nutec Acc. (RPS): "),
l_nutec_dec("Nutec Dec. (RPS): "),
l_rm8_low_speed("RM8 Low speed (steps): "),
l_rm8_acc("RM8 Acc. (steps): "),
l_rm8_high_speed("RM8 High speed (steps): "), 
m_button_set("Set"),
m_button_cancel("Cancel")
{
    set_title("Motor Config Panel");
    set_border_width(5);
    set_default_size(300, 100);
    ConfigMotor::cfg_t = cfg_t;
    
    Gtk::Box *vbox = get_vbox();
    canceled=false;
    vbox->pack_end(m_HBox_buttons,Gtk::PACK_SHRINK);
    m_HBox_buttons.pack_start(m_button_set,Gtk::PACK_SHRINK);
    m_HBox_buttons.pack_start(m_button_cancel,Gtk::PACK_SHRINK);
    vbox->pack_start(m_HBox_settings,Gtk::PACK_SHRINK);
    
    m_HBox_settings.pack_start(v_nutec,Gtk::PACK_SHRINK);
    m_HBox_settings.pack_start(v_rm8,Gtk::PACK_SHRINK);
    v_nutec.pack_start(h_nutec_1,Gtk::PACK_SHRINK);
    v_nutec.pack_start(h_nutec_2,Gtk::PACK_SHRINK);
    v_nutec.pack_start(h_nutec_3,Gtk::PACK_SHRINK);
    v_rm8.pack_start(h_rm8_1,Gtk::PACK_SHRINK);
    v_rm8.pack_start(h_rm8_2,Gtk::PACK_SHRINK);
    v_rm8.pack_start(h_rm8_3,Gtk::PACK_SHRINK);
    v_nutec.set_margin_right(10);
    h_nutec_1.pack_start(l_nutec_speed,Gtk::PACK_SHRINK);
    h_nutec_1.pack_end(e_nutec_speed,Gtk::PACK_SHRINK);
    h_nutec_2.pack_start(l_nutec_acc,Gtk::PACK_SHRINK);
    h_nutec_2.pack_end(e_nutec_acc,Gtk::PACK_SHRINK);
    h_nutec_3.pack_start(l_nutec_dec,Gtk::PACK_SHRINK);
    h_nutec_3.pack_end(e_nutec_dec,Gtk::PACK_SHRINK);
    h_rm8_1.pack_start(l_rm8_low_speed,Gtk::PACK_SHRINK);
    h_rm8_1.pack_end(e_rm8_low_speed,Gtk::PACK_SHRINK);
    h_rm8_2.pack_start(l_rm8_high_speed,Gtk::PACK_SHRINK);
    h_rm8_2.pack_end(e_rm8_high_speed,Gtk::PACK_SHRINK);
    h_rm8_3.pack_start(l_rm8_acc,Gtk::PACK_SHRINK);
    h_rm8_3.pack_end(e_rm8_acc,Gtk::PACK_SHRINK);
    
   
    e_rm8_low_speed.set_width_chars(7);
    e_rm8_high_speed.set_width_chars(7);
    e_rm8_acc.set_width_chars(7);
    e_nutec_speed.set_width_chars(7);
    e_nutec_acc.set_width_chars(7);
    e_nutec_dec.set_width_chars(7);
    
    //::::::::::::::::::::::::::::::::::::
    //:::  set the received values    ::::
    //::::::::::::::::::::::::::::::::::::
    e_nutec_dec.set_text( double_to_string(cfg_t->nutec_dec) );
    e_nutec_acc.set_text( double_to_string(cfg_t->nutec_acc) );
    e_nutec_speed.set_text( double_to_string(cfg_t->nutec_speed));
    e_rm8_acc.set_text( std::to_string(cfg_t->rm8_acc));
    e_rm8_high_speed.set_text( std::to_string(cfg_t->rm8_high_speed));
    e_rm8_low_speed.set_text( std::to_string(cfg_t->rm8_low_speed));
    //[...]
    
    m_button_set.signal_clicked().connect( sigc::mem_fun(*this,&ConfigMotor::on_button_set));
    m_button_cancel.signal_clicked().connect( sigc::mem_fun(*this,&ConfigMotor::on_button_cancel));

    show_all_children();
}

void ConfigMotor::on_button_set()
{
 
    cfg_t->nutec_speed = std::stod(e_nutec_speed.get_text().c_str());
    cfg_t->nutec_acc= std::stod(e_nutec_acc.get_text().c_str());
    cfg_t->nutec_dec= std::stod(e_nutec_dec.get_text().c_str());
    cfg_t->rm8_low_speed= std::atoi(e_rm8_low_speed.get_text().c_str());
    cfg_t->rm8_high_speed= std::atoi(e_rm8_high_speed.get_text().c_str());
    cfg_t->rm8_acc= std::atoi(e_rm8_acc.get_text().c_str());

    ConfigMotor::close();
}
void ConfigMotor::on_button_cancel()
{
    canceled=true;
    ConfigMotor::close();
}

ConfigMotor::~ConfigMotor()
{}
