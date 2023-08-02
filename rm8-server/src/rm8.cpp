#include "rm8.h"

mutex usb_lock;


bool writeCommand(instHandle *handle,string cmd)
/*
 * Description
 * ------------
 * 	Write a command to the NSC-A1 controller.
 * 
 * Returns
 * -------
 * 	successfull -> true
 *  failed		-> false
 */ 
{
	
	
	//char lpDeviceString[PERFORMAX_MAX_DEVICE_STRLEN];
	char w_buffer[IOBUFFSIZE];
	char r_buffer[IOBUFFSIZE];
	string answ="";
	strcpy(w_buffer, cmd.c_str()); //set low speed
	usb_lock.lock();
	if(!fnPerformaxComSendRecv(handle->handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		usb_lock.unlock();
		fprintf(stderr,"Could not send command to motors\n");
		return false;
	}
	usb_lock.unlock();
	answ = string(r_buffer);
	if (answ.substr(0,2).compare("OK")!=0)
	{
		return false;
	}
	return true;
}

bool readCommand(instHandle *handle,string cmd,string *recv)
/*
 * Description
 * ------------
 * 	read a command to the NSC-A1 controller.
 * 
 * Returns
 * -------
 * 	successfull -> true
 *  failed		-> false
 */ 
{
	
	
	//char lpDeviceString[PERFORMAX_MAX_DEVICE_STRLEN];
	char w_buffer[IOBUFFSIZE];
	char r_buffer[IOBUFFSIZE];
	*recv = "";
	strcpy(w_buffer, cmd.c_str()); //set low speed
	usb_lock.lock();
	if(!fnPerformaxComSendRecv(handle->handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		usb_lock.unlock();
		fprintf(stderr,"Could not send command to motors\n");
		return false;
	}
	usb_lock.unlock();
	*recv = string(r_buffer);
	/*
	if (answ.substr(0,2).compare("OK")!=0)
	{
		return false;
	}*/
	
	return true;
}

int connect(instHandle *handle) 
/*
 * Description
 * -----------
 * 	Try to establish the communication via USB with the RM8
 * 	then we set the default low speed (10,000 pulse/sec) and
 * 	the default high speed (20,000 pulse/sec). We set the 
 *	acceleration to 100 msec and we enable the motor power.
 *	We then set the motor to absolute mode. Finaly, we reset
 * 	the current position to 0 and we enable the s-curve acc-
 * 	eleration mode.
 * 
 * Return code
 * -----------
 *	0 connection established successfully
 * 	1 COMM error
 * 	2 No motor found
 * 	3 Generic Error, check stderr
 *  4 CMD failed
 * 
 */ 
{
	char lpDeviceString[PERFORMAX_MAX_DEVICE_STRLEN];
	char w_buffer[IOBUFFSIZE];
	char r_buffer[IOBUFFSIZE];
	AR_DWORD num;
	
	handle->active = false;

	memset(w_buffer,0,IOBUFFSIZE);
	memset(r_buffer,0,IOBUFFSIZE);

	//acquire information
	if(!fnPerformaxComGetNumDevices(&num))
	{
		fprintf(stderr,"error in fnPerformaxComGetNumDevices\n");
		return 1;
	}
	if(num<1)
	{
		fprintf(stderr,"No motor found (Probably not powered on)\n");
		return 2;
	}

	if( !fnPerformaxComGetProductString(0, lpDeviceString, PERFORMAX_RETURN_SERIAL_NUMBER) ||
		!fnPerformaxComGetProductString(0, lpDeviceString, PERFORMAX_RETURN_DESCRIPTION) )
	{
		fprintf(stderr,"error acquiring product string\n");
		return 1;
	}
	
	//setup the connection
	
	if(!fnPerformaxComOpen(0,&handle->handle))
	{
		fprintf(stderr,"Error opening device\n");
		return 1;
	}
	handle->active = true;
	if(!fnPerformaxComSetTimeouts(2000,2000))
	{
		fprintf(stderr,"Error setting timeouts\n");
		return 3;
	}
	if(!fnPerformaxComFlush(handle->handle))
	{
		fprintf(stderr,"Error flushing the coms\n");
		return 1;
	}
	//We're connected, lets flag the connection
	handle->active = true;
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:::   Set the controller in baseline configuration   :::
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	//set low speed
	if (!writeCommand(handle,"LSPD=5000")){return 4;}
	//set high speed
	if (!writeCommand(handle,"HSPD=10000")){return 4;}
	//set acceleration
	if (!writeCommand(handle,"ACC=300")) {return 4;}
	//Enable the drive
	if (!writeCommand(handle,"EO=1")){return 4;}
	//set absolute mode
	if (!writeCommand(handle,"ABS")){return 4;}
	//Set current position to 0
	if (!writeCommand(handle,"PX=0")){return 4;}
	//Enable POL output
	if (!writeCommand(handle,"SCV=1")){return 4;}

	return 0;	
}

void abort(instHandle *handle,cmd *cc)
/*
 * Description
 * -----------
 * 	When motor is moving, jogging, or homing, 
 *  using the ABORT command will immediately stop 
 *  the motor.
 */ 
{
	if (!writeCommand(handle,"ABORT"))
	{
        
		cc->respond("Unable to abort movement",uicsCMD_ERR_VALUE);
		return ;
	}	
	cc->respond();
	return ;	
}


void enable_drive(instHandle *handle,cmd *cc)
/*
 * Description
 * -----------
 * 	Enable the motor, by default (at startup)
 *  the drive is enable.

 */ 
{
	if (!writeCommand(handle,"EO=1"))
	{
		cc->respond("Unable to enable the drive",uicsCMD_ERR_VALUE);
		return ;
	}	
	cc->respond();
	return ;	
	
}

void disable_drive(instHandle *handle,cmd *cc)
/*
 * Description
 * -----------
 * 	Disable the motor, by default (at startup)
 *  the drive is enable.

 */ 
{
	if (!writeCommand(handle,"EO=0"))
	{
		cc->respond("Unable to disable the drive",uicsCMD_ERR_VALUE);
		return ;
	}	
	cc->respond();
	return ;	
	
}


void read_position(instHandle *handle,cmd *cc)
/*
 * Description
 * -----------
 * 	Read the motor position 
 * 	position 	-> motor position
 */ 
{
	
	string answ="";
	
	if (!readCommand(handle,"PX",&answ)){
		cc->respond("Unable to read position",uicsCMD_ERR_VALUE);
		return ;}
	cc->respond(answ);
	return ;
}

bool readCommand_int(instHandle *handle,string cmd,int *recv)
/*
 * Description
 * ------------
 * 	read a command to the NSC-A1 controller.
 * 
 * Returns
 * -------
 * 	successfull -> true
 *  failed		-> false
 */ 
{
	
	
	//char lpDeviceString[PERFORMAX_MAX_DEVICE_STRLEN];
	char w_buffer[IOBUFFSIZE];
	char r_buffer[IOBUFFSIZE];
	strcpy(w_buffer, cmd.c_str()); //set low speed
	usb_lock.lock();
	if(!fnPerformaxComSendRecv(handle->handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{usb_lock.unlock();
		fprintf(stderr,"Could not send command to motors\n");
		return false;
	}
	usb_lock.unlock();
	for (auto &c: string(r_buffer))
	{
		if (!isdigit(c) and c!='-')
		{
			return false;
		}
	}
	*recv = std::stoi(string(r_buffer).c_str());
	return true;
}

void p_status(instHandle *handle,cmd *cc)
{
	string status="";
	status+=string("Position: ")+to_string(handle->position)+"\n";
	status+=string("Drive Enabled: ")+ ((handle->drive_enabled) ? "T" : "F")  +"\n";
	status+=string("Com. Active: ")+ ((handle->active) ? "T" : "F" ) +"\n";
	status+=string("Moivng: ")+ ((handle->moving) ? "T" : "F" ) +"\n";
	status+=string("Lim +: ")+ ((handle->lim_p) ? "T" : "F" ) +"\n";
	status+=string("Lim -: ")+ ((handle->lim_n) ? "T" : "F")  +"\n";
	status+=string("Lim Home: ")+ ((handle->lim_home) ? "T" : "F")  +"\n";
	status+=string("Motor Status: ")+to_string(handle->mst)+"\n";
	cc->respond(status);
	return ;
	
	
}

void g_status(instHandle *handle,cmd *cc)
{
	char *encodedStructure=NULL;
	rm8_telemetry tlm;
	size_t length=0;
	//::::::::::::::::::::::::::::::
	//:::   copy the structure   :::
	//::::::::::::::::::::::::::::::
	tlm.position = handle->position;
	tlm.drive_enabled = handle->drive_enabled;
	tlm.active = handle->active;//the serial communication is established, position is updated.
	tlm.moving = handle->moving;
	tlm.lim_p = handle->lim_p;
	tlm.lim_n = handle->lim_n;
	tlm.lim_home = handle->lim_home;

	length = islb64EncodeAlloc((const char *)&tlm,sizeof(tlm),&encodedStructure);
	cc->respond(std::to_string(length)+" "+std::string(encodedStructure));
	return ;
	
}

void status_t(instHandle *handle)
{
	int fd = create_socket(4569);
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


void motor_status_t(instHandle *handle)
{
	int mst,position,enabled;
	while(1){
		delay(1000);
		if (handle->active){
			mst=0;
			position=0;
			enabled=0;
			if (readCommand_int(handle,"MST",&mst))
			{
				handle->mst = static_cast<uint16_t>(mst);
				handle->lim_n = ((mst& 16) == 16) ? true : false;
				handle->lim_p = ((mst& 32) == 32) ? true : false;
				handle->lim_home = ((mst& 8) == 8) ? true : false;
				handle->moving = (((mst & 1) == 1) || ((mst & 2) == 2) || ((mst & 4) == 4));
			}
			if (readCommand_int(handle,"PX",&position))
			{
				handle->position = position;
			}
			if (readCommand_int(handle,"EO",&enabled))
			{
				handle->drive_enabled = (enabled==1) ? true : false;
			}
		}
	}
	
}


void motor_status(instHandle *handle,cmd *cc)
/*
 * Description
 * -----------
 * 	Read the motor encoder 
 * 	encoder 	-> encoder position
 * <0>-> Motor running at constant speed
 * <1>-> Motor in acceleration
 * <2>-> Motor in deceleration
 * <3>-> Home input switch status
 * <4>-> Minus limit input switch status
 * <5>-> Plus limit input switch status
 * <6>-> Minus limit error. This error must be cleared using the CLR command
 * <7>-> Plus limit error. This error must be cleared using the CLR command
 * <8>-> Latch input status
 * <9>-> Z-index status
 * <10>-> TOC time-out status
 * 
 */ 
{
	string answ="";
	if (!readCommand(handle,"MST",&answ))
	{
		cc->respond("Unable to read motor status",uicsCMD_ERR_VALUE);
		return ;
	}
	cc->respond(answ);
	return ;
	
}

void clear_limit(instHandle *handle,cmd *cc)
/*
 * Description
 * -----------
 * Clear the latched limit error. 
 * 
 */ 
{
	if (!writeCommand(handle,"CLR"))
	{
		cc->respond("Unable to read motor status",uicsCMD_ERR_VALUE);
		return ;
	}
	cc->respond();
	return ;
	
}

void serialio(instHandle *handle,cmd *cc)
{	
	string mycmd="";
	if ( (*cc)["write"].compare("")!=0  ){
		
		char w_buffer[IOBUFFSIZE];
		char r_buffer[IOBUFFSIZE];
		string answ="";
		strcpy(w_buffer, (*cc)["write"].c_str()); //set low speed
		usb_lock.lock();
		if(!fnPerformaxComSendRecv(handle->handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
		{usb_lock.unlock();
			fprintf(stderr,"Could not send command to motors\n");
			return;
		}
		usb_lock.unlock();
		answ = string(r_buffer);
		cc->respond(answ);
		return ;
	}
	cc->respond("Unable to read",uicsCMD_ERR_VALUE);
	return ;
}

void get_high_speed(instHandle *handle,cmd *cc)
{
	string answ="";
	
	if (!readCommand(handle,"HSPD",&answ))
	{
		cc->respond("unable to rea dhigh speed",uicsCMD_ERR_VALUE);
		return;
	}
	cc->respond(answ);
	return;
	
}
void get_low_speed(instHandle *handle,cmd *cc)
{
	string answ="";
	
	if (!readCommand(handle,"LSPD",&answ))
	{
		cc->respond("unable to read low speed",uicsCMD_ERR_VALUE);
		return;
	}
	cc->respond(answ);
	return;
	
}
void get_acceleration(instHandle *handle,cmd *cc)
{
	string answ="";
	
	if (!readCommand(handle,"ACC",&answ))
	{
		cc->respond("unable to read accelerartion",uicsCMD_ERR_VALUE);
		return;
	}
	cc->respond(answ);
	return;
	
}

void loop(instHandle *handle,cmd *cc)
/*
 * Description
 * -----------
 * 	Try to perform an on-the-fly target position change, if 
 *  not successfull, we try to perform a simple position change.
 *  
 *  This function is meant to be used inside the guiding loop.
 *  It is not clear if we should use absolute movement in 
 *  combination with the get_position function for more accurate 
 *  results.
 * 
 * NOTE
 * -------
 * On-the-fly target position change can be achieved using the T[value] command. While
 * the motor is moving, T[value] will change the final destination of the motor. If the motor
 * has already passed the new target position, it will reverse direction once the target
 * position change command is issued.
 * 
 * If a T command is sent while the controller is not performing a target move, the
 * command is not processed. Instead, an error response is returned	
*/	
{

	int c_position=0,n_position=0,position=0,x=0;
	string t_command="",x_command="";
	
	if (!isInt((*cc)["position"])){
		cc->respond("Position must be integer",uicsCMD_ERR_VALUE);
		return ;
	}
	x = std::atoi((*cc)["position"].c_str());
	
	
	if (!readCommand_int(handle,"PX",&position))
	{
		cc->respond("Unable to read motor position",uicsCMD_ERR_VALUE);
		return ;
	}
	
	c_position = position;
	n_position = position+x;
	t_command = "T"+to_string(n_position);
	x_command = "X"+to_string(n_position);
	//try to do a change of position T[value] if 
	//it fail we will try the X[value] cmd
	if (writeCommand(handle,t_command))
	{
		cc->respond();
		return ;
	}
	else if (writeCommand(handle,x_command))
	{
		cc->respond();
		return ;
	}
	else {
		cc->respond("Unable to move the motor",uicsCMD_ERR_VALUE);
		return ;
	}
	
}
void loop_no_return(instHandle *handle,cmd *cc)
/*
 * Description
 * -----------
 * 	Try to perform an on-the-fly target position change, if 
 *  not successfull, we try to perform a simple position change.
 *  
 *  This function is meant to be used inside the guiding loop.
 *  It is not clear if we should use absolute movement in 
 *  combination with the get_position function for more accurate 
 *  results.
 * 
 * NOTE
 * -------
 * On-the-fly target position change can be achieved using the T[value] command. While
 * the motor is moving, T[value] will change the final destination of the motor. If the motor
 * has already passed the new target position, it will reverse direction once the target
 * position change command is issued.
 * 
 * If a T command is sent while the controller is not performing a target move, the
 * command is not processed. Instead, an error response is returned	
*/	
{

	int c_position=0,n_position=0,position=0,x=0;
	string t_command="",x_command="";
	
	if (!isInt((*cc)["position"])){
		return ;
	}
	x = std::atoi((*cc)["position"].c_str());
	
	
	if (!readCommand_int(handle,"PX",&position))
	{
		return ;
	}
	
	c_position = position;
	n_position = position+x;
	t_command = "T"+to_string(n_position);
	x_command = "X"+to_string(n_position);
	//try to do a change of position T[value] if 
	//it fail we will try the X[value] cmd
	if (writeCommand(handle,t_command))
	{
		printf("Switch this statement!\n");
		return ;
	}
	else if (writeCommand(handle,x_command))
	{
		return ;
	}
	
		return ;
	
	
}
bool isInt(string INT)
{
	for (auto &c: INT)
		{
			if (!isdigit(c) and c!='-')
			{
				return false;
			}
		}	
	return true;
}
void set_low_speed(instHandle *handle,cmd *cc)
{
	string C="LSPD=";
	
	if (!isInt( (*cc)["speed"] ))
	{
		cc->respond("argument must be a integer",uicsCMD_ERR_VALUE);
		return ;
	}
	C+=(*cc)["speed"];
	if (!writeCommand(handle,C))
	{
		cc->respond("Unable to set low speed",uicsCMD_ERR_VALUE);
		return ;
	}
	cc->respond();
	return ;
}
void set_high_speed(instHandle *handle,cmd *cc)
{
	string C="HSPD=";
	
	if (!isInt( (*cc)["speed"] ))
	{
		cc->respond("argument must be a integer",uicsCMD_ERR_VALUE);
		return ;
	}
	C+=(*cc)["speed"];
	if (!writeCommand(handle,C))
	{
		cc->respond("Unable to set low speed",uicsCMD_ERR_VALUE);
		return ;
	}
	cc->respond();
	return ;
}

void set_acceleration(instHandle *handle,cmd *cc)
{
	string C="ACC=";
	
	if (!isInt( (*cc)["acceleration"] ))
	{
		cc->respond("argument must be a integer",uicsCMD_ERR_VALUE);
		return ;
	}
	C+=(*cc)["acceleration"];
	if (!writeCommand(handle,C))
	{
		cc->respond("Unable to set low speed",uicsCMD_ERR_VALUE);
		return ;
	}
	cc->respond();
	return ;
}


void setPosition(instHandle *handle,cmd *cc)
/*	Description
 * 	-----------
 * 	Set the motor absolute position.
 *
 * Return 
 * ------
 * 	0->success 
 *	1 - > communication problem
 *  2 -> command failed.
 */ 
{

	string C="PX=";
	
	if (!isInt( (*cc)["position"] ))
	{
		cc->respond("argument must be a integer",uicsCMD_ERR_VALUE);
		return ;
	}
	C+=(*cc)["position"];
	
	if (!writeCommand(handle,C))
	{
		cc->respond("Unable to set position",uicsCMD_ERR_VALUE);
		return ;
	}
	cc->respond();
	return ;
}

void isMoving(instHandle *handle,cmd *cc)
{
	int mst=0;
	string ret = "";
	if (!readCommand_int(handle,"MST",&mst))
	{
		cc->respond("Unable to read MST",uicsCMD_ERR_VALUE);
		return;
	}
	ret = (((mst & 1) == 1) || ((mst & 2) == 2) || ((mst & 4) == 4)) ? "T" : "F";
	
	cc->respond(ret);
	return;
}
void delay(int ms)
/*!
  *\brief Delay in ms
  * Create a delay. ms is delay time in millisecond
  */
{
   this_thread::sleep_for(chrono::milliseconds(ms));
}


void usb(instHandle *handle,cmd *cc)
{
	if ((*cc)["-open"].compare("")!=0 && !handle->active)
	{
		if (connect(handle)!=0)
		{
			cc->respond("Unable");
			return;
		}
		else {cc->respond();return;}
	}
	else if ((*cc)["-open"].compare("")!=0 && (*cc)["-force"].compare("")!=0)
	{
		if (connect(handle)!=0)
		{
			cc->respond("Unable to connect the ");
			return;
		}
		else {cc->respond();return;}
	}
	else if ((*cc)["-close"].compare("")!=0)
	{
		if (!writeCommand(handle,"ABORT"))
		{
			cc->respond("Unable to abort movement",uicsCMD_ERR_VALUE);
			return ;
		}
		usb_lock.lock();
		if(!fnPerformaxComClose(handle->handle))
		{usb_lock.unlock();
			fprintf(stderr, "Error Closing\n");
			cc->respond("Unable to close connection",uicsCMD_ERR_VALUE);
			return ;
		}
		usb_lock.unlock();
		handle->active=false;
		cc->respond();
		return;
	}
}

//Closes the connection to the motor
void closeConnection(instHandle *handle,cmd *cc)
/*
 * Description
 * -----------
 * Close the communication with the NSC-A1. 
 * 
 * NOTE
 * ----
 * we make sure the motor is not moving by sending
 * the ABORT command.
 * 
 * return 
 * ------
 * 0 -> successfull
 * 1 -> Communication problem
 * 2 -> Failed to send the command
 * 3 -> failed to disconnect
 * 
 */ 
{
	if (!writeCommand(handle,"ABORT"))
	{
		cc->respond("Unable to abort movement",uicsCMD_ERR_VALUE);
		return ;
	}
	usb_lock.lock();
	if(!fnPerformaxComClose(handle->handle))
	{usb_lock.unlock();
		fprintf(stderr, "Error Closing\n");
		cc->respond("Unable to close connection",uicsCMD_ERR_VALUE);
		return ;
	}
	usb_lock.unlock();
	handle->active=false;
	cc->respond();
	
	
	return ;
}
























