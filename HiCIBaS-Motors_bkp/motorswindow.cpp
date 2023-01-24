
#include "motorswindow.h"


motorswindow::motorswindow():   
m_VBox_main(Gtk::ORIENTATION_VERTICAL),
m_HBox_status(Gtk::ORIENTATION_HORIZONTAL),
m_VBox_Col_alt(Gtk::ORIENTATION_VERTICAL),
m_VBox_Col_az(Gtk::ORIENTATION_VERTICAL),
m_HBox_encoder_alt(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_encoder_az(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_state_az(Gtk::ORIENTATION_HORIZONTAL),
m_HBox_state_alt(Gtk::ORIENTATION_HORIZONTAL),
m_HButtonBox(Gtk::ORIENTATION_HORIZONTAL),
m_HButtonBoxScript(Gtk::ORIENTATION_HORIZONTAL),
m_Button_move_alt("Move Alt."),
m_Button_move_az("Move AZ."),
m_Button_stop_all("Stop"),
m_Button_landing_position("Landing"),
m_Button_pos("Position 1"),
m_label_state_alt("State: "),
m_label_encoder_alt("Encoder (°): "),
m_label_state_az("State: "),
m_label_encoder_az("Encoder (°): "),
m_value_encoder_az("10.02"),
m_value_state_az("Moving"),
m_value_encoder_alt("89.01"),
m_value_state_alt("Stalled"),
label_alt("Altitude"),
label_az("Azimuth")
{
    set_title("HiCIBaS Telescope Motors");
    set_border_width(5);
    set_default_size(260, 180);
    add(m_VBox_main);
    m_VBox_main.pack_start(m_HBox_status,Gtk::PACK_SHRINK);
    m_VBox_main.pack_start(m_Separator1);
    m_VBox_main.pack_start(m_HButtonBox,Gtk::PACK_SHRINK);
    m_VBox_main.pack_start(m_Separator2);
    m_VBox_main.pack_start(m_HButtonBoxScript,Gtk::PACK_SHRINK);
    m_VBox_main.pack_start(m_Separator3);
    m_VBox_main.pack_start(statusBar,Gtk::PACK_SHRINK);
    m_HButtonBox.set_margin_start(5);
    m_HButtonBoxScript.set_margin_start(5);
    m_VBox_Col_alt.set_margin_start(5);
    m_VBox_Col_az.set_margin_start(5);
    m_HBox_status.pack_start(m_VBox_Col_alt,Gtk::PACK_SHRINK);
    m_HBox_status.pack_start(m_Separator4,Gtk::PACK_SHRINK);//m_Separator
    m_HBox_status.pack_end(m_Separator5,Gtk::PACK_SHRINK);//m_Separator
    m_HBox_status.pack_end(m_VBox_Col_az,Gtk::PACK_SHRINK);
    m_VBox_Col_alt.pack_start(label_alt,Gtk::PACK_SHRINK);
    
    m_VBox_Col_alt.pack_start(m_HBox_encoder_alt,Gtk::PACK_SHRINK);
    m_VBox_Col_alt.pack_start(m_HBox_state_alt,Gtk::PACK_SHRINK);
    m_VBox_Col_az.pack_start(label_az,Gtk::PACK_SHRINK);
    
    m_VBox_Col_az.pack_start(m_HBox_encoder_az,Gtk::PACK_SHRINK);
    m_VBox_Col_az.pack_start(m_HBox_state_az,Gtk::PACK_SHRINK);
   
   
    m_HBox_state_az.pack_start(m_label_state_az,Gtk::PACK_SHRINK);
    m_HBox_state_az.pack_start(m_value_state_az,Gtk::PACK_SHRINK);
    m_HBox_state_alt.pack_start(m_label_state_alt,Gtk::PACK_SHRINK);
    m_HBox_state_alt.pack_start(m_value_state_alt,Gtk::PACK_SHRINK);
    
    m_HBox_encoder_az.pack_start(m_label_encoder_az,Gtk::PACK_SHRINK);
    m_HBox_encoder_az.pack_start(m_value_encoder_az,Gtk::PACK_SHRINK);
    m_HBox_encoder_alt.pack_start(m_label_encoder_alt,Gtk::PACK_SHRINK);
    m_HBox_encoder_alt.pack_start(m_value_encoder_alt,Gtk::PACK_SHRINK);
    
    m_HButtonBox.pack_start(m_Button_move_alt,Gtk::PACK_SHRINK);
    m_HButtonBox.pack_start(m_Button_move_az,Gtk::PACK_SHRINK);
    m_HButtonBox.pack_start(m_Button_stop_all,Gtk::PACK_SHRINK);
    m_HButtonBoxScript.pack_start(m_Button_landing_position,Gtk::PACK_SHRINK);
    m_HButtonBoxScript.pack_start(m_Button_pos,Gtk::PACK_SHRINK);
    
    Gdk::RGBA font_color;
    font_color.set_rgba(153/255.0, 26/255.0, 3/255.0,1);
    m_value_state_alt.override_color(font_color,Gtk::StateFlags::STATE_FLAG_NORMAL);
    
    
    statusBar.push("Not Connected");
    show_all_children();
    
}
motorswindow::~motorswindow()
{
    
}
