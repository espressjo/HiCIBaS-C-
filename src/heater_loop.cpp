#include "heater_loop.h"


void heater_cmd(instHandle *handle,cmd *cc)
{
    
    if ( (*cc)["-close_all"].compare("")!=0 )
    {
        handle->heater.ttm_setpoint=-999;
        handle->heater.nutec_setpoint=-999;
        handle->heater.ccam_setpoint=-999;
        cc->respond();
        return;
    }
    else if ((*cc)["-get_setpoint"].compare("")!=0) 
    {
        cc->respond("TTM: "+std::to_string(handle->heater.ttm_setpoint)+", Nutec: "+std::to_string(handle->heater.nutec_setpoint)+", Cam: "+std::to_string(handle->heater.ccam_setpoint));
        return ;
    }
    else if ((*cc)["-ttm"].compare("")!=0) 
    {
        if ((*cc)["-close"].compare("")!=0)
        {
            handle->heater.ttm_setpoint=-999;
            cc->respond();
            return;
        }
        else if ( (*cc)["setpoint"].compare("")!=0  ) 
        {
            handle->heater.ttm_setpoint = std::atoi((*cc)["setpoint"].c_str());
            cc->respond();
            return;
        }
        else {
              cc->respond("Read the doc!",uicsCMD_ERR_VALUE);
                return;  
        }
    }
    else if ((*cc)["-nutec"].compare("")!=0) 
    {
        if ((*cc)["-close"].compare("")!=0)
        {
            handle->heater.nutec_setpoint=-999;
            cc->respond();
            return;
        }
        else if ( (*cc)["setpoint"].compare("")!=0  ) 
        {
            handle->heater.nutec_setpoint = std::atoi((*cc)["setpoint"].c_str());
            cc->respond();
            return;
        }
        else {
              cc->respond("Read the doc!",uicsCMD_ERR_VALUE);
              return;  
        }
    }
    else if ((*cc)["-cam"].compare("")!=0) 
    {
        if ((*cc)["-close"].compare("")!=0)
        {
            handle->heater.ccam_setpoint=-999;
            cc->respond();
            return;
        }
        else if ( (*cc)["setpoint"].compare("")!=0  ) 
        {
            handle->heater.ccam_setpoint = std::atoi((*cc)["setpoint"].c_str());
            cc->respond();
            return;
        }
        else {
              cc->respond("Read the doc!",uicsCMD_ERR_VALUE);
              return;  
        }
    }
    cc->respond("Read the doc!",uicsCMD_ERR_VALUE);
    return;
}

void heating_loop_t(instHandle *handle)
{
    std::string model="T7",mode="Ethernet";
	int serial=470015647;
    std::string ttm_input="",nutec_input1="",nutec_input2="",cam_input="";
    
    if (ui_get_string("/opt/HiCIBaS/config/HiCIBaS.conf","IN_TTM",&ttm_input)!=0)
    {
        std::cout<<"[Warning!!!] Unable to find TTM input name" <<std::endl;
    }
    if (ui_get_string("/opt/HiCIBaS/config/HiCIBaS.conf","IN_NUTEC1",&nutec_input1)!=0)
    {
        std::cout<<"[Warning!!!] Unable to find Nutec 1/2 input name" <<std::endl;
    }
    if (ui_get_string("/opt/HiCIBaS/config/HiCIBaS.conf","IN_NUTEC2",&nutec_input2)!=0)
    {
        std::cout<<"[Warning!!!] Unable to find Nutec2/2 input name" <<std::endl;
    }
    if (ui_get_string("/opt/HiCIBaS/config/HiCIBaS.conf","IN_CAM",&cam_input)!=0)
    {
        std::cout<<"[Warning!!!] Unable to find Cam input name" <<std::endl;
    }
    
    
    
	ui_get_string("/opt/HiCIBaS/config/network.conf","LJMODEL",&model);
	ui_get_int("/opt/HiCIBaS/config/network.conf","LJSERIAL",&serial);
	ui_get_string("/opt/HiCIBaS/config/network.conf","LJMODE",&mode);
	
	ljack LJ(model,mode,serial);	
    while(1){
        //:::::::::::::::::::::::::::::::::::::::::
        //:::   Check if labjack is connected   :::
        //:::::::::::::::::::::::::::::::::::::::::
        if (!LJ.connected)
        {
            handle->tcs->tcs_tel->devices = handle->tcs->tcs_tel->devices & 127;
            continue;
        }
        //::::::::::::::::::::::::
        //:::   Loop for TTM   :::
        //::::::::::::::::::::::::
        if (handle->heater.ttm_setpoint==-999)
        {
            if (ttm_input.compare("")!=0){
                if (LJ.dio_0(ttm_input)!=0)
                {
                    handle->tcs->tcs_tel->H1=0;
                }
            }
        }
        else if (handle->tcs->tcs_tel->T1 < handle->heater.ttm_setpoint)
        {
            if (ttm_input.compare("")!=0){
                if (LJ.dio_1(ttm_input)!=0)
                {
                    handle->tcs->tcs_tel->H1=100;
                }
                else {
                    handle->tcs->tcs_tel->H1=0;
                }
                
            
            }
        }
        else {
            if (ttm_input.compare("")!=0){
                if (LJ.dio_0(ttm_input)!=0)
                {
                    handle->tcs->tcs_tel->H1=0;
                }
                else {
                    handle->tcs->tcs_tel->H1=0;
                }
            }
        }
        
        //::::::::::::::::::::::::::
        //:::   Loop for Nutec   :::
        //::::::::::::::::::::::::::
        if (handle->heater.nutec_setpoint==-999)
        {
            if (nutec_input1.compare("")!=0){
                if (LJ.dio_0(nutec_input1)!=0)
                {
                    handle->tcs->tcs_tel->H2=0;
                }
            }
            if (nutec_input2.compare("")!=0){
                if (LJ.dio_0(nutec_input2)!=0)
                {
                    handle->tcs->tcs_tel->H3=0;
                }
            }
        }
        else if (handle->tcs->tcs_tel->T4 < handle->heater.nutec_setpoint)
        {
            if (nutec_input1.compare("")!=0){
                if (LJ.dio_1(nutec_input1)!=0)
                {
                    handle->tcs->tcs_tel->H2=100;
                }
                else {
                    handle->tcs->tcs_tel->H2=0;
                }
            
            }
            if (nutec_input2.compare("")!=0){
                if (LJ.dio_1(nutec_input2)!=0)
                {
                    handle->tcs->tcs_tel->H3=100;
                }
                else {
                    handle->tcs->tcs_tel->H3=0;
                }
            
            }
        }
        else {
            if (nutec_input1.compare("")!=0){
                if (LJ.dio_0(nutec_input1)!=0)
                {
                    handle->tcs->tcs_tel->H2=0;
                }
                else {
                    handle->tcs->tcs_tel->H2=0;
                }
            }
            if (nutec_input2.compare("")!=0){
                if (LJ.dio_0(nutec_input2)!=0)
                {
                    handle->tcs->tcs_tel->H3=0;
                }
                else {
                    handle->tcs->tcs_tel->H3=0;
                }
            }
        }
        
        //::::::::::::::::::::::::
        //:::   Loop for CAM   :::
        //::::::::::::::::::::::::
        if (handle->heater.ccam_setpoint==-999)
        {
            if (cam_input.compare("")!=0){
                if (LJ.dio_0(cam_input)!=0)
                {
                    handle->tcs->tcs_tel->H4=0;
                }
            }
        }
        else if (handle->heater.ccam_setpoint!=-999)
        {
            if (cam_input.compare("")!=0){
                if (LJ.dio_1(cam_input)!=0)
                {
                    handle->tcs->tcs_tel->H4=100;
                }
                else {
                    handle->tcs->tcs_tel->H4=0;
                }
            
            }
        }
        else {
            if (cam_input.compare("")!=0){
                if (LJ.dio_0(cam_input)!=0)
                {
                    handle->tcs->tcs_tel->H4=0;
                }
                else {
                    handle->tcs->tcs_tel->H4=0;
                }
            }
        }
        std::this_thread::sleep_for (std::chrono::seconds(1));
    }//while
}