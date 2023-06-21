#ifndef RM8_H
#define RM8_H


#include <stdio.h>
#include <string.h>
#include <string>
#include "ArcusPerformaxDriver.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "insthandle.h"
#include "uics.h"
#include <thread>
#include <mutex>

#define IOBUFFSIZE 64

using namespace std;


bool writeCommand(instHandle *handle,string cmd);
bool readCommand(instHandle *handle,string cmd,string *recv);
bool readCommand_int(instHandle *handle,string cmd,int *recv);
int connect(instHandle *handle);
void status_t(instHandle *handle);
bool isInt(string INT);
void delay(int ms);


//::::::::::::::::::::::::::
//:::   Server command   :::
//::::::::::::::::::::::::::
void abort(instHandle *handle,cmd *cc);
void enable_drive(instHandle *handle,cmd *cc);
void disable_drive(instHandle *handle,cmd *cc);
void read_position(instHandle *handle,cmd *cc);
void motor_status(instHandle *handle,cmd *cc);
void clear_limit(instHandle *handle,cmd *cc);
void motor_status(instHandle *handle,cmd *cc);
void loop(instHandle *handle,cmd *cc);
void loop_no_return(instHandle *handle,cmd *cc);
void serialio(instHandle *handle,cmd *cc);
void set_low_speed(instHandle *handle,cmd *cc);
void set_high_speed(instHandle *handle,cmd *cc);
void set_acceleration(instHandle *handle,cmd *cc);
void get_low_speed(instHandle *handle,cmd *cc);
void get_high_speed(instHandle *handle,cmd *cc);
void get_acceleration(instHandle *handle,cmd *cc);
void isMoving(instHandle *handle,cmd *cc);
void setPosition(instHandle *handle,cmd *cc);
void closeConnection(instHandle *handle,cmd *cc);
void p_status(instHandle *handle,cmd *cc);
void usb(instHandle *handle,cmd *cc);
#endif

