#include "cam_config_window.h"

//Gtk::CheckButton btn_boost;
    //Gtk::Entry entry_exp,entry_fps,entry_gain; 
ConfigCam::ConfigCam(std::string cam,std::string *cmd,cam_param param)
:btn_boost(""),
btn_cancel("Cancel"),
btn_set("Set"),
lbl_exp("Exp (ms): "),
lbl_fps("FPS: "),
lbl_boost("Boost: "),
lbl_gain("Gain: ")
{
    set_title("Cam Config Panel ["+cam+"]");
    set_border_width(5);
    set_default_size(250, 100);
    
    cam_name = cam;
    script = "/opt/HiCIBaS/scripts/cam_config.py";
    cmd_ = cmd;
    Gtk::Box *vbox = get_vbox();
    vbox->pack_start(hb1,Gtk::PACK_SHRINK);
    vbox->pack_start(hb2,Gtk::PACK_SHRINK);
    vbox->pack_start(hb3,Gtk::PACK_SHRINK);
    vbox->pack_start(hb4,Gtk::PACK_SHRINK);
    vbox->pack_end(hbox_btn,Gtk::PACK_SHRINK);
    
    hbox_btn.pack_start(btn_set,Gtk::PACK_SHRINK);
    hbox_btn.pack_start(btn_cancel,Gtk::PACK_SHRINK);
    hb1.pack_start(lbl_exp,Gtk::PACK_SHRINK);
    hb1.pack_end(entry_exp,Gtk::PACK_SHRINK);
    hb2.pack_start(lbl_fps,Gtk::PACK_SHRINK);
    hb2.pack_end(entry_fps,Gtk::PACK_SHRINK);
    hb3.pack_start(lbl_gain,Gtk::PACK_SHRINK);
    hb3.pack_end(entry_gain,Gtk::PACK_SHRINK);
    hb4.pack_start(lbl_boost,Gtk::PACK_SHRINK);
    hb4.pack_end(btn_boost,Gtk::PACK_SHRINK);
    
    entry_exp.set_width_chars(7);
    entry_fps.set_width_chars(5);
    entry_gain.set_width_chars(4);
    
    
    
    btn_set.signal_clicked().connect( sigc::mem_fun(*this,&ConfigCam::on_button_set));
    btn_cancel.signal_clicked().connect( sigc::mem_fun(*this,&ConfigCam::on_button_cancel));
    show_all_children();
    
    if (!param.exp.empty()){
    entry_exp.set_text(param.exp);}
    if (!param.fps.empty()){
    entry_fps.set_text(param.fps);}
    if (!param.gain.empty()){
    entry_gain.set_text(param.gain);}
    
    if (!param.boost.empty())
    {
        if (param.boost.compare("1")==0)
        {
            btn_boost.set_active(true);
            
        }
        else
        {btn_boost.set_active(false);
         }
    }
    std::cout<<"Boost: "<<param.boost<<std::endl;
    std::cout<<"Exp: "<<param.exp<<std::endl;
    std::cout<<"FPS: "<<param.fps<<std::endl;
    std::cout<<"Gain: "<<param.gain<<std::endl;
    

    
}

void ConfigCam::on_button_set()
{
 /*
    cfg_t->nutec_speed = std::stod(e_nutec_speed.get_text().c_str());
    cfg_t->nutec_acc= std::stod(e_nutec_acc.get_text().c_str());
    cfg_t->nutec_dec= std::stod(e_nutec_dec.get_text().c_str());
    cfg_t->rm8_low_speed= std::atoi(e_rm8_low_speed.get_text().c_str());
    cfg_t->rm8_high_speed= std::atoi(e_rm8_high_speed.get_text().c_str());
    cfg_t->rm8_acc= std::atoi(e_rm8_acc.get_text().c_str());
*/
    std::string buff="";
    std::vector<std::string> arg;
    if (entry_exp.get_text().compare("")!=0)
    {
        arg.push_back("--exp="+entry_exp.get_text());
    }
    if (entry_fps.get_text().compare("")!=0)
    {
        arg.push_back("--fps="+entry_fps.get_text());
    }
    if (entry_gain.get_text().compare("")!=0)
    {
        arg.push_back("--gain="+entry_gain.get_text());
    }
    if (btn_boost.get_active())
    {
        arg.push_back("--boost=1");
    }
    else{
        arg.push_back("--boost=0");
    }
    arg.push_back("--"+cam_name);
    
    int i=0;
    for (auto v : arg)
    {
     i++;   
     buff+=" arg"+std::to_string(i)+" "+v;
    }

    buff = "python script "+script+buff+" -run";
    
    
    *cmd_ = buff;
    //std::cout<<buff<<std::endl;
    ConfigCam::close();
}
void ConfigCam::on_button_cancel()
{
    *cmd_ = "";
    ConfigCam::close();
}

ConfigCam::~ConfigCam()
{}
