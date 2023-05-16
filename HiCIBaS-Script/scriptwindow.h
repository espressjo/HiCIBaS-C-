#ifndef GTKMM_SCRIPTWINDOW_H
#define GTKMM_SCRIPTWINDOW_H

#include "HiCIBaS_window.h"
#include <gtkmm.h>
#include <string>
#include <vector>
#include "socket_.h"
#include <gtkmm/statusbar.h>
#include "py_scripts_config.h"

class MainWindow : public HiCIBaSWindow
{
public:
  MainWindow();
  virtual ~MainWindow();

protected:
    //Signal handlers:
    void on_button_kill();
    void on_button_run();
    void on_button_read();//read stdout and stderr
    void on_button_update();//update the tree view
	
    //Gtk::Statusbar statusBar;
  
  //Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:
    ModelColumns(){ add(m_col_id); add(m_col_script_name); add(m_col_status); }
    Gtk::TreeModelColumn<unsigned int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_script_name;
    Gtk::TreeModelColumn<Glib::ustring> m_col_status;
  };
    //Child widgets:
    ModelColumns m_Columns;
    Gtk::Box *m_VBox;
    std::unique_ptr<Gtk::MessageDialog> m_pDialog;
    Gtk::ScrolledWindow m_ScrolledWindow;
    Gtk::TreeView m_TreeView;
    Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
    Gtk::ButtonBox m_ButtonBox,m_HButtonBox;
    Gtk::Button m_Button_Kill,m_Button_Read,m_Button_Run,m_Button_update_script;
	Gtk::CheckButton check_arguments;
	Gtk::HBox m_args;
	Gtk::Entry e_arguments;
private:
    //int status_bar_flag;
    //Gdk::RGBA font_color;
    void set_running(Glib::ustring script);//set running kw in the treeview
    void set_stopped(Glib::ustring script);//set stopped kw in the treeview
    void set_none(Glib::ustring script);//set -- kw in the treeview
    void set_pid(Glib::ustring script,int pid);////set the pid number in the treeview
    void show_std(Glib::ustring script);//display a window which as stdout and stderr 
    //std::string HiCIBaS_ip;//IP address of HiCIBaS's server
    //bool status();//function to update the treeview information
	std::vector<std::string> split_arguments(std::string args);
    bool HiCIBaS_get_status();
    void update_treeview(std::string ip);//fetch all the script from py_manager. 
    std::string get_selected_script_name();//return the user selected script
    //Timeout signals
    sigc::connection m_connection_timeout;//status timeout signal
	std::map<std::string,int> my_py_conf;
};

#endif //GTKMM_MAINWINDOW_H
