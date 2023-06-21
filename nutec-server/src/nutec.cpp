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
		sndMsg(cc->sockfd,"Unable to enable the motor");
		return ;
	}
	handle->enabled = true;
	sndMsg(cc->sockfd);
	
}
void disable(instHandle *handle,cmd *cc)
{
	if (setRegister(handle,"0x24",0)!=0)
	{
		sndMsg(cc->sockfd,"Unable to enable the motor");
		return ;
	}
	handle->enabled = false;
	sndMsg(cc->sockfd);
	
}
void isEnabled(instHandle *handle,cmd *cc)
{
	int enabled=0;
	if (readRegister(handle,"0x24",&enabled)!=0)
	{
		sndMsg(cc->sockfd,"Unable to enable the motor",uicsCMD_ERR_VALUE);
		return;
	}
	if (enabled==21)
	{
		handle->enabled = true;
		sndMsg(cc->sockfd,"T");
		return;
	}
	else if (enabled==0){
		handle->enabled = false;
		sndMsg(cc->sockfd,"F");
		return;
	}
	else {
		sndMsg(cc->sockfd,"Motor status is "+std::to_string(enabled),uicsCMD_ERR_VALUE);
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
void status_t(instHandle *handle)
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
				
				handle->position = pos;
			}
			//read the position
			if (readRegister_32(handle,"0xa0",&xa0)==0)
			{
				handle->xa0 = xa0;
				handle->lim_p = ( (xa0 & 512) == 512 ) ? true : false ;
				handle->lim_n = ( (xa0 & 1024) == 1024 ) ? true : false ;
				handle->moving = ( (xa0 & 134217728) == 134217728 ) ? true : false ;
				handle->enabled = ( (xa0 & 4096) == 4096 ) ? true : false ;
				handle->phase_error = ( (xa0 & 64) == 64 ) ? true : false ;
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
		answ = answ.substr(2,answ.length());
		return toInt(answ, value);
	}
	else if (answ.substr(0,1).compare("e")==0) 
	{
		answ = answ.substr(2,answ.length());
		toInt(answ, value);
		return -1;
	}
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
		answ = answ.substr(2,answ.length());
		
		return toInt32(answ, value);
	}
	else if (answ.substr(0,1).compare("e")==0) 
	{
		answ = answ.substr(2,answ.length());
		toInt32(answ, value);
		return -1;
	}
	
	return -1;
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
		if (!isdigit(c))
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
		if (!isdigit(c))
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
			sndMsg(cc->sockfd,"Unable to write command",uicsCMD_ERR_VALUE);
			return ;
		}
		if (handle->sport.lirec(&answ,'\r')==0)
		{
			sndMsg(cc->sockfd,"Unable to read command",uicsCMD_ERR_VALUE);
			return ;
		}
		serial_lock.unlock();
		sndMsg(cc->sockfd,answ);
		return;
	}
	else if ((*cc)["write"].compare("")!=0 && (*cc)[">"].compare("")!=0){
                
		mycmd="s "+(*cc)[">"]+" "+(*cc)["write"]+"\r";
		serial_lock.lock();
		if (handle->sport.ecrireport(mycmd)!=OK)
		{
			sndMsg(cc->sockfd,"Unable to write command",uicsCMD_ERR_VALUE);
			return ;
		}
		if (handle->sport.lirec(&answ,'\r')==0)
		{
			sndMsg(cc->sockfd,"Unable to read command",uicsCMD_ERR_VALUE);
			return ;
		}
		serial_lock.unlock();
		sndMsg(cc->sockfd,answ);
		return;
	}
	sndMsg(cc->sockfd,"read the doc",uicsCMD_ERR_VALUE);
	return ;	
}

void abort(instHandle *handle,cmd *cc)
/*
 * Abort a movement.
 */ 
{
		if (trigger_abort(handle)!=0)
		{
		
			sndMsg(cc->sockfd,"Unable to abort a movement",uicsCMD_ERR_VALUE);
			return ;
		}
	sndMsg(cc->sockfd);
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
		sndMsg(cc->sockfd,"wrong argument",uicsCMD_ERR_VALUE);
		return;
	}
	position = (*cc)["position"];
	//set absolute mode
	if (setRegister(handle,"0xca",position)!=0)
	{
		sndMsg(cc->sockfd,"Unable to trigger a relatif move",uicsCMD_ERR_VALUE);
		return ;
	}
	if (trigger_move(handle)!=0)
	{
		sndMsg(cc->sockfd,"Unable to trigger a relatif move",uicsCMD_ERR_VALUE);
		return ;
	}
	sndMsg(cc->sockfd);
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
	string speed="";
	 
	if ( (*cc)["speed"].compare("")==0 )
	{
		sndMsg(cc->sockfd,"wrong argument",uicsCMD_ERR_VALUE);
		return;
	}
	speed = (*cc)["speed"];
	//set absolute mode
	if (setRegister(handle,"0xcb",speed)!=0)
	{
		sndMsg(cc->sockfd,"Unable to set speed",uicsCMD_ERR_VALUE);
		return ;
	}
	
	sndMsg(cc->sockfd);
	return ;
}

void get_speed(instHandle *handle,cmd *cc)
/*
 * Will return the speed of the motor.
 */ 
{
	string speed="";
	 
	//set absolute mode
	if (readRegister(handle,"0xcb",&speed)!=0)
	{
		sndMsg(cc->sockfd,"Unable to set speed",uicsCMD_ERR_VALUE);
		return ;
	}
	
	sndMsg(cc->sockfd,speed);
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
	if (readRegister_32(handle,"0xa0",&xa0)!=0)
	{
		sndMsg(cc->sockfd,"Unable to set speed",uicsCMD_ERR_VALUE);
		return ;
	}
	if ((xa0 & 134217728) == 134217728)
	{
		sndMsg(cc->sockfd,"T");
		return ;
	}
	
	sndMsg(cc->sockfd,"F");
	return ;
}

void move_abs(instHandle *handle,cmd *cc)
{
	string position="",val="";
	 
	if ( (*cc)["position"].compare("")==0 )
	{
		sndMsg(cc->sockfd,"wrong argument",uicsCMD_ERR_VALUE);
		return;
	}
	position = (*cc)["position"];
	//set absolute mode
	if (setRegister(handle,"0xc8",1)!=0)
	{
		sndMsg(cc->sockfd,"Unable to set absolute move mode",uicsCMD_ERR_VALUE);
		return ;
	}
	if (setRegister(handle,"0xca",position)!=0)
	{
		sndMsg(cc->sockfd,"Unable to set position",uicsCMD_ERR_VALUE);
		return ;
	}
	
	if (trigger_move(handle)!=0)
	{
		sndMsg(cc->sockfd,"Unable to trigger move",uicsCMD_ERR_VALUE);
		return ;
	}
	//set absolute mode
	if (setRegister(handle,"0xc8",257)!=0)
	{
		sndMsg(cc->sockfd,"Unable to set relative move mode",uicsCMD_ERR_VALUE);
		return ;
	}
	sndMsg(cc->sockfd);
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
		sndMsg(cc->sockfd,"Unable to read position",uicsCMD_ERR_VALUE);
		return ;
	}	
	sndMsg(cc->sockfd,val);
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
	status+=string("Register 0xa0: ")+to_string(handle->xa0)+"\n";
	sndMsg(cc->sockfd,status);
	return ;
}
void g_status(instHandle *handle,cmd *cc)
/*
 * Return the status in a structure.
 */ 
{
	sndMsg(cc->sockfd);
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
		sndMsg(cc->sockfd);
		return;
	} 
	else if ( (*cc)["-open"].compare("") != 0 ){
		if ( (*cc)["-force"].compare("") != 0 ) {
			handle->sport.ouvrirport(handle->serial_port,handle->baudrate);
			sndMsg(cc->sockfd);
			return;
		}
		else {
		//do something	
			if (handle->sport.status!=OPEN)
			{
				handle->sport.ouvrirport(handle->serial_port,handle->baudrate);
				sndMsg(cc->sockfd);
				return;
			}
			else 
			{
				sndMsg(cc->sockfd,"Serial port already open",uicsCMD_ERR_VALUE);
				return;
			}
		}
		
	}
	sndMsg(cc->sockfd,"Wrong argument",uicsCMD_ERR_VALUE);
	return ;
}