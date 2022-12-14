#include <iostream>
#include "scriptwindow.h"
#include <vector>

MainWindow::MainWindow()
: m_VBox(Gtk::ORIENTATION_VERTICAL),
  m_HButtonBox(Gtk::ORIENTATION_HORIZONTAL),
  m_Button_Kill("Kill"),
  m_Button_Run("Run"),
  m_Button_Read("Read"),
  m_Button_update_script("Update")
  
{   //set the IP adress of HiCIBaS main software.
    //This should eventually be done from commandline or from the gui.
    HiCIBaS_ip="localhost";
    
    //set the main window attributes
    set_title("HiCIBaS Script Manager");
    set_border_width(5);
    set_default_size(600, 200);
    add(m_VBox);//add m_VBox inside the window
    
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
    m_VBox.pack_start(m_ScrolledWindow);
    m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);
    m_VBox.pack_start(statusBar,Gtk::PACK_SHRINK);
    
    m_ButtonBox.pack_start(m_HButtonBox);  
    m_HButtonBox.pack_start(m_Button_Run, Gtk::PACK_SHRINK);
    m_HButtonBox.pack_start(m_Button_Read, Gtk::PACK_SHRINK);
    m_HButtonBox.pack_start(m_Button_Kill, Gtk::PACK_SHRINK);
    m_HButtonBox.pack_start(m_Button_update_script, Gtk::PACK_SHRINK);
    m_HButtonBox.set_border_width(10);
    m_HButtonBox.set_layout(Gtk::BUTTONBOX_END);
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
    
    
    //::::::::::::  connect some signals ::::::::::::::::::
    m_connection_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,
              &MainWindow::status), 1000 );
    m_Button_Kill.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_button_kill));
    m_Button_Read.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_button_read));
    m_Button_Run.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_button_run));
    m_Button_update_script.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_button_update));
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::
  
    //Try to fetch (update) all the scripts name from py_manager. You can always 
    //use the update button if the connection was not established at startup.
    update_treeview(HiCIBaS_ip);

    show_all_children();
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
 
  
    socket_ sock(HiCIBaS_ip,5555,2);//crank up the timeout to 5sec
    if (sock.status!=0)
    {//not connected
        std::cout<<"Unable to connect to HiCIBaS"<<std::endl;
        return;
    }
    sock.readSocket();//read the welcome msg.
    sock.writeSocket("python script "+script_name+" -stdout_no_n");
    std::string stdout = sock.readSocket();
    sock.writeSocket("python script "+script_name+" -stderr_no_n");
    std::string stderr = sock.readSocket();
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
  
  sock.closeSocket();
  m_pDialog.reset(new Gtk::MessageDialog(*this, "[stdout / stderr] "+script_name));
  m_pDialog->set_secondary_text("stdout:\n"+n_stdout+"\nstderr:\n"+n_stderr);
  
  m_pDialog->set_modal(true);
 // m_pDialog->set_hide_on_close(true);
  m_pDialog->signal_response().connect(
    sigc::hide(sigc::mem_fun(*m_pDialog, &Gtk::Widget::hide)));

  m_pDialog->show();
}
void MainWindow::update_treeview(std::string ip)
{
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
  
  std::vector<std::string> scripts;
  std::string buff="";
  Gtk::TreeModel::Row row;
  for (auto &c:avail_script)
  {
    if (c==';'){scripts.push_back(buff);buff="";continue;}
    if (c=='\n'){continue;}
    buff+=c;    
  }
  scripts.push_back(buff);//last script
  m_refTreeModel->clear();
  for (auto sc:scripts)
  {   
      row = *(m_refTreeModel->append());
      row[m_Columns.m_col_id] = 0;
      row[m_Columns.m_col_script_name] = sc;
      row[m_Columns.m_col_status] = "--";
  }
}
bool MainWindow::status()
{
  socket_ sock(HiCIBaS_ip,5555,12);//crank up the timeout to 5sec
  if (sock.status!=0){
    font_color.set_rgba(153/255.0, 26/255.0, 3/255.0,1);
    statusBar.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
    statusBar.push("Not Connected");    
      
      return true;}
  if (status_bar_flag==0){
      font_color.set_rgba(28/255.0, 150/255.0, 32/255.0,0.9);
      statusBar.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
    statusBar.push("Connected");
    status_bar_flag=1;
    std::cout<<"green"<<std::endl;
  }
  else{
      std::cout<<"black"<<std::endl;
      font_color.set_rgba(72/255.0, 74/255.0, 73/255.0,0.9);
            statusBar.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);

      statusBar.push("Connected");
      status_bar_flag=0;
      }
  //statusBar.get
  sock.readSocket();//read the welcome msg.
  sock.writeSocket("python -whos_running");
  std::string running_script =sock.readSocket();
  //check if Ok or NOK
  sock.writeSocket("python -whos_stopped");
  std::string stopped_script =sock.readSocket();
  running_script = running_script.substr(3,running_script.length());//-1 to remove the \n
  stopped_script = stopped_script.substr(3,stopped_script.length());//-1 to remove the \n
  std::cout<<"running_script: "<<running_script<<std::endl;
  std::cout<<"stopped_script: "<<stopped_script<<std::endl;

  sock.closeSocket();
  
  std::vector<std::string> script_r;
  std::vector<std::string> script_s;
  std::string buff="";
  for (auto &c:running_script)
  {
    if (c==';'){script_r.push_back(buff);buff="";continue;}
    if (c=='\n'){continue;}
    buff+=c;    
  }
  script_r.push_back(buff);//last script
  
  buff="";
  for (auto &c:stopped_script)
  {
    if (c==';'){script_s.push_back(buff);buff="";continue;}
    if (c=='\n'){continue;}
    buff+=c;    
  }
  script_s.push_back(buff);//last script
  
  for (auto script:script_r)
  {
      set_running(script);
  }
  for (auto script:script_s)
  {
      set_stopped(script);
  }
  
  
    
    return true;}
void MainWindow::on_button_kill()
{
       
    Glib::RefPtr<Gtk::TreeSelection> selection = m_TreeView.get_selection();
    Gtk::TreeModel::iterator selectedRow = selection->get_selected();
    Gtk::TreeModel::Row row = *selectedRow;
    Glib::ustring script_name = row.get_value(m_Columns.m_col_script_name);
    std::cout<<script_name<<std::endl;
    socket_ sock(HiCIBaS_ip,5555,12);//crank up the timeout to 5sec
    if (sock.status!=0){return ;}
    sock.readSocket();//read the welcome msg.
    sock.writeSocket("python script "+script_name+" -kill");
    sock.readSocket();
    sock.closeSocket();
    //kill the program script_name and show stdout and stderr
    std::cout<<"Kill "<<script_name<<std::endl;
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
    std::cout<<"Read signal: "<<script_name<<std::endl;
}

void MainWindow::on_button_run()
{
    Glib::RefPtr<Gtk::TreeSelection> selection = m_TreeView.get_selection();
    Gtk::TreeModel::iterator selectedRow = selection->get_selected();
    Gtk::TreeModel::Row row = *selectedRow;
    Glib::ustring script_name = row.get_value(m_Columns.m_col_script_name);
    
    
    socket_ sock(HiCIBaS_ip,5555,12);//crank up the timeout to 5sec
    if (sock.status!=0){return;}
    sock.readSocket();//read the welcome msg.
    sock.writeSocket("python script "+script_name+" -run");
    std::string ret =sock.readSocket();
    
    //sleep(1);
    sock.writeSocket("python script "+script_name+" -pid");
    std::string pid =sock.readSocket();
    pid = pid.substr(3,pid.length());
    sock.closeSocket();
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

    //run the program script_name and show stdout and stderr
   
  
    
    
    std::cout<<"Run :"<<script_name<<std::endl;
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
