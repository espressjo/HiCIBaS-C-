#include <iostream>
#include "scriptwindow.h"
#include <vector>

MainWindow::MainWindow()
: m_HButtonBox(Gtk::ORIENTATION_HORIZONTAL),
  m_Button_Kill("Kill"),
  m_Button_Run("Run"),
  m_Button_Read("Read"),
  m_Button_update_script("Update")
  //m_VBox(Gtk::ORIENTATION_VERTICAL)
{   
	//set the IP adress of HiCIBaS main software.
    //This should eventually be done from commandline or from the gui.
    HiCIBaS_ip="localhost";
	std::cout<<"Socket timeout: "<<socket_timeout<<std::endl;
	std::cout<<"TCP/IP port: "<<HiCIBaS_is_tcpip<<std::endl;
    //set the main window attributes
    set_title("HiCIBaS Script Manager");
    set_border_width(5);
    set_default_size(600, 200);
    //add(m_VBox);//add m_VBox inside the window
    m_VBox = get_box();
    //:::   Put the treeview inside a scrolledWindow widget.  :::         
    //Add the TreeView, inside a ScrolledWindow, with the button underneath:
    m_ScrolledWindow.add(m_TreeView);
    //Only show the scrollbars when they are necessary:
    m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    //Create the Tree model:
    m_refTreeModel = Gtk::ListStore::create(m_Columns);
    m_TreeView.set_model(m_refTreeModel);
        //Add the TreeView's view columns:
    //This number will be shown with the default numeric formatting.
    m_TreeView.append_column("PID", m_Columns.m_col_id);
    m_TreeView.append_column("Script", m_Columns.m_col_script_name);
    m_TreeView.append_column("Status", m_Columns.m_col_status);
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //:::::::: Pack the widgets inside m_VBox :::::::::
    m_VBox->pack_start(m_ScrolledWindow);
    m_VBox->pack_start(m_ButtonBox, Gtk::PACK_SHRINK);
    m_VBox->pack_start(statusBar,Gtk::PACK_SHRINK);
    
    m_ButtonBox.pack_start(m_HButtonBox);  
    m_HButtonBox.pack_start(m_Button_Run, Gtk::PACK_SHRINK);
    m_HButtonBox.pack_start(m_Button_Read, Gtk::PACK_SHRINK);
    m_HButtonBox.pack_start(m_Button_Kill, Gtk::PACK_SHRINK);
    m_HButtonBox.pack_start(m_Button_update_script, Gtk::PACK_SHRINK);
    m_HButtonBox.set_border_width(10);
    m_HButtonBox.set_layout(Gtk::BUTTONBOX_END);
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
    
    
    //::::::::::::  connect some signals ::::::::::::::::::
    //m_connection_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,
    //          &MainWindow::status), 1000 );
    m_Button_Kill.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_button_kill));
    m_Button_Read.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_button_read));
    m_Button_Run.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_button_run));
    m_Button_update_script.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_button_update));
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::
  
    //Try to fetch (update) all the scripts name from py_manager. You can always 
    //use the update button if the connection was not established at startup.
    update_treeview(HiCIBaS_ip);
    //::::::::::::::::::::::::::::::::::::::::
	//:::   fetch the python config file   :::
	//::::::::::::::::::::::::::::::::::::::::
	
	
	if (getConf("/opt/HiCIBaS/config/scripts.txt",&my_py_conf)!=0)
	{
		std::cout<<"Problem with the python script config file."<<std::endl;
		std::cout<<"Do not use the local mode"<<std::endl;	
	}
	
	
	
    show_all_children();
	m_InfoBar.hide();
}

MainWindow::~MainWindow()
{
}
void MainWindow::on_button_update(){
/*
 * Description
 * -----------
 *      We will try to establish a connection with
 *      HiCIBaS's server to download all the scripts
 *      name. If successfull, the treeview will be 
 *      updated with those names. 
 */ 
    update_treeview(HiCIBaS_ip);    
}
std::string MainWindow::get_selected_script_name()
/*
 * Description
 * -----------
 *      This function will return the name of the 
 *      script which is currently selected in the 
 *      treeview. 
 * Return
 * ------
 *      script name <string> if an element of the 
 *      treeview is selected, otherwise an empty string
 *      will be returned.
 */ 
{
    std::string script="";
    Glib::RefPtr<Gtk::TreeSelection> selection = m_TreeView.get_selection();
    
    Gtk::TreeModel::iterator selectedRow = selection->get_selected();
    
    Gtk::TreeModel::Row row = *selectedRow;
    
    Glib::ustring script_name = row.get_value(m_Columns.m_col_script_name);
    script = script_name;
    
    
    return script;
}
void MainWindow::show_std(Glib::ustring script_name)
{
  //get stdout and stderr
	std::string stdout="",stderr="";
	if (snd_cmd("python script "+script_name+" -stdout_no_n",&stdout,panel_configuration.tcpip,panel_configuration.socket_timeout)!=OK){
		set_info_message("Unable to connect to server");
		return;
	}
	if (
	snd_cmd("python script "+script_name+" -stderr_no_n",&stderr,panel_configuration.tcpip,panel_configuration.socket_timeout)!=OK)
	{set_info_message("Unable to connect to server");
	return;}

    std::string n_stdout="",n_stderr="";
    for (auto &c:stdout)
    {
        if (c==';'){n_stdout+='\n';}
        else{n_stdout+=c;}
    }
    for (auto &c:stderr)
    {
        if (c==';'){n_stderr+='\n';}
        else{n_stderr+=c;}
    }
  
  //sock.closeSocket();
  m_pDialog.reset(new Gtk::MessageDialog(*this, "[stdout / stderr] "+script_name));
  m_pDialog->set_secondary_text("stdout:\n"+n_stdout+"\nstderr:\n"+n_stderr);
  
  m_pDialog->set_modal(true);
 // m_pDialog->set_hide_on_close(true);
  m_pDialog->signal_response().connect(
    sigc::hide(sigc::mem_fun(*m_pDialog, &Gtk::Widget::hide)));

  m_pDialog->show();
}
void MainWindow::update_treeview(std::string ip)
/*
 * Description
 * -----------
 *  Update the tree view from the server. 
 */ 
{
    /*
    std::cout<<"Trying to connect..."<<std::endl;
    socket_ sock(ip,5555,2);//crank up the timeout to 5sec
    if (sock.status!=0)
    {//not connected
        std::cout<<"Unable to connect to HiCIBaS"<<std::endl;
        return;
    }
  sock.readSocket();//read the welcome msg.
  sock.writeSocket("python -get_avail_script");
  std::string avail_script =sock.readSocket();
  //check if Ok or NOK
  avail_script = avail_script.substr(3,avail_script.length());//-1 to remove the \n
  std::cout<<avail_script<<std::endl;
  sock.closeSocket();
  */
    std::vector<std::string> scripts;
    std::string buff="";
    Gtk::TreeModel::Row row;
    std::string avail_script="";
    if (snd_cmd("python -get_avail_script",&avail_script,panel_configuration.tcpip,panel_configuration.socket_timeout)!=OK)
    {
		set_info_message("Unable to connect to server");
        return;  
    }
  
    scripts = split_semi_colon(avail_script);
  
  
  
  m_refTreeModel->clear();
  for (auto sc:scripts)
  {   
      row = *(m_refTreeModel->append());
      row[m_Columns.m_col_id] = 0;
      row[m_Columns.m_col_script_name] = sc;
      row[m_Columns.m_col_status] = "--";
  }
}
bool MainWindow::HiCIBaS_get_status()
/*
 * Description
 * -----------
 *      Get status which fetch which script is running or stopped.
 *      It also display the connected or disconnect label in the 
 *      status bar. 
 * 		There is two protocole for the status update; local and 
 * 		remote. Local uses the shared memory created by the telemetry
 *  	UI. The remote protocol use UDP/TCPIP protocol to fetch
 * 		the scripts status.
 * 
 */ 
{
    std::vector<std::string> script_r;//running script
    std::vector<std::string> script_s;//stopped script
    std::string tmp_running_script="",tmp_stopped_script="";
	std::vector<std::string> scripts_stop,scripts_run;
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//:::   We will probe the shared memory and not the server   :::
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if (panel_configuration.local)
	{
		if (!shm_tel->shmp->connected)
		{
			display_disconnected();
			return true;
		}
		else{
			display_connected();
		}

		decode_scripts(shm_tel->shmp->r_scripts,&scripts_run,my_py_conf);
		decode_scripts(shm_tel->shmp->s_scripts,&scripts_stop,my_py_conf);

		for (auto s : scripts_run)
		{
			set_running(s);
		}

		for (auto s : scripts_stop)
		{
			set_stopped(s);
		}
	}//if
	
	//:::   END   :::
	else {	
	
		//::::::::::::::::::::::::::::::::::::
		//:::   Case we probe the server   :::
		//::::::::::::::::::::::::::::::::::::
			
		//fetch the list of script that are running and stopped
		
		if (snd_cmd("python -whos_running",&tmp_running_script,panel_configuration.tcpip,panel_configuration.socket_timeout)!=OK)
		{	//if we don't have connection return
			display_disconnected();
			return true;
		}
		if (snd_cmd("python -whos_stopped",&tmp_stopped_script,panel_configuration.tcpip,panel_configuration.socket_timeout)!=OK)
		{	//if we don't have connection return
			display_disconnected();
			return true;
		}
		else {
			display_connected();														
		}
		
		//We expect to receive a long string separated by ; and terminate by a new line
		//e.g., script1.py;script2.py;script3.py; ...\n
																														
		script_r = split_semi_colon(tmp_running_script);
		script_s = split_semi_colon(tmp_stopped_script);
		
		//For each script, tag them in the tree view.
		for (auto script:script_r)
		{
			set_running(script);
		}
		for (auto script:script_s)
		{
			set_stopped(script);
		}
	}//else
    
	return true;    
}
void MainWindow::on_button_kill()
{
	std::string value="";
    Glib::RefPtr<Gtk::TreeSelection> selection = m_TreeView.get_selection();
    Gtk::TreeModel::iterator selectedRow = selection->get_selected();
    Gtk::TreeModel::Row row = *selectedRow;
    Glib::ustring script_name = row.get_value(m_Columns.m_col_script_name);
    std::cout<<script_name<<std::endl;
	if (panel_configuration.tcpip){
		socket_ sock(panel_configuration.ip,panel_configuration.port,panel_configuration.socket_timeout);//crank up the timeout to 5sec
		if (sock.status!=0){return ;}
		sock.readSocket();//read the welcome msg.
		sock.writeSocket("python script "+script_name+" -kill");
		sock.readSocket();
		sock.closeSocket();
	}
	else{
		if (snd_cmd("python script "+script_name+" -kill",&value,panel_configuration.tcpip,panel_configuration.socket_timeout)!=OK)
		{
			set_info_message("Unable to connect to server");
		}
	}
	
}
void MainWindow::on_button_read()
{
   
    Glib::RefPtr<Gtk::TreeSelection> selection = m_TreeView.get_selection();
    Gtk::TreeModel::iterator selectedRow = selection->get_selected();
    Gtk::TreeModel::Row row = *selectedRow;
    Glib::ustring script_name = row.get_value(m_Columns.m_col_script_name);
    //std::cout<<script_name<<std::endl;
    //set_pid(script_name,14763);
    //set_none(script_name);
    //read the program script_name and show stdout and stderr
    show_std(script_name);
}

void MainWindow::on_button_run()
{
    Glib::RefPtr<Gtk::TreeSelection> selection = m_TreeView.get_selection();
    Gtk::TreeModel::iterator selectedRow = selection->get_selected();
    Gtk::TreeModel::Row row = *selectedRow;
    Glib::ustring script_name = row.get_value(m_Columns.m_col_script_name);
    std::string pid="";
	if (panel_configuration.tcpip){
		socket_ sock(panel_configuration.ip,panel_configuration.port,panel_configuration.socket_timeout);//crank up the timeout to 5sec
		if (sock.status!=0){return;}
		sock.readSocket();//read the welcome msg.
		sock.writeSocket("python script "+script_name+" -run");
		std::string ret =sock.readSocket();
		
		//sleep(1);
		sock.writeSocket("python script "+script_name+" -pid");
		std::string pid =sock.readSocket();
		pid = pid.substr(3,pid.length());
		sock.closeSocket();
	}
	else{
		std::string rcv="";
		if (snd_cmd("python script "+script_name+" -run",&rcv,panel_configuration.tcpip,panel_configuration.socket_timeout)!=OK)
		{
			std::cout<<"error message"<<std::endl;
			set_info_message("Unable to connect to server");
			return;
		}//
		if (snd_cmd("python script "+script_name+" -pid",&rcv,panel_configuration.tcpip,panel_configuration.socket_timeout)!=OK)
		{
			std::cout<<"error message"<<std::endl;
			return;
		}
	}
	
	std::string b_pid="";
	for (auto &c: pid)
	{
		if (c==' ' || c=='\n'){continue;}
		b_pid+=c;
	}
	if (b_pid.compare("")!=0){
		set_pid(script_name,std::atoi(b_pid.c_str()));
		}
	else {set_pid(script_name,0);}
	
	set_running(script_name);

}

void MainWindow::set_running(Glib::ustring script){
    Glib::ustring name="";
    for (auto c: m_refTreeModel->children())
    {
        name = c[m_Columns.m_col_script_name];
        if (name.compare(script)==0)
        {
            c[m_Columns.m_col_status] = "Running ...";
        }
        
    }
}
void MainWindow::set_stopped(Glib::ustring script){
    Glib::ustring name="";
    for (auto c: m_refTreeModel->children())
    {
        name = c[m_Columns.m_col_script_name];
        if (name.compare(script)==0)
        {
            c[m_Columns.m_col_status] = "Stopped";
        }
        
    }
}
void MainWindow::set_none(Glib::ustring script){
    Glib::ustring name="";
    for (auto c: m_refTreeModel->children())
    {
        name = c[m_Columns.m_col_script_name];
        if (name.compare(script)==0)
        {
            c[m_Columns.m_col_status] = "N/A";
        }
    }
}
void MainWindow::set_pid(Glib::ustring script,int pid){
    Glib::ustring name="";
    for (auto c: m_refTreeModel->children())
    {
        name = c[m_Columns.m_col_script_name];
        if (name.compare(script)==0)
        {
            c[m_Columns.m_col_id] = pid;
        }
    }
}
