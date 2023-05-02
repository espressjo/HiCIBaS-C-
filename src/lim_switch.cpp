#include "lim_switch.h"

lim_switch::lim_switch(string device,string connection_type,int serial_nb)
/*
 * Description
 * ===========
 * 	Will try to establish a connection with the labjack.
 * 	If successfull, you can check the status of the connection
 * 	with the connected member.
 */ 
{
	if (LJM_OpenS(device.c_str(), connection_type.c_str(), to_string(serial_nb).c_str(), &handle)!=0)
	{
		connected=false;
	}
	else {
		connected=true;
	}
	lim.alt_zero=false;
	lim.az_zero=false;
	lim.launch=false;
	lim.left=false;
	lim.lower=false;
	lim.right=false;
	lim.upper=false;
}
lim_switch::~lim_switch()
/*
 * Description
 * ===========
 * 	Disconnect from the labjack. 
 * 
 * NOTE
 * ----
 * 	Very important if you are using the USB connection.
 */ 
{
	if (connected){
		LJM_Close(handle);
	}
}
int lim_switch::read_lim_switch()
/*
 * Description
 * ===========
 * 	Read the limit switch status. If you want to 
 * 	fetch the value use the limit structure lim.
 * NOTE:
 * -----
 * 	If an error (!=0) is return, the values are not 
 * updated
 * 
 * Return
 * ------
 *  0 -> success, anything else == error
 */ 
{
	if (!connected){return 1;}
	int error=0;
	double status=0;
	error+=LJM_eReadName(handle, "DIO6", &status);
	if (status>0.5){ lim.launch = false;}
	else{lim.launch = true;}
	error+=LJM_eReadName(handle, "DIO8", &status);
	if (status>0.5){ lim.lower = false;}
	else{lim.lower = true;}
	error+=LJM_eReadName(handle, "DIO9", &status);
	if (status>0.5){ lim.alt_zero = false;}
	else{lim.alt_zero = true;}
	error+=LJM_eReadName(handle, "DIO10", &status);
	if (status>0.5){ lim.right = false;}
	else{lim.right = true;}
	error+=LJM_eReadName(handle, "DIO11", &status);
	if (status>0.5){ lim.left = false;}
	else{lim.left = true;}
	error+=LJM_eReadName(handle, "DIO12", &status);
	if (status>0.5){ lim.upper = false;}
	else{lim.upper = true;}
	error+=LJM_eReadName(handle, "DIO4", &status);
	if (status>0.5){ lim.az_zero = false;}
	else{lim.az_zero = true;}
	return error;
}

uint8_t lim_switch::compress()
/*
 * Compress the limit structure into a uint8 number 
 * <0> upper, <1> lower , <2> right, <3> left, <4> launch, <5> AZ-0, <6> ALT-0
 */ 
{
	uint8_t C=0;
	if (lim.upper){C+=pow(2,0);}
	if (lim.lower){C+=pow(2,1);}
	if (lim.right){C+=pow(2,2);}
	if (lim.left){C+=pow(2,3);}
	if (lim.launch){C+=pow(2,4);}
	if (lim.az_zero){C+=pow(2,5);}
	if (lim.alt_zero){C+=pow(2,6);}
	return C;
}
void lim_switch::print_status()
/* 
 * Description
 * ===========
 * 	Print on screen the status of the limit switch
 */ 
{
	cout<<"Upper:    "<<((lim.upper) ? "Activated" : " ")<<endl;
	cout<<"Lower:    "<<((lim.lower) ? "Activated" : " ")<<endl;
	cout<<"Right:    "<<((lim.right) ? "Activated" : " ")<<endl;
	cout<<"Left:     "<<((lim.left) ? "Activated" : " ")<<endl;
	cout<<"Launch:   "<<((lim.launch) ? "Activated" : " ")<<endl;
	cout<<"az-zero:  "<<((lim.az_zero) ? "Activated" : " ")<<endl;
	cout<<"alt-zero: "<<((lim.alt_zero) ? "Activated" : " ")<<endl;
	return;
}

void read_limits(instHandle *handle)
{
	
	lim_switch mylim("T4","Ethernet",440010529);	
	if (!mylim.connected)
	{
		handle->lim_online=false;
		return ;
	}
	else{
		handle->lim_online=true;
		}
	int err=0;
	while(1)
	{
		err = mylim.read_lim_switch();
		if (err!=0){break;}
		handle->tcs->tcs_tel->limswitch = mylim.compress();
		std::cout<<"Compress: "<<std::to_string(static_cast<int>(handle->tcs->tcs_tel->limswitch))<<std::endl;
		std::this_thread::sleep_for (std::chrono::seconds(1));	
	}
	handle->lim_online=false;
	return ;
}