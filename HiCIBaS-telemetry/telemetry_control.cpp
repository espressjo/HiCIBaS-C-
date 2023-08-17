#include "telemetry_control.h"
TelemetryControlWindow::TelemetryControlWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,shared_telemetry *shm_tel)
:Gtk::Dialog(cobject),
HiCIBaS_connection("localhost",5555,6555)
{
    builder->get_widget("btn_close",btn_close);
    builder->get_widget("btn_heater_nutec_set",btn_heater_nutec_set);
    builder->get_widget("btn_heater_nutec_off",btn_heater_nutec_off);
    builder->get_widget("btn_heater_ttm_set",btn_heater_ttm_set);
    builder->get_widget("btn_heater_ttm_off",btn_heater_ttm_off);
    builder->get_widget("btn_cam_coarse_idle",btn_cam_coarse_idle);
    builder->get_widget("btn_cam_coarse_stop",btn_cam_coarse_stop);
    builder->get_widget("btn_cam_fine_idle",btn_cam_fine_idle);
    builder->get_widget("btn_cam_fine_stop",btn_cam_fine_stop);
    builder->get_widget("btn_cam_sh_idle",btn_cam_sh_idle);
    builder->get_widget("btn_cam_sh_stop",btn_cam_sh_stop);
    
    btn_close->signal_clicked().connect( sigc::mem_fun(*this, &TelemetryControlWindow::on_btn_close_clicked) );
    btn_heater_nutec_set->signal_clicked().connect( sigc::mem_fun(*this, &TelemetryControlWindow::on_btn_heater_nutec_set_clicked) );
    btn_heater_nutec_off->signal_clicked().connect( sigc::mem_fun(*this, &TelemetryControlWindow::on_btn_heater_nutec_off_clicked) );
    btn_heater_ttm_set->signal_clicked().connect( sigc::mem_fun(*this, &TelemetryControlWindow::on_btn_heater_ttm_set_clicked) );
    btn_heater_ttm_off->signal_clicked().connect( sigc::mem_fun(*this, &TelemetryControlWindow::on_btn_heater_ttm_off_clicked) );
    btn_cam_coarse_idle->signal_clicked().connect( sigc::mem_fun(*this, &TelemetryControlWindow::on_btn_cam_coarse_idle_clicked) );
    btn_cam_coarse_stop->signal_clicked().connect( sigc::mem_fun(*this, &TelemetryControlWindow::on_btn_cam_coarse_stop_clicked) );
    btn_cam_fine_idle->signal_clicked().connect( sigc::mem_fun(*this, &TelemetryControlWindow::on_btn_cam_fine_idle_clicked) );
    btn_cam_fine_stop->signal_clicked().connect( sigc::mem_fun(*this, &TelemetryControlWindow::on_btn_cam_fine_stop_clicked) );
    btn_cam_sh_idle->signal_clicked().connect( sigc::mem_fun(*this, &TelemetryControlWindow::on_btn_cam_sh_idle_clicked) );
    btn_cam_sh_stop->signal_clicked().connect( sigc::mem_fun(*this, &TelemetryControlWindow::on_btn_cam_sh_stop_clicked) );
    
    builder->get_widget("vbox_nutec",vbox_nutec);
    builder->get_widget("vbox_ttm",vbox_ttm);
    builder->get_widget("vbox_coarse",vbox_coarse);
    builder->get_widget("vbox_sh",vbox_sh);
    builder->get_widget("vbox_fine",vbox_fine);
    
    builder->get_widget("entry_nutec",entry_nutec);
    builder->get_widget("entry_ttm",entry_ttm);
    
    vbox_nutec->pack_end(led_nutec);  
    vbox_ttm->pack_end(led_ttm);  
    vbox_coarse->pack_end(led_coarse);  
    vbox_fine->pack_end(led_fine);
    vbox_sh->pack_end(led_sh);
    
    led_nutec.set_label("Heater On");
    led_ttm.set_label("Heater On");
    led_coarse.set_label("Idle On");
    led_fine.set_label("Idle On");
    led_sh.set_label("Idle On");
    shm = shm_tel;
    //ON=true;
    //g_signal_connect (this, "delete-event", G_CALLBACK (&TelemetryControlWindow::test), NULL);
    m_connection_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,
            &TelemetryControlWindow::get_status), 2000 );
    host="localhost";
    if (ui_get_string("/opt/HiCIBaS/config/network.conf","HOST",&host)!=0)
    {
        cout<<"host name not found in config file"<<endl;
    }
    
    
    
    string heater="";
    int ret = snd_cmd_ip("heater -get_setpoint",&heater,5555,6555,host,false,400);
    string val="";
    if (get_heater_val(heater,"TTM",&val)!=-1)
    {
        entry_ttm->set_text(val);
        
    }
    if (get_heater_val(heater,"Nutec",&val)!=-1)
    {
        entry_nutec->set_text(val);
        
    }

    
    show_all_children(); 
}

TelemetryControlWindow::~TelemetryControlWindow()
{
}

vector<string> TelemetryControlWindow::split(string str,char sep)
{
    vector<string> V;
    string buff="";
    for (auto &c:str)
        {
            if (c==sep){
                V.push_back(buff);
                buff="";
                continue;
            }
            else if (c=='\n')
            {
                break;
            }
            
            buff+=c;
        }
        if (buff!=""){V.push_back(buff);}
        return V; 
}
string TelemetryControlWindow::strip_space(string w)
{   string buff="";
    for (auto &c:w)
    {
        if (c!=' '){buff+=c;}
        }
        return buff;
}

//TTM: 20, Nutec: 20, Cam: 20

int TelemetryControlWindow::get_heater_val(string heater_return,string heater,string *value)
{
        *value = "";
        if ( heater_return.find(":")==std::string::npos)
        {
            return -1;
        }
        vector<string> V = split(heater_return,',');
        vector<string> V_;
        for (auto w : V)
        {
            V_ = split(w,':');
            if (V_.empty()){continue;}
            if (V_[0].find(heater)!=string::npos)
            {
                *value = strip_space(V_[1]);
                return 0;
            }
        }
        return -1;
}

bool TelemetryControlWindow::on_delete_event( GdkEventAny* ) 
{
    m_connection_timeout.disconnect();
    return false;
}
bool TelemetryControlWindow::get_status()
{
   if (shm->shmp->cam1)
   {
       led_coarse.activate();
    }
   else{
       led_coarse.deactivate();
    }
    if (shm->shmp->cam3)
   {
       led_fine.activate();
    }
   else{
       led_fine.deactivate();
    }
    if (shm->shmp->cam2)
   {
       led_sh.activate();
    }
   else{
       led_sh.deactivate();
    }
    if (shm->shmp->H3>0 && shm->shmp->H2>0)
    {
    led_nutec.activate();    
    }
    else if (shm->shmp->H3>0 || shm->shmp->H2>0)
    {
        led_nutec.activate_red();
    }
    else {
        led_nutec.deactivate();
    }
    if (shm->shmp->H1>0)
    {
    led_ttm.activate();    
    }
    else{
        led_ttm.deactivate();
    }
    return true;
}
void TelemetryControlWindow::on_btn_heater_nutec_set_clicked()
{
    //snd_cmd_ip()
    string buff="";
    snd_cmd_ip("heater setpoint "+entry_nutec->get_text()+" -nutec",&buff,5555,6555,host,false,400);
}
void TelemetryControlWindow::on_btn_heater_nutec_off_clicked()
{
     string buff="";
    snd_cmd_ip("heater setpoint -999 -nutec",&buff,5555,6555,host,false,400);
}
void TelemetryControlWindow::on_btn_heater_ttm_set_clicked()
{
     string buff="";
    snd_cmd_ip("heater setpoint "+entry_ttm->get_text()+" -nutec",&buff,5555,6555,host,false,400);
}
void TelemetryControlWindow::on_btn_heater_ttm_off_clicked()
{
     string buff="";
    snd_cmd_ip("heater setpoint -999 -ttm",&buff,5555,6555,host,false,400);
}
void TelemetryControlWindow::on_btn_cam_coarse_idle_clicked()
{string buff="";
    snd_cmd_ip("python script /opt/HiCIBaS/scripts/idle_coarse.py -run",&buff,host,5555,6555,host,false,400);
}
void TelemetryControlWindow::on_btn_cam_coarse_stop_clicked()
{string buff="";
    snd_cmd_ip("python script /opt/HiCIBaS/scripts/idle_coarse.py -kill",&buff,host,5555,6555,host,false,400);
}
void TelemetryControlWindow::on_btn_cam_fine_idle_clicked()
{string buff="";
    snd_cmd_ip("python script /opt/HiCIBaS/scripts/idle_fine.py -run",&buff,host,5555,6555,host,false,400);
}
void TelemetryControlWindow::on_btn_cam_sh_idle_clicked()
{string buff="";
    snd_cmd_ip("python script /opt/HiCIBaS/scripts/idle_sh.py -run",&buff,host,5555,6555,host,false,400);
}
void TelemetryControlWindow::on_btn_cam_sh_stop_clicked()
{
    string buff="";
    snd_cmd_ip("python script /opt/HiCIBaS/scripts/idle_sh.py -kill",&buff,host,5555,6555,host,false,400);
}
void TelemetryControlWindow::on_btn_cam_fine_stop_clicked()
{string buff="";
    snd_cmd_ip("python script /opt/HiCIBaS/scripts/idle_fine.py -kill",&buff,host,5555,6555,host,false,400);
}
void TelemetryControlWindow::on_btn_close_clicked()
{
    m_connection_timeout.disconnect();
    
    TelemetryControlWindow::close();
    
}