/*
File: code_rm.c
Author: Newmark
Modified by: Philippe Truchon
Date: 09/08/22  
Desc: This file contains the methods to communicate with the NSC-A1 controller   
state: WORK IN PROGRESS
*/

#include "rm8_driver.h"
//Refer NSC-A1 user_guide in documentation_externe to know what the commands mean


int sndCmd(char snd[],char *recv)
/*
 * Description
 * -----------
 * 	Send a command and read back the answer from the NSC-A1
 *  controller
 *  
 * 
 * Return
 * ------
 * 	0 if successfull
 * 	1 if we have a communication error
 * 
 */ 
{
	char w_buffer[IOBUFFSIZE];
	char r_buffer[IOBUFFSIZE];

	memset(w_buffer,0,IOBUFFSIZE);
	memset(r_buffer,0,IOBUFFSIZE);
	strncpy(w_buffer,snd,IOBUFFSIZE);//we want to make sure the string is nulll-terminated

	

	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	strcpy(recv,r_buffer);
	
	return 0;	
}


int stopall(void)
/*
 * Description
 * -----------
 * 	When motor is moving, jogging, or homing, 
 *  using the ABORT command will immediately stop 
 *  the motor.
 *  
 * 
 * Return
 * ------
 * 	0 if successfull
 * 	1 if we have a communication error
 * 	2 motor is probably not moving
 */ 
{
	char w_buffer[IOBUFFSIZE];
	char r_buffer[IOBUFFSIZE];

	memset(w_buffer,0,IOBUFFSIZE);
	memset(r_buffer,0,IOBUFFSIZE);
	

	
	strcpy(w_buffer, "ABORT"); //read ID
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		fprintf(stderr,"Motor is probably already stopped\n");
		return 2;	
	}
	
	return 0;	
}

int enable_drive(void)
/*
 * Description
 * -----------
 * 	Enable the motor, by default (at startup)
 *  the drive is enable.
 *  
 * 
 * Return
 * ------
 * 	0 if successfull
 * 	1 if we have a communication error
 *  2 fail to enable the drive
 */ 
{
	char w_buffer[IOBUFFSIZE];
	char r_buffer[IOBUFFSIZE];

	memset(w_buffer,0,IOBUFFSIZE);
	memset(r_buffer,0,IOBUFFSIZE);
	

	
	strcpy(w_buffer, "EO=1"); //read ID
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		fprintf(stderr,"Unable to enable the drive\n");
		return 2;	
	}
	
	return 0;	
}
int disable_drive(void)
/*
 * Description
 * -----------
 * 	Disable the motor, by default (at startup)
 *  the drive is enable.
 *  
 * 
 * Return
 * ------
 * 	0 if successfull
 * 	1 if we have a communication error
 *  2 fail to disable the drive
 */ 
{
	char w_buffer[IOBUFFSIZE];
	char r_buffer[IOBUFFSIZE];

	memset(w_buffer,0,IOBUFFSIZE);
	memset(r_buffer,0,IOBUFFSIZE);
	

	
	strcpy(w_buffer, "EO=0"); //read ID
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		fprintf(stderr,"Unable to disable the drive\n");
		return 2;	
	}
	
	return 0;	
}

int isOK(char *r_buffer)
/*
 * Return 1 if the 1st two letters of r_buffer 
 * is OK.
 */ 
{
	if (r_buffer[0]=='O' && r_buffer[1]=='K')
	{return 1;}
	return 0;
	
}
int read_position(char *position)
/*
 * Description
 * -----------
 * 	Read the motor position 
 * 	position 	-> motor position
 *  
 * 
 * Return
 * ------
 * 	0 if successfull
 * 	1 if we have a communication error
 */ 
{
	char w_buffer[IOBUFFSIZE];
	char r_buffer[IOBUFFSIZE];

	memset(w_buffer,0,IOBUFFSIZE);
	memset(r_buffer,0,IOBUFFSIZE);
	

	
	strcpy(w_buffer, "PX"); //read ID
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	strcpy(position,r_buffer);
	
	return 0;	
}
int getAnalogueInput(char *input1,char *input2)
/*
 * Description
 * -----------
 * 	Read the analogue 1 and analogue 2 input in mV.
 * Return 
 * -------
 * 	0 uppon success
 */ 
{
	char w_buffer[IOBUFFSIZE];
	char r_buffer[IOBUFFSIZE];

	memset(w_buffer,0,IOBUFFSIZE);
	memset(r_buffer,0,IOBUFFSIZE);
	

	
	strcpy(w_buffer, "AI1"); //read ID
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	strcpy(input1,r_buffer);
	strcpy(w_buffer, "AI2"); //read ID
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	strcpy(input2,r_buffer);
	return 0;
	
}

int status(int *motor_status)
/*
 * Description
 * -----------
 * 	Read the motor encoder 
 * 	encoder 	-> encoder position
 *  
 * 
 * Return
 * ------
 * 	0 if successfull
 * 	1 if we have a communication error
 */ 
{
	char w_buffer[IOBUFFSIZE];
	char r_buffer[IOBUFFSIZE];

	memset(w_buffer,0,IOBUFFSIZE);
	memset(r_buffer,0,IOBUFFSIZE);
	

	
	strcpy(w_buffer, "MST"); //read ID
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	if (isInt(r_buffer,IOBUFFSIZE))
	{
		*motor_status = atoi(r_buffer);
	}
	else {*motor_status = -1;}
	

	return 0;	
}
// Connects to the motor
int get_info(char *dev_serial,char *id,char *name)
/*
 * Description
 * -----------
 * 	Fetch information on the Motor. 
 * 	dev_serial 	-> device serial number
 *  id 			-> device id
 *  name		-> device name
 * 
 * Return
 * ------
 * 	0 if successfull
 * 	1 if we have a communication error
 */ 
{
	char serial[PERFORMAX_MAX_DEVICE_STRLEN];
	char w_buffer[IOBUFFSIZE];
	char r_buffer[IOBUFFSIZE];

	memset(w_buffer,0,IOBUFFSIZE);
	memset(r_buffer,0,IOBUFFSIZE);
	memset(serial,0,PERFORMAX_MAX_DEVICE_STRLEN);
	
	if( !fnPerformaxComGetProductString(0, serial, PERFORMAX_RETURN_SERIAL_NUMBER))
		
	{
		fprintf(stderr,"error acquiring product string\n");
		return 1;
	}
	strcpy(dev_serial,serial);
	
	strcpy(w_buffer, "ID"); //read ID
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	strcpy(id,r_buffer);
	//printf("Arcus Product: %s\n",in);

	strcpy(w_buffer, "DN"); //read Device name
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	strcpy(name,r_buffer);
	
	return 0;	
}
int connect(void) 
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
	
	if(!fnPerformaxComOpen(0,&Handle))
	{
		fprintf(stderr,"Error opening device\n");
		return 1;
	}
	
	if(!fnPerformaxComSetTimeouts(2000,2000))
	{
		fprintf(stderr,"Error setting timeouts\n");
		return 3;
	}
	if(!fnPerformaxComFlush(Handle))
	{
		fprintf(stderr,"Error flushing the coms\n");
		return 1;
	}
	
	// setup the device	
	strcpy(w_buffer, "LSPD=10000"); //set low speed
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		fprintf(stderr,"CMD NOK\n");
		return 4;
	}
	
	strcpy(w_buffer, "HSPD=20000"); //set high speed
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		fprintf(stderr,"CMD NOK\n");
		return 4;
	}

	strcpy(w_buffer, "ACC=100"); //set acceleration
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		fprintf(stderr,"CMD NOK\n");
		return 4;
	}

	strcpy(w_buffer, "EO=1"); //enable device
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		fprintf(stderr,"CMD NOK\n");
		return 4;
	}
 //Incremental mode no longer appears to be supported...
	strcpy(w_buffer, "ABS"); //set absolute mode
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
if (!isOK(r_buffer))
	{
		fprintf(stderr,"CMD NOK\n");
		return 4;
	}
	
	
	
	//printf("Device Number: %s\n",in);

	// TODO We seem to have to set a starting position
	strcpy(w_buffer, "PX=0"); //Set current position to 0
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
if (!isOK(r_buffer))
	{
		fprintf(stderr,"CMD NOK\n");
		return 4;
	}
	
	strcpy(w_buffer, "SCV=1"); //Enable POL output
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send command to motors\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		fprintf(stderr,"CMD NOK\n");
		return 4;
	}
	return 0;	
}

int loop(int x)
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
 *  Return
 * 	-------
 * 	0 if successfull
 * 	1 if we have a communication error
 *  2 command failed.
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
	char r_buffer[IOBUFFSIZE];
	char w_buffer[IOBUFFSIZE];
	memset(r_buffer,0,IOBUFFSIZE);
	memset(w_buffer,0,IOBUFFSIZE);
	
	int c_position=0;
	int n_position=0;
	
	//get the current position and calculate new target position.
	strcpy(w_buffer, "PX"); //move the motor	
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Unable to communicate with controller");
		return 1;
	}	
	if (!isInt(r_buffer,IOBUFFSIZE))
	{
		fprintf(stderr,"Unable to read the motor position");
		return 2;
	}
	c_position = atoi(r_buffer);
	n_position = c_position+x;
	
	//try to do a change of position T[value] if 
	//it fail we will try the X[value] cmd
	memset(w_buffer,0,IOBUFFSIZE);
	sprintf(w_buffer,"T%d",n_position);

	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Unable to communicate with controller");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		//probably the motor was not already moving we will try the X command
		memset(w_buffer,0,IOBUFFSIZE);
		sprintf(w_buffer,"X%d",n_position);
		if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
		{
			fprintf(stderr,"Unable to communicate with controller");
			return 1;
		}
		if (!isOK(r_buffer))
		{
			fprintf(stderr,"Unable to move the motor");
			return 2;			
		}
	}
	
	return 0;
}

// Move function for the loop function

int old_loop(int x){
	//char 		lpDeviceString[PERFORMAX_MAX_DEVICE_STRLEN];

	char		out[64];
	char		in[64];
	//AR_DWORD	num;
	//int i;
	char buf[20];
	//FILE *fptr;

	memset(out,0,64);
	memset(in,0,64);
	
	{
		
		
			if (in[0]!='O')
			{
			strcpy(out, "T0"); //move the motor
				if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
				{
					printf("Could not send\n");
					return 1;
				}
			}
			strcpy(out, "X0"); //move the motor
			if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
			{
				printf("Could not send\n");
				return 1;
			}
			
			sleep(0.01);
		
		memset(buf,0,IOBUFFSIZE);
		sprintf(buf, "PX=%d", x); // puts string into buffer
      	
		strcpy(out, buf); //Set current position to 0
		if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
		{
			printf("Could not send\n");
			return 1;
		}
		
		//strcpy(out, "PX"); //Read current position
		//if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
		//{
		//	printf("Could not send\n");
		//	return 1;
		//}
			//printf("Pos: %s\n",in);
			//fptr = fopen("/home/hicibas-clone/Desktop/test_philippe_rm8/share.txt","w");
			//fprintf(fptr,"%s",in);
			//fclose(fptr);
		
			if (in[0]!='O')
			{
			strcpy(out, "T0"); //move the motor
				if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
				{
					printf("Could not send\n");
					return 1;
				}
			}
			strcpy(out, "X0"); //move the motor
			if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
			{
				printf("Could not send\n");
				return 1;
			}
			
			//sleep(1);
		
		
	
	}
	return 0;
	}

int isInt(char *buffer,int buffer_length)
/*
 * Description
 * -----------
 * 	Check if the buffer string is an INT.
 * Return 
 * ------
 * 	T/F (1/0)
 */ 
{
	for (int i=0;i<buffer_length;i++)
	{	if (buffer[i]=='\0'){return 1;}
		if (!isdigit(buffer[i]) && buffer[i]!='-')
		{return 0;}
	}
	return 1;
	
}

int setMParameters(int LSPD,int HSPD,int ACC)
/*	Description
 * 	-----------
 * 	Set the motor most common motor parameters
 *  HSPD -> high speed
 *  LSPD -> Low speed
 *  ACC  -> acceleration
 *
 * Return 
 * ------
 * 	0->success 
 *	1 - > motor is probably already in movement
 *  2 -> command failed.
 */ 
{

	char c_HSPD[IOBUFFSIZE];
	char c_LSPD[IOBUFFSIZE];
	char c_ACC[IOBUFFSIZE];
	char r_buffer[IOBUFFSIZE];
	memset(r_buffer,0,IOBUFFSIZE);
	memset(c_ACC,0,IOBUFFSIZE);
	memset(c_LSPD,0,IOBUFFSIZE);
	memset(c_HSPD,0,IOBUFFSIZE);

	//set the parameters
	sprintf(c_LSPD,"LSPD=%d",LSPD);
	sprintf(c_HSPD,"HSPD=%d",HSPD);
	sprintf(c_ACC,"ACC=%d",ACC);

	
	if(!fnPerformaxComSendRecv(Handle, c_LSPD, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		printf("Could not send\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		
		fprintf(stderr,"Unable to set the low speed");
		return 2;
	}
	if(!fnPerformaxComSendRecv(Handle, c_HSPD, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		printf("Could not send\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		
		fprintf(stderr,"Unable to set the hi speed");
		return 2;
	}
	if(!fnPerformaxComSendRecv(Handle, c_ACC, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Communication problem\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		
		fprintf(stderr,"Unable to set the the acceleration");
		return 2;
	}
	
	return 0;
}
int setPosition(int position)
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

	char PX[IOBUFFSIZE];
	char r_buffer[IOBUFFSIZE];
	memset(r_buffer,0,IOBUFFSIZE);
	memset(PX,0,IOBUFFSIZE);

	//set the parameters
	sprintf(PX,"PX=%d",position);


	
	if(!fnPerformaxComSendRecv(Handle, PX, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		printf("Could not send\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{		
		fprintf(stderr,"Unable to set the absolute position");
		return 2;
	}
	
	return 0;
}
int move_wait(int x,int *new_position)
/*	Description
 * 	-----------
 * 		Make the motor move x-pulse with the follwing
 * 	configuration LSPD=5000, HSPD=10000 and ACC=300. 
 * 	The new position will be written to new_position 
 *  Note
 *  ----
 * 		(1) If move does not return 0, the position is not
 * 	valid. (2) This function will return only when the position
 *  is reached.
 * Return 
 * ------
 * 	0->success 
 *	1 - > motor is probably already in movement
 *  2 -> command failed.
 *  3 - > motor is probably already in movement
 */ 
{

	int c_position=0;
	int n_position=0;
	char r_buffer[IOBUFFSIZE];
	char w_buffer[IOBUFFSIZE];
	memset(r_buffer,0,IOBUFFSIZE);
	memset(w_buffer,0,IOBUFFSIZE);



	strcpy(w_buffer, "LSPD=5000"); //set low speed
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		printf("Could not send\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		
		fprintf(stderr,"Unable to set the low speed");
		return 2;
	}
	strcpy(w_buffer, "HSPD=10000"); //set high speed
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		printf("Could not send\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		
		fprintf(stderr,"Unable to set the hi speed");
		return 2;
	}
	strcpy(w_buffer, "ACC=300"); //set acceleration
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Communication problem\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		
		fprintf(stderr,"Unable to set the the acceleration");
		return 2;
	}
	
	if (read_position(r_buffer)!=0)
	{
		fprintf(stderr,"Communication problem\n");
		return 1;
	}
	//Check if we received an INT
	if (!isInt(r_buffer,IOBUFFSIZE))
	{
		fprintf(stderr,"Could not read current position\n");
		return 2;
	}
	c_position = atoi(r_buffer);//current positon
	
	n_position = c_position+x;//calculate the new target position
	memset(w_buffer,0,IOBUFFSIZE);
	sprintf(w_buffer,"X%d",n_position);
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		
		fprintf(stderr,"Unable to move the motor, the motor is probably already moving");
		return 3;
	}
	
	//wait for the motor to finish moving
	int mst=1024;
	status(&mst);
	
	while(mst>0 && mst<8)//we're moving
	{
		sleep(0.1);
		if (status(&mst)!=0){break;}
	}
	
	if (read_position(r_buffer)!=0)
	{
		fprintf(stderr,"Communication problem\n");
		return 1;
	}
	//Check if we received an INT
	
	if (!isInt(r_buffer,IOBUFFSIZE))
	{
		fprintf(stderr,"Could not read current position\n");
		return 2;
	}
	*new_position = atoi(r_buffer);
	
	return 0;
}

int move(int x)
/*	Description
 * 	-----------
 * 		Make the motor move x-pulse with the follwing
 * 	configuration LSPD=5000, HSPD=10000 and ACC=300. 
 * 	The new position will be written to new_position 
 *  Note
 *  ----
 * 		(1) If move does not return 0, the position is not
 * 	valid. (2) This function return immediatly. (3) If the 
 *  motor is running while this command is used, it will 
 *  return an error
 * Return 
 * ------
 * 	0->success 
 *	1 - > motor is probably already in movement
 *  2 -> command failed.
 *  3 - > motor is probably already in movement
 */ 
{

	int c_position=0;
	int n_position=0;
	char r_buffer[IOBUFFSIZE];
	char w_buffer[IOBUFFSIZE];
	memset(r_buffer,0,IOBUFFSIZE);
	memset(w_buffer,0,IOBUFFSIZE);



	strcpy(w_buffer, "LSPD=5000"); //set low speed
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		printf("Could not send\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		
		fprintf(stderr,"Unable to set the low speed");
		return 2;
	}
	strcpy(w_buffer, "HSPD=10000"); //set high speed
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		printf("Could not send\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		
		fprintf(stderr,"Unable to set the hi speed");
		return 2;
	}
	strcpy(w_buffer, "ACC=300"); //set acceleration
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Communication problem\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		
		fprintf(stderr,"Unable to set the the acceleration");
		return 2;
	}
	
	if (read_position(r_buffer)!=0)
	{
		fprintf(stderr,"Communication problem\n");
		return 1;
	}
	//Check if we received an INT
	if (!isInt(r_buffer,IOBUFFSIZE))
	{
		fprintf(stderr,"Could not read current position\n");
		return 2;
	}
	c_position = atoi(r_buffer);//current positon
	
	n_position = c_position+x;//calculate the new target position
	memset(w_buffer,0,IOBUFFSIZE);
	sprintf(w_buffer,"X%d",n_position);
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		fprintf(stderr,"Could not send\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		
		fprintf(stderr,"Unable to move the motor, the motor is probably already moving");
		return 3;
	}
	
	return 0;
}

 //Move function for the other functions

int old_move(int x){
	//char 		lpDeviceString[PERFORMAX_MAX_DEVICE_STRLEN];

	char		out[64];
	char		in[64];
	//AR_DWORD	num;
	//int i;
	char buf[20];
	//FILE *fptr;

	memset(out,0,64);
	memset(in,0,64);


	strcpy(out, "LSPD=5000"); //set low speed
	if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
	{
		printf("Could not send\n");
		return 1;
	}
	strcpy(out, "HSPD=10000"); //set high speed
	if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
	{
		printf("Could not send\n");
		return 1;
	}

	strcpy(out, "ACC=300"); //set acceleration
	if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
	{
		printf("Could not send\n");
		return 1;
	}
	sleep(0.1);
	{
		do
		{
			
			strcpy(out, "X0"); //move the motor
			if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
			{
				printf("Could not send\n");
				return 1;
			}
			
			sleep(0.01);
		} while(in[0]!='O'); //motor is still moving. wait and retry;
		memset(buf,0,IOBUFFSIZE);
		sprintf(buf, "PX=%d", x); // puts string into buffer
      	
		strcpy(out, buf); //Set current position to 0
		if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
		{
			printf("Could not send\n");
			return 1;
		}
		
		strcpy(out, "PX"); //Read current position
		if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
		{
			printf("Could not send\n");
			return 1;
		}
			//printf("Pos: %s\n",in);
			//fptr = fopen("/home/hicibas-clone/Desktop/test_philippe_rm8/share.txt","w");
			//fprintf(fptr,"%s",in);
			//fclose(fptr);
		do
		{	
			strcpy(out, "X0"); //move the motor
			if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
			{
				printf("Could not send\n");
				return 1;
			}
			
			//sleep(1);
		}while(in[0]!='O'); //motor is still moving. wait and retry;
		
	
	}
	return 0;
	}



 //Move function for the response_test

int move_resp(int x, int l, int h, int a){
	//char 		lpDeviceString[PERFORMAX_MAX_DEVICE_STRLEN];

	char		out[64];
	char		in[64];
	//AR_DWORD	num;
	//int i;
	char buf[20];
	char ls[20];
	char hs[20];
	char acc[20];
	//FILE *fptr;

	memset(out,0,64);
	memset(in,0,64);

	sprintf(ls, "LSPD=%d", l); // puts string into buffer
	sprintf(hs, "HSPD=%d", h); // puts string into buffer
	sprintf(acc, "ACC=%d", a); // puts string into buffer

	//printf("ls value: %s\n",l);


	strcpy(out, ls); //set low speed
	if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
	{
		printf("Could not send\n");
		return 1;
	}
	strcpy(out, hs); //set high speed
	if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
	{
		printf("Could not send\n");
		return 1;
	}

	strcpy(out, acc); //set acceleration
	if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
	{
		printf("Could not send\n");
		return 1;
	}
	sleep(0.1);
	{
		do
		{
			
			strcpy(out, "X0"); //move the motor
			if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
			{
				printf("Could not send\n");
				return 1;
			}
			
			sleep(0.01);
		} while(in[0]!='O'); //motor is still moving. wait and retry;
		memset(buf,0,IOBUFFSIZE);
		sprintf(buf, "PX=%d", x); // puts string into buffer
      	
		strcpy(out, buf); //Set current position to 0
		if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
		{
			printf("Could not send\n");
			return 1;
		}
		
		strcpy(out, "PX"); //Read current position
		if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
		{
			printf("Could not send\n");
			return 1;
		}
			//printf("Pos: %s\n",in);
			//fptr = fopen("/home/hicibas-clone/Desktop/test_philippe_rm8/share.txt","w");
			//fprintf(fptr,"%s",in);
			//fclose(fptr);
		do
		{	
			strcpy(out, "X0"); //move the motor
			if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
			{
				printf("Could not send\n");
				return 1;
			}
			
			//sleep(1);
		}while(in[0]!='O'); //motor is still moving. wait and retry;
		
	
	}
	return 1;
	}
	
//Closes the connection to the motor
int closeConnection(void)
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

	char r_buffer[IOBUFFSIZE];
	char w_buffer[IOBUFFSIZE];
	memset(r_buffer,0,IOBUFFSIZE);
	memset(w_buffer,0,IOBUFFSIZE);



	strcpy(w_buffer, "ABORT"); //set low speed
	if(!fnPerformaxComSendRecv(Handle, w_buffer, IOBUFFSIZE,IOBUFFSIZE, r_buffer))
	{
		printf("Could not send\n");
		return 1;
	}
	if (!isOK(r_buffer))
	{
		
		fprintf(stderr,"Unable to set the low speed");
		return 2;
	}
	if(!fnPerformaxComClose(Handle))
	{
		fprintf(stderr, "Error Closing\n");
		return 3;
	}
	
	
}

int closem(void){ 

	char		out[64];
	char		in[64];
	
	strcpy(out, "STOP"); //disable device
	if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
	{
		printf("Could not send\n");
		return 1;
	}
	strcpy(out, "EO=0"); //disable device
	if(!fnPerformaxComSendRecv(Handle, out, 64,64, in))
	{
		printf("Could not send\n");
		return 1;
	}
	if(!fnPerformaxComClose(Handle))
	{
		printf( "Error Closing\n");
		return 1;
	}
	return 1;
	//printf("Motor connection has been closed\n");
	}
