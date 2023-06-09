#include "nutec.h"

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
	
	if (handle->sport.ecrireport(mycmd)!=OK)
	{
		return -1;
	}
	if (handle->sport.lirec(&answ,'\r')==0)
	{
		return -1;
	}
	if (answ.substr(0,1).compare("v")==0)
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
	
	if (handle->sport.ecrireport(mycmd)!=OK)
	{
		return -1;
	}
	if (handle->sport.lirec(&answ,'\r')==0)
	{
		return -1;
	}
	if (answ.substr(0,1).compare("v")==0)
	{
		return 0;
	}
	return -1;
	
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
	
	if (handle->sport.ecrireport(mycmd)!=OK)
	{
		return -1;
	}
	if (handle->sport.lirec(&answ,'\r')==0)
	{
		return -1;
	}
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
	
	if (handle->sport.ecrireport(mycmd)!=OK)
	{
		return -1;
	}
	if (handle->sport.lirec(&answ,'\r')==0)
	{
		return -1;
	}
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
		sndMsg(cc->sockfd,answ);
		return;
	}
	else if ((*cc)["write"].compare("")!=0 && (*cc)[">"].compare("")!=0){
                
		mycmd="s "+(*cc)[">"]+" "+(*cc)["write"]+"\r";

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
	if (handle->sport.ecrireport("t 1\r")!=OK)
	{
		return -1;
	}
	if (handle->sport.lirec(&val,'\r')==0)
	{
		return -1;
	}
	return 0;
}
int trigger_abort(instHandle *handle)
/*
 * Stop a movement
 */ 
{
	string val="";
	if (handle->sport.ecrireport("t 0\r")!=OK)
	{
		return -1;
	}
	if (handle->sport.lirec(&val,'\r')==0)
	{
		return -1;
	}
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
{
	string status="";
	int s=0;
	//set absolute mode
	if (readRegister(handle,"0xa0",&status)!=0)
	{
		sndMsg(cc->sockfd,"Unable to set speed",uicsCMD_ERR_VALUE);
		return ;
	}
	if (toInt(status,&s)!=0)
	{
		sndMsg(cc->sockfd,"Unable to read status register",uicsCMD_ERR_VALUE);
		return ;
	}
	
	if ((s & 134217728) == 134217728)
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

