#include "nutec.h"

mutex serial_lock;

void delay(int ms)
/*!
  *\brief Delay in ms
  * Create a delay. ms is delay time in millisecond
  */
{
   this_thread::sleep_for(chrono::milliseconds(ms));
}


void enable(instHandle *handle,cmd *cc)
{
	if (setRegister(handle,"0x24",21)!=0)
	{
		cc->respond("Unable to enable the motor",uicsCMD_ERR_VALUE);
		return ;
	}
	handle->enabled = true;
cc->respond();
	
}
void disable(instHandle *handle,cmd *cc)
{
	if (setRegister(handle,"0x24",0)!=0)
	{
		cc->respond("Unable to enable the motor",uicsCMD_ERR_VALUE);
		return ;
	}
	handle->enabled = false;
	cc->respond();
	
}
void isEnabled(instHandle *handle,cmd *cc)
{
	int enabled=0;
	if (readRegister(handle,"0x24",&enabled)!=0)
	{
		cc->respond("Unable to enable the motor",uicsCMD_ERR_VALUE);
		return;
	}
	if (enabled==21)
	{
		handle->enabled = true;
		cc->respond("T");
		return;
	}
	else if (enabled==0){
		handle->enabled = false;
		cc->respond("F");
		return;
	}
	else {
		cc->respond("Motor status is "+std::to_string(enabled),uicsCMD_ERR_VALUE);
		return;
	}
}



int setup(instHandle *handle)
/*
 * Make sure the default value for guiding are set 
 * to the baseline. 
 */ 
{
	string check="";
	//write 1 to current each bit to clear any errors
	if (setRegister(handle,"0xa4","0xffff")!=0){return -1;}
	
	if (setRegister(handle,"0xc8",257)!=0){return -1;}
	
	if (setRegister(handle,"0xcb",48000)!=0){return -1;}
	
	if (setRegister(handle,"0xcc",28800)!=0){return -1;}
	
	if (setRegister(handle,"0xcd",28800)!=0){return -1;}
	
	if (setRegister(handle,"0xce",1440000)!=0){return -1;}
	
	if (setRegister(handle,"0xcf",299988)!=0){return -1;}
		
	//:::::::::::::::::::::::::::
	//:::   To be tested!!!   :::
	//:::::::::::::::::::::::::::
	if (handle->sport.ecrireport("\x00\x4f\x02\x14\x00\x03\x00\x00\r")!=OK){return -1;}
	if (handle->sport.lirec(&check,'\r')==0){return -1;}
	cout<<"Serial array return: "<<check<<endl;
	
	return 0;
}


int setRegister(instHandle *handle,string reg,int value,bool RAM)
/*
 * Set the given register with value. 
 * 
 * Parameters
 * ----------
 * 	reg : STR
 * 		register to be read. e.g., 0x32. Do not specify r/f !
 * 	value : INT
 * 		The value to be written.
 * 	RAM : BOOL
 * 		If we read from RAM or FLASH
 * 
 * Return 
 * ------
 * 	0 - > successfull
 *  -1 -> unsuccessfull or and error code has been returned.
 */ 
{
	string c="",answ="",mycmd="";
	
	c = (RAM) ? "r" : "f";
	
	mycmd="s "+c+reg+" "+std::to_string(value)+"\r";
	serial_lock.lock();
	if (handle->sport.ecrireport(mycmd)!=OK)
	{
		return -1;
	}
	if (handle->sport.lirec(&answ,'\r')==0)
	{
		return -1;
	}
	serial_lock.unlock();
	if (answ.substr(0,2).compare("ok")==0)
	{
		return 0;
	}
	return -1;
	
}

int setRegister(instHandle *handle,string reg,string value,bool RAM)
/*
 * Set the given register with value. 
 * 
 * Parameters
 * ----------
 * 	reg : STR
 * 		register to be read. e.g., 0x32. Do not specify r/f !
 * 	value : STR
 * 		The value to be written.
 * 	RAM : BOOL
 * 		If we read from RAM or FLASH
 * 
 * Return 
 * ------
 * 	0 - > successfull
 *  -1 -> unsuccessfull or and error code has been returned.
 */ 
{
	string c="",answ="",mycmd="";
	
	c = (RAM) ? "r" : "f";
	
	mycmd="s "+c+reg+" "+value+"\r";
	serial_lock.lock();
	if (handle->sport.ecrireport(mycmd)!=OK)
	{
		return -1;
	}
	if (handle->sport.lirec(&answ,'\r')==0)
	{
		return -1;
	}
	serial_lock.unlock();
	if (answ.substr(0,2).compare("ok")==0)
	{
		return 0;
	}
	return -1;
	
}

void deduce_moving(instHandle *handle,cmd *cc)
{
    if ((*cc)["-set"].compare("")!=0)
    {
        handle->deduce_moving = true;
        cc->respond();
        return;
    }
    if ((*cc)["-unset"].compare("")!=0)
    {
        handle->deduce_moving = false;
        cc->respond();
        return;
    }
    cc->respond("Read the doc",uicsCMD_ERR_VALUE);
    return;
    
}

void position_status_t(instHandle *handle)
/*
 * Update the handle position every second.
 */ 
{
	int pos = 0;
	uint32_t xa0=0;
	while(1){
		delay(1000);
		//read the position
		if (handle->sport.status==OPEN)
		{
			
			if (readRegister(handle,"0x32",&pos)==0)
			{
				if (handle->deduce_moving){
                    
                    handle->moving = ( abs(handle->position - pos)<100 ) ? true : false;
                }
				handle->position = pos;
			}
            else {
                printf("[Warning] Merde status [1]\n");
            }
			//read the position
			if (readRegister_32(handle,"0xa0",&xa0)==0)
			{
				handle->xa0 = xa0;
				handle->lim_n = ( (xa0 & static_cast<uint32_t>(512)) == static_cast<uint32_t>(512) ) ? true : false ;
				handle->lim_p = ( (xa0 & static_cast<uint32_t>(1024)) == static_cast<uint32_t>(1024) ) ? true : false ;
                handle->home =( (xa0 & static_cast<uint32_t>(67108864)) == static_cast<uint32_t>(67108864) ) ? true : false ;
				if (!handle->deduce_moving){
                handle->moving = ( (xa0 & static_cast<uint32_t>(134217728)) == static_cast<uint32_t>(134217728) ) ? true : false ;
                }
				handle->enabled = ( (xa0 & static_cast<uint32_t>(4096)) == static_cast<uint32_t>(4096) ) ? false : true ;
				handle->phase_error = ( (xa0 & static_cast<uint32_t>(64)) == static_cast<uint32_t>(64) ) ? true : false ;
			}
            else {
                printf("[Warning] Merde status [2]\n");
            }
 
		}	
		
	}
}
int readRegister(instHandle *handle,string reg,int *value,bool RAM)
/*
 * Use to read the specified register. The value will
 * automatically be converted into an integer. 
 * 
 * Parameters
 * ----------
 * 	reg : STR
 * 		register to be read. e.g., 0x32. Do not specify r/f !
 * 	*value : INT
 * 		The value read will be set by the value pointer. If an
 * 		error is read, the error return value will be set to *value 
 * 	RAM : BOOL
 * 		If we read from RAM or FLASH
 * 
 * Return 
 * ------
 * 	0 - > successfull
 *  -1 -> unsuccessfull or and error code has been returned.
 * 
 */ 
{
	string answ="";
	
	//set the flash or RAM character
    if (readRegister(handle,reg,&answ,RAM)!=0)
    {
        std::cout<<"Error received: "<<answ<<std::endl;
        return -1;
    }
    if (ui_conf::is_int(answ)){
        *value = std::atoi(answ.c_str());
        return 0;
    }
    else {std::cout<<"Msg recv is not INT: "<<answ<<std::endl;}
	return -1;
}
int readRegister_32(instHandle *handle,string reg,uint32_t *value,bool RAM)
/*
 * Use to read the specified register. The value will
 * automatically be converted into an integer. 
 * 
 * Parameters
 * ----------
 * 	reg : STR
 * 		register to be read. e.g., 0x32. Do not specify r/f !
 * 	*value : unsigned int 32 bits
 * 		The value read will be set by the value pointer. If an
 * 		error is read, the error return value will be set to *value 
 * 	RAM : BOOL
 * 		If we read from RAM or FLASH
 * 
 * Return 
 * ------
 * 	0 - > successfull
 *  -1 -> unsuccessfull or and error code has been returned.
 * 
 */ 
{
	string answ="";
	
	//set the flash or RAM character
    if (readRegister(handle,reg,&answ,RAM)!=0)
    {
        std::cout<<"Error received: "<<answ<<std::endl;
        return -1;
    }
    //make sure number is position and INT 
    for (auto &c:answ)
    {
        if (!isdigit(c))
        {return -1;}
    }
    
    *value = std::stoul(answ.c_str());
    return 0;
    
}

int readRegister(instHandle *handle,string reg,string *value,bool RAM)
/*
 * Use to read the specified register. The value will
 * automatically be converted into an integer. 
 * 
 * Parameters
 * ----------
 * 	reg : STR
 * 		register to be read. e.g., 0x32. Do not specify r/f !
 * 	*value : STR
 * 		The value read will be set by the value pointer. If an
 * 		error is read, the error return value will be set to *value
 *		including the e code character.	
 * 	RAM : BOOL
 * 		If we read from RAM or FLASH
 * 
 * Return 
 * ------
 * 	0 - > successfull
 *  -1 -> unsuccessfull or and error code has been returned.
 * 
 */ 
{
	string c="",answ="",mycmd="";
	
	//set the flash or RAM character
	c = (RAM) ? "r" : "f";
	
	mycmd="g "+c+reg+"\r";
	serial_lock.lock();
	if (handle->sport.ecrireport(mycmd)!=OK)
	{
		return -1;
	}
	if (handle->sport.lirec(&answ,'\r')==0)
	{
		return -1;
	}
	serial_lock.unlock();
	//strip the \r
	if (answ.substr(answ.length()-1,1).compare("\r")==0)
	{
		answ = answ.substr(0,answ.length()-2);
	}
		
	if (answ.substr(0,1).compare("v")==0)
	{		
		*value = answ.substr(2,answ.length());
		return 0;
	}
	else if (answ.substr(0,1).compare("e")==0) 
	{
		*value = answ;
		return -1;
	}
	
	return -1;
}

int toInt(string s,int *value)
/*
 * Convert a string to integer only if the string
 * if an integer. If it is not, the function will
 * return -1.
 * 
 * Paramaters
 * ----------
 * 	s : STR
 * 		string to convert
 * 	value : *INT
 * 		will be set to the converted value
 * 
 * Return 
 * ------
 * 	Return 0 if successfull.
 */ 
{
	for (auto &c:s)
	{
		if (!isdigit(c) && c!='-')
		{
			return -1;
		}
	}
	*value = std::stoi(s.c_str());
	return 0;
}
int toInt32(string s,uint32_t *value)
/*
 * Convert a string to integer only if the string
 * if an integer. If it is not, the function will
 * return -1.
 * 
 * Paramaters
 * ----------
 * 	s : STR
 * 		string to convert
 * 	value : *unsigned int 32-bits
 * 		will be set to the converted value
 * 
 * Return 
 * ------
 * 	Return 0 if successfull.
 */ 
{
	for (auto &c:s)
	{
		if (!isdigit(c) && c!='-')
		{
			return -1;
		}
	}
	*value = static_cast<uint32_t>(std::stoul(s.c_str()));
	return 0;
}
void serial_cmd_io(instHandle *handle,cmd *cc)
/*
 * Description
 * -----------
 * 	This function is used to read or write value to the copley
 *  controller. You must specify if you want to read or write
 *  from flash (f) or RAM (r) in the address. e.g., addr: r0x32
 *  
 */ 
{
	string mycmd="",answ="";
	if ((*cc)["read"].compare("")!=0){
		mycmd+="g ";
		mycmd += (*cc)["read"]+"\r";
		printf("cmd: %s\n",mycmd.c_str());
		serial_lock.lock();
		if (handle->sport.ecrireport(mycmd)!=OK)
		{
            cc->respond("Unable to write command",uicsCMD_ERR_VALUE);
			return ;
		}
		if (handle->sport.lirec(&answ,'\r')==0)
		{
			cc->respond("Unable to read command",uicsCMD_ERR_VALUE);
			return ;
		}
		serial_lock.unlock();
		cc->respond(answ);
		return;
	}
	else if ((*cc)["write"].compare("")!=0 && (*cc)[">"].compare("")!=0){
                
		mycmd="s "+(*cc)[">"]+" "+(*cc)["write"]+"\r";
		serial_lock.lock();
		if (handle->sport.ecrireport(mycmd)!=OK)
		{
			cc->respond("Unable to write command",uicsCMD_ERR_VALUE);
			return ;
		}
		if (handle->sport.lirec(&answ,'\r')==0)
		{
			cc->respond("Unable to read command",uicsCMD_ERR_VALUE);
			return ;
		}
		serial_lock.unlock();
		cc->respond(answ);
		return;
	}
	cc->respond("read the doc",uicsCMD_ERR_VALUE);
	return ;	
}

void abort(instHandle *handle,cmd *cc)
/*
 * Abort a movement.
 */ 
{
		if (trigger_abort(handle)!=0)
		{
		
			cc->respond("Unable to abort a movement",uicsCMD_ERR_VALUE);
			return ;
		}
	cc->respond();
	return ;
}

int trigger_move(instHandle *handle)
/*
 * Trigger a move.
 */ 
{
	string val="";
	serial_lock.lock();
	if (handle->sport.ecrireport("t 1\r")!=OK)
	{
		return -1;
	}
	if (handle->sport.lirec(&val,'\r')==0)
	{
		return -1;
	}
	serial_lock.unlock();
	return 0;
}
int trigger_abort(instHandle *handle)
/*
 * Stop a movement
 */ 
{
	string val="";
	serial_lock.lock();
	if (handle->sport.ecrireport("t 0\r")!=OK)
	{
		return -1;
	}
	if (handle->sport.lirec(&val,'\r')==0)
	{
		return -1;
	}
	serial_lock.unlock();
	return 0;
}

void move(instHandle *handle,cmd *cc)
{
	string position="",val="";
	 
	if ( (*cc)["position"].compare("")==0 )
	{
		cc->respond("wrong argument",uicsCMD_ERR_VALUE);
		return;
	}
	position = (*cc)["position"];
	//set absolute mode
	if (setRegister(handle,"0xca",position)!=0)
	{
		cc->respond("Unable to trigger a relatif move",uicsCMD_ERR_VALUE);
		return ;
	}
	if (trigger_move(handle)!=0)
	{
		cc->respond("Unable to trigger a relatif move",uicsCMD_ERR_VALUE);
		return ;
	}
	cc->respond();
	return ;
}
void move_no_return(instHandle *handle,cmd *cc)
{
	string position="",val="";
	 
	if ( (*cc)["position"].compare("")==0 )
	{
		
		return;
	}
	position = (*cc)["position"];
	//set absolute mode
	if (setRegister(handle,"0xca",position)!=0)
	{
		
		return ;
	}
	if (trigger_move(handle)!=0)
	{
		
		return ;
	}
	
	return ;
}
void set_speed(instHandle *handle,cmd *cc)
{
	string buff="";
	 int counts=0;
	 
	if ( (*cc)["counts"].compare("")!=0 )
	{
		if ( !ui_conf::is_int((*cc)["counts"]))
		{
			cc->respond("Counts must be int",uicsCMD_ERR_VALUE);
			return ;
		}
		counts = std::atoi( (*cc)["counts"].c_str()  ) ;
		
		if (setRegister(handle,"0xcb",counts)!=0)
		{
			cc->respond("Unable to set speed",uicsCMD_ERR_VALUE);
			return ;
		}
		cc->respond();
		return ;
	}
	
	else if ( (*cc)["rpm"].compare("")!=0 )
	{
		if (!ui_conf::is_double((*cc)["rpm"]))
		{
			cc->respond("Counts must be int",uicsCMD_ERR_VALUE);
			return ;
		}
		counts = vel_counts( std::stod( (*cc)["rpm"].c_str() ) );
		
		if (setRegister(handle,"0xcb",counts)!=0)
		{
			cc->respond("Unable to set speed",uicsCMD_ERR_VALUE);
			return ;
		}
		cc->respond();
		return ;
	}
	else{
		cc->respond(" wrong arguments ",uicsCMD_ERR_VALUE);
		return ;
	}

}

void get_speed(instHandle *handle,cmd *cc)
/*
 * Will return the speed of the motor.
 */ 
{
	string speed="";
	int counts=0;
	//set absolute mode
	if (readRegister(handle,"0xcb",&speed)!=0)
	{
		cc->respond("Unable to set speed",uicsCMD_ERR_VALUE);
		return ;
	}
	if ( (*cc)["-rpm"].compare("")!=0  ){
		counts = std::atoi(speed.c_str());
		speed = std::to_string(vel_rpm(counts));
		cc->respond(speed);
		return ;
	}
	
	cc->respond(speed);
	return ;
}

void isMoving(instHandle *handle,cmd *cc)
/*
 * Read the status of the controller, if the motor
 * is moving it will return T, otherwise it will return F.
 */ 
{
	uint32_t xa0=0;
	//set absolute mode
	if (readRegister_32(handle,"0xa0",&xa0,true)!=0)
	{
		cc->respond("Unable to read status",uicsCMD_ERR_VALUE);
		return ;
	}
	if ((xa0 & static_cast<uint32_t>(pow(2,27)) ) == static_cast<uint32_t>(pow(2,27)))
	{
		cc->respond("T");
		return ;
	}
	
	cc->respond("F");
	return ;
}

void move_abs(instHandle *handle,cmd *cc)
{
	string position="",val="";
	 
	if ( (*cc)["position"].compare("")==0 )
	{
		cc->respond("wrong argument",uicsCMD_ERR_VALUE);
		return;
	}
	position = (*cc)["position"];
	//set absolute mode
	if (setRegister(handle,"0xc8",1)!=0)
	{
		cc->respond("Unable to set absolute move mode",uicsCMD_ERR_VALUE);
		return ;
	}
	if (setRegister(handle,"0xca",position)!=0)
	{
		cc->respond("Unable to set position",uicsCMD_ERR_VALUE);
		return ;
	}
	
	if (trigger_move(handle)!=0)
	{
		cc->respond("Unable to trigger move",uicsCMD_ERR_VALUE);
		return ;
	}
	//set absolute mode
	if (setRegister(handle,"0xc8",257)!=0)
	{
		cc->respond("Unable to set relative move mode",uicsCMD_ERR_VALUE);
		return ;
	}
	cc->respond();
	return;
}

void read_position(instHandle *handle,cmd *cc)
/*
 * read the controller position.
 */ 
{
	string val="";
	if (readRegister(handle,"0x32",&val)!=0)
	{
		cc->respond("Unable to read position",uicsCMD_ERR_VALUE);
		return ;
	}	
	cc->respond(val);
	return ;
	
}

void p_status(instHandle *handle,cmd *cc)
/*
 * Print the status of the motor.
 */ 
{
	string status="";
	status+=string("Position: ")+to_string(handle->position)+"\n";
	status+=string("Drive Enabled: ")+ ((handle->enabled) ? "T" : "F")  +"\n";
	status+=string("Com. Active: ")+ ((handle->active) ? "T" : "F" ) +"\n";
	status+=string("Moivng: ")+ ((handle->moving) ? "T" : "F" ) +"\n";
	status+=string("Phase Error: ")+ ((handle->phase_error) ? "T" : "F")  +"\n";
	status+=string("Lim +: ")+ ((handle->lim_p) ? "T" : "F" ) +"\n";
	status+=string("Lim -: ")+ ((handle->lim_n) ? "T" : "F")  +"\n";
	status+=string("Home : ")+ ((handle->home) ? "T" : "F")  +"\n";
	status+=string("Register 0xa0: ")+to_string(handle->xa0)+"\n";
	cc->respond(status);
	return ;
}

void status_t(instHandle *handle)
{
	int fd = create_socket(4568);
	cmd *c = new cmd;
	while(1)
	{
			c->recvCMD(fd);

			if ((*c)["-print"].compare("")!=0)
			{
					p_status(handle,c);
					continue;
			}
			else if ((*c)["-get"].compare("")!=0)
			{
					g_status(handle,c);
					continue;
			}
			else {
					c->respond("Read the doc!",uicsCMD_ERR_VALUE);
					continue;
			}
	}
}

void g_status(instHandle *handle,cmd *cc)
/*
 * Return the status in a structure.
 */ 
{
	char *encodedStructure=NULL;
	nutec_telemetry tlm;
	size_t length=0;
	//::::::::::::::::::::::::::::::
	//:::   copy the structure   :::
	//::::::::::::::::::::::::::::::
	tlm.position = handle->position;
	tlm.enabled = handle->enabled;
	tlm.active = handle->active;//the serial communication is established, position is updated.
	tlm.moving = handle->moving;
	tlm.lim_p = handle->lim_p;
	tlm.lim_n = handle->lim_n;
	tlm.home = handle->home;
	tlm.phase_error = handle->phase_error;
	
	length = islb64EncodeAlloc((const char *)&tlm,sizeof(tlm),&encodedStructure);
    cc->respond(std::to_string(length)+" "+std::string(encodedStructure));
	return ;
}

void serial_cmd(instHandle *handle,cmd *cc)
/*
 * This function is used to open/close or re-open
 * the serial port of the nutec controller. Use
 * the argument -open, -close and -force to do so.
 * The -force argument will try to open/re-open the port
 * regarless of the serial status.
 */ 
{
	if ( (*cc)["-close"].compare("") != 0 ){
		handle->sport.fermerport();
		cc->respond();
		return;
	} 
	else if ( (*cc)["-open"].compare("") != 0 ){
		if ( (*cc)["-force"].compare("") != 0 ) {
			handle->sport.ouvrirport(handle->serial_port,handle->baudrate);
			cc->respond();
			return;
		}
		else {
		//do something	
			if (handle->sport.status!=OPEN)
			{
				handle->sport.ouvrirport(handle->serial_port,handle->baudrate);
				cc->respond();
				return;
			}
			else 
			{
				cc->respond("Serial port already open",uicsCMD_ERR_VALUE);
				return;
			}
		}
		
	}
	cc->respond("Wrong argument",uicsCMD_ERR_VALUE);
	return ;
}

double vel_rpm(int counts){

	return 0.1*counts*60.0/3.6e6;
}
int vel_counts(double rpm){
	double unit=0.1;
	return int((rpm/60.0) *(3.6e6)/unit);
}
int acc_counts(double rps){
	double unit=10;
	return rps*3.6e6/unit;
}
    
double acc_rps(int count){
	int unit=10;
    count = count*unit;
    return static_cast<double>(count)/3.6e6;
}


void set_acceleration(instHandle *handle,cmd *cc)
{
	string buff="";
	 int counts=0;
	 
	if ( (*cc)["counts"].compare("")!=0 )
	{
		if ( !ui_conf::is_int((*cc)["counts"]))
		{
			cc->respond("Counts must be int",uicsCMD_ERR_VALUE);
			return ;
		}
		counts = std::atoi( (*cc)["counts"].c_str()  ) ;
		
		if (setRegister(handle,"0xcc",counts)!=0)
		{
			cc->respond("Unable to set speed",uicsCMD_ERR_VALUE);
			return ;
		}
		cc->respond();
		return ;
	}
	
	else if ( (*cc)["rps"].compare("")!=0 )
	{
		if (!ui_conf::is_double((*cc)["rps"]))
		{
			cc->respond("RPS must be double",uicsCMD_ERR_VALUE);
			return ;
		}
		counts = acc_counts( std::stod( (*cc)["rps"].c_str() ) );
		
		if (setRegister(handle,"0xcc",counts)!=0)
		{
			cc->respond("Unable to set acceleration",uicsCMD_ERR_VALUE);
			return ;
		}
		cc->respond();
		return ;
	}
	else{
		cc->respond(" wrong arguments ",uicsCMD_ERR_VALUE);
		return ;
	}

}

void set_deceleration(instHandle *handle,cmd *cc)
{
	string buff="";
	 int counts=0;
	 
	if ( (*cc)["counts"].compare("")!=0 )
	{
		if ( !ui_conf::is_int((*cc)["counts"]))
		{
			cc->respond("Counts must be int",uicsCMD_ERR_VALUE);
			return ;
		}
		counts = std::atoi( (*cc)["counts"].c_str()  ) ;
		
		if (setRegister(handle,"0xcd",counts)!=0)
		{
			cc->respond("Unable to set deceleration",uicsCMD_ERR_VALUE);
			return ;
		}
		cc->respond();
		return ;
	}
	
	else if ( (*cc)["rps"].compare("")!=0 )
	{
		if (!ui_conf::is_double((*cc)["rps"]))
		{
			cc->respond("RPS must be double",uicsCMD_ERR_VALUE);
			return ;
		}
		counts = acc_counts( std::stod( (*cc)["rps"].c_str() ) );
		
		if (setRegister(handle,"0xcd",counts)!=0)
		{
			cc->respond("Unable to set decceleration",uicsCMD_ERR_VALUE);
			return ;
		}
		cc->respond();
		return ;
	}
	else{
		cc->respond(" wrong arguments ",uicsCMD_ERR_VALUE);
		return ;
	}

}

void get_acceleration(instHandle *handle,cmd *cc)
/*
 * Will return the speed of the motor.
 */ 
{
	string acc="";
	int counts=0;
	//set absolute mode
	if (readRegister(handle,"0xcc",&acc)!=0)
	{
		cc->respond("Unable to set speed",uicsCMD_ERR_VALUE);
		return ;
	}
	if ( (*cc)["-rps"].compare("")!=0  ){
		counts = std::atoi(acc.c_str());
		acc = std::to_string(acc_rps(counts));
		cc->respond(acc);
		return ;
	}
	
	cc->respond(acc);
	return ;
}

void get_deceleration(instHandle *handle,cmd *cc)
/*
 * Will return the speed of the motor.
 */ 
{
	string acc="";
	int counts=0;
	//set absolute mode
	if (readRegister(handle,"0xcd",&acc)!=0)
	{
		cc->respond("Unable to get deceleration",uicsCMD_ERR_VALUE);
		return ;
	}
	if ( (*cc)["-rps"].compare("")!=0  ){
		counts = std::atoi(acc.c_str());
		acc = std::to_string(acc_rps(counts));
		cc->respond(acc);
		return ;
	}
	
	cc->respond(acc);
	return ;
}
