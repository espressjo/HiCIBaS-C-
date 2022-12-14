#include <iostream>
#include "mainwindow.h"
#include <vector>

MainWindow::MainWindow()
: HiCIBaS_connection("localhost",5555) ,
m_VBox(Gtk::ORIENTATION_VERTICAL),
cfg_button(Gtk::Stock::PREFERENCES)
/*
 * Description 
 * -----------
 *      This is a minimaly configured window for HiCIBaS. The window
 *      has a toolbar named toolbar, a status bar name statusBar and 
 *      the main container is name m_VBox. Use the method get_vbox, 
 *      get_toolbar and get_statusbar to add/change anything to the 
 *      example window i.e., Gtk::Box *vbox = mainWindow.get_vbox();
 * Configuration
 * -------------
 *      Each new window will inherit the config toolbar button. Clicking
 *      this button will show a new window with several configuration 
 *      options. Use the config_t panel_configuration to retrieve the 
 *      new values.
 * Server Communication
 * --------------------
 *      A utility method snd_cmd as been implemented. It will connect
 *      to the server, do the handshake, write a command and read the 
 *      output until the 1st \n is encoutered.
 */ 
{   
    //::::::::::::::::::::::::::::::::::::::
    //::: set the main window attributes :::
    //::::::::::::::::::::::::::::::::::::::
    set_title("HiCIBaS Main Window");
    set_border_width(5);
    set_default_size(600, 200);
    add(m_VBox);//add m_VBox inside the window
    
    //::::::::::::::::::::::::::::::::::::
    //::: Set the serve default values :::
    //::::::::::::::::::::::::::::::::::::
    socket_timeout = 3;//set the derived HiCIBaS_connection timeout for socket communication.
    HiCIBaS_port = 5555;
    HiCIBaS_ip="localhost";
    HiCIBaS_is_tcpip = true;
    HiCIBaS_is_local = false;
    connection_status_timeout = 1000;
    //:::::::::::::::::::::::::::::::::::::::::::::
    //::: Set the default HiCIBaS window widget :::
    //:::::::::::::::::::::::::::::::::::::::::::::
    m_VBox.pack_start(toolbar,Gtk::PACK_SHRINK);//every window will have a server config button
    m_VBox.pack_start(separator,Gtk::PACK_SHRINK);
    m_VBox.pack_end(statusBar,Gtk::PACK_SHRINK);//every window will have a status bar 
    toolbar.append(cfg_button);//every window will have a tool bar
    
    //::::::::::::::::::::::::::::
    //::: connect some signals :::
    //::::::::::::::::::::::::::::
    cfg_button.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_button_config));
        
    //::::::::::::::::::::::::::::::::::
    //::: connect the signal timeout :::
    //::::::::::::::::::::::::::::::::::
    m_connection_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,
              &MainWindow::get_status), connection_status_timeout );

    show_all_children();
}
Gtk::Box* MainWindow::get_box()
/*
 * Description
 * -----------
 *      return the address of the main box container.
 *      Use this method to pack new widget into the main
 *      window.
 */ 
{
    return &m_VBox;
}
Gtk::Toolbar* MainWindow::get_toolbar()
/*
 * Description
 * -----------
 *      return the address of the toolbar. Use this
 *      method to pack new tool buttons into the toolbar.
 */ 
{
    return &toolbar;
}
Gtk::Statusbar* MainWindow::get_statusbar()
/*
 * Description
 * -----------
 *      return the address of the status bar. Use this
 *      to acces the status bar.
 */ 
{
    return &statusBar;
}
MainWindow::~MainWindow()
{
    
    
}

void MainWindow::display_connected()
{
    Gdk::RGBA font_color;
    if (status_bar_flag==0){
    font_color.set_rgba(50/255.0, 153/255.0, 3/255.0,1);
    statusBar.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
    statusBar.push("Connected");
    status_bar_flag=1;}
    else{
        font_color.set_rgba(72/255.0, 74/255.0, 73/255.0,0.3);
        statusBar.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
        statusBar.push("Connected");
        status_bar_flag=0;
        }
}
void MainWindow::display_disconnected()
{
    Gdk::RGBA font_color;
    font_color.set_rgba(153/255.0, 26/255.0, 3/255.0,1);
    statusBar.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
    statusBar.push("Disconnected");     
}

bool MainWindow::get_status()
{
    socket_ sock(HiCIBaS_ip,HiCIBaS_port,socket_timeout);
    if (sock.status!=0){
        display_disconnected();
        return true;
        }
    else {
        display_connected();
        }
    std::string value="";
    std::cout<<"snd_cmd: "<<snd_cmd("lscmd",&value)<<std::endl;
    std::cout<<"value: "<<value<<std::endl;
    return true;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:::::::::::: Definiction of HiCIBaS_connection Class ::::::::
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

HiCIBaS_connection::HiCIBaS_connection(std::string ip,int port)
/*
 * 
 */ 
{
    HiCIBaS_connection::socket_timeout = 3;
    HiCIBaS_connection::HiCIBaS_ip = ip;
    HiCIBaS_connection::HiCIBaS_port = port;
    
}
bool HiCIBaS_connection::snd_cmd(std::string cmd,std::string *value_returned)
/*
 * Description
 * -----------
 *      First a connection is established with the server. Handshake is
 *      performed, then cmd is sent. <value_returned> will be set to the 
 *      server response to <cmd>. This function can only be used with 
 *      server response for a single "sentence" (up to the first \n read).
 *      The connection will be closed whenever the function return. 
 * 
 * NOTE
 * ----
 *      Since the connection is establish (with handshake) and close at 
 *      the end of all call, this is not ideal for slow or unstable connections.
 * 
 * Return 
 * ------
 *      true -> successfully read, false -> an error or connection timeout
 *      occured.
 */ 
{
    std::string buff="";
    int byte_sent=0;//number of bytes sent
    socket_ sock(HiCIBaS_ip,static_cast<uint16_t>(HiCIBaS_port),socket_timeout);
    if (sock.status!=0){return false;}//if cannot connect return False
    if (sock.readWelcomeMessage()!=0){sock.closeSocket(); return false;}
    //make sure there is a \n at the end of the command
    if (cmd[cmd.length()-1]!='\n')
    {
        cmd+='\n';
    }
    byte_sent = sock.writeSocket(cmd);
    if (byte_sent!=cmd.length()){sock.closeSocket(); return false;}
    *value_returned = sock.readSocket();
    //we received an NOK answer
    if (value_returned->substr(0,2).compare("OK")==0){sock.closeSocket(); return true;}
    else {sock.closeSocket(); return false;}

}

ConfigWindow::ConfigWindow(config_t *cfg_t)
: m_button_set("Set"),
m_button_cancel("Cancel"),
l_port("Port:"),
l_ip("IP address:"),
l_polling("Polling time (ms):"),
l_socket_timeout("Socket timeout (s):"),
l_comm("Protocol:"),
l_server("Server:"),
m_HBox_port(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_polling(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_ip(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_buttons(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_server(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_comm(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_socket_timeout(Gtk::ORIENTATION_HORIZONTAL),
rbtn_udp("UDP"), 
rbtn_tcp("TCP/IP"),
rbtn_local("local"),
rbtn_remote("remote")

{
    set_title("Config Panel");
    set_border_width(5);
    set_default_size(300, 300);
    ConfigWindow::cfg_t = cfg_t;
    Gtk::RadioButtonGroup group_comm,group_server;
    rbtn_udp.set_group(group_comm);
    rbtn_tcp.set_group(group_comm);
    rbtn_local.set_group(group_server);
    rbtn_remote.set_group(group_server);
   // add(m_VBox);//add m_VBox inside the window
    //m_VBox.pack_start(myBut);
    //vbox.pack_start(myBut);
    
    Gtk::Box *vbox = get_vbox();
    
    vbox->pack_end(m_HBox_buttons,Gtk::PACK_SHRINK);
    vbox->pack_start(m_HBox_ip,Gtk::PACK_SHRINK);
    vbox->pack_start(m_HBox_port,Gtk::PACK_SHRINK);
    vbox->pack_start(m_HBox_polling,Gtk::PACK_SHRINK);
    vbox->pack_start(m_HBox_comm,Gtk::PACK_SHRINK);
    vbox->pack_start(m_HBox_server,Gtk::PACK_SHRINK);
    vbox->pack_start(m_HBox_socket_timeout,Gtk::PACK_SHRINK);
    
    m_HBox_socket_timeout.pack_start(l_socket_timeout);
    m_HBox_socket_timeout.pack_end(e_socket_timeout);
    m_HBox_server.pack_start(l_server);
    m_HBox_server.pack_end(rbtn_local);
    m_HBox_server.pack_end(rbtn_remote);
    m_HBox_comm.pack_start(l_comm);
    m_HBox_comm.pack_end(rbtn_tcp);
    m_HBox_comm.pack_end(rbtn_udp);
    m_HBox_buttons.pack_start(m_button_set);
    m_HBox_buttons.pack_end(m_button_cancel);
    m_HBox_ip.pack_start(l_ip);
    m_HBox_polling.pack_start(l_polling);
    m_HBox_port.pack_start(l_port);
    m_HBox_ip.pack_end(e_ip);
    m_HBox_polling.pack_end(e_polling);
    m_HBox_port.pack_end(e_port);
    e_port.set_text(std::to_string(cfg_t->port));
    e_ip.set_text(cfg_t->ip);
    e_polling.set_text(std::to_string(cfg_t->polling_time));
    e_socket_timeout.set_text(std::to_string(cfg_t->socket_timeout));
    if (cfg_t->tcpip){
        rbtn_tcp.set_active();
        rbtn_udp.set_active(false);}
    else {
        rbtn_udp.set_active();
        rbtn_tcp.set_active(false);
        }
    if (cfg_t->local){
        rbtn_local.set_active();
        rbtn_remote.set_active(false);}
    else {
        rbtn_remote.set_active();
        rbtn_local.set_active(false);
        }
    
    m_button_set.signal_clicked().connect( sigc::mem_fun(*this,&ConfigWindow::on_button_set));
    m_button_cancel.signal_clicked().connect( sigc::mem_fun(*this,&ConfigWindow::on_button_cancel));

    show_all_children();
}
void MainWindow::on_button_config()
{

    panel_configuration.polling_time=connection_status_timeout;
    panel_configuration.port=HiCIBaS_port;
    panel_configuration.ip=HiCIBaS_ip;
    panel_configuration.local = HiCIBaS_is_local;
    panel_configuration.tcpip=HiCIBaS_is_tcpip;
    panel_configuration.socket_timeout = socket_timeout;
    ConfigWindow conf(&panel_configuration);
    conf.run();
    std::cout<<"port: "<<panel_configuration.port<<std::endl;
    std::cout<<"polling time (ms): "<<panel_configuration.polling_time<<std::endl;
    std::cout<<"IP: "<<panel_configuration.ip<<std::endl;
    std::cout<<"Server: "<<(panel_configuration.local ? "local" : "remote") <<std::endl;
    std::cout<<"Protocol: "<<(panel_configuration.tcpip ? "TCP/IP" : "UDP") <<std::endl;
    std::cout<<"Socket timeout (s): "<<panel_configuration.socket_timeout<<std::endl;
    //:::::: Settings ::::::::::::
    //start with the timedout signal
    m_connection_timeout.disconnect();
    connection_status_timeout = panel_configuration.polling_time;//set the derived HiCIBaS_connection timeout for socket communication.
    //reconnected the signal with new timeout
    m_connection_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,&MainWindow::get_status), connection_status_timeout );
    
    HiCIBaS_port = panel_configuration.port;
    HiCIBaS_ip=panel_configuration.ip;
    HiCIBaS_is_local = panel_configuration.local;
    HiCIBaS_is_tcpip = panel_configuration.tcpip;
    socket_timeout = panel_configuration.socket_timeout;
}
void ConfigWindow::on_button_set()
{

    cfg_t->ip = e_ip.get_text();
    cfg_t->port = std::atoi(e_port.get_text().c_str());
    cfg_t->polling_time = std::atoi(e_polling.get_text().c_str());
    cfg_t->local = rbtn_local.get_active();
    cfg_t->tcpip = rbtn_tcp.get_active();
    cfg_t->socket_timeout = std::atoi(e_socket_timeout.get_text().c_str());
    ConfigWindow::close();
}
void ConfigWindow::on_button_cancel()
{
    ConfigWindow::close();
}

ConfigWindow::~ConfigWindow()
{}