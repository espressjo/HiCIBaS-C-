#include <iostream>
#include "mainwindow.h"
#include <vector>

MainWindow::MainWindow()
: m_VBox(Gtk::ORIENTATION_VERTICAL)
  
{   //set the IP adress of HiCIBaS main software.
    //This should eventually be done from commandline or from the gui.
    HiCIBaS_ip="localhost";
    
    //set the main window attributes
    set_title("HiCIBaS Main Window");
    set_border_width(5);
    set_default_size(600, 200);
    add(m_VBox);//add m_VBox inside the window
    connection_status_timeout = 1000;
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    //:::::::: Pack the widgets inside m_VBox :::::::::

    m_VBox.pack_end(statusBar,Gtk::PACK_SHRINK);
    
  
    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::
    
    
    //::::::::::::  connect some signals ::::::::::::::::::
    m_connection_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,
              &MainWindow::connection_status), connection_status_timeout );
   
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::

    show_all_children();
}

MainWindow::~MainWindow()
{
}



bool MainWindow::connection_status()
{
  socket_ sock(HiCIBaS_ip,5555,12);//crank up the timeout to 5sec
  Gdk::RGBA font_color;
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
return true;}
