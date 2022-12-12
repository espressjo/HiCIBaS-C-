#ifndef MOTORSWINDOW_H
#define MOTORSWINDOW_H


#include <gtkmm.h>
#include <string>
//#include "socket_.h"
#include <gtkmm/statusbar.h>


class motorswindow : public Gtk::Window
{
public:
    motorswindow();
    ~motorswindow();
protected:
    Gtk::Statusbar statusBar;
    Gtk::Separator m_Separator1,m_Separator2,m_Separator3,m_Separator4,m_Separator5;
    Gtk::Box m_VBox_main,m_HBox_status,m_VBox_Col_alt,
        m_VBox_Col_az,m_HBox_encoder_alt,m_HBox_encoder_az,
        m_HBox_state_az,m_HBox_state_alt;//main box that contains everything
    //std::unique_ptr<Gtk::MessageDialog> m_pDialog;
    Gtk::ButtonBox m_HButtonBox,m_HButtonBoxScript;
    Gtk::Button m_Button_landing_position,m_Button_pos;
    Gtk::Button m_Button_move_alt,m_Button_move_az,m_Button_stop_all;
    Gtk::Label label_alt,label_az;
    Gtk::Label m_label_encoder_alt,m_label_encoder_az,m_label_state_az,m_label_state_alt;
    Gtk::Label m_value_encoder_az,m_value_state_az,m_value_encoder_alt,m_value_state_alt;

};
#endif // MOTORSWINDOW_H
