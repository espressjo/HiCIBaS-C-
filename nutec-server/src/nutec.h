#ifndef NUTEC_H
#define NUTEC_H

#include "insthandle.h"
#include "uics.h"
#include <string>
#include <iostream>
#include <mutex>
#include <thread>


using namespace std;

void status_t(instHandle *handle);
int setup(instHandle *handle);
int setRegister (instHandle *handle,string reg,int value,bool RAM=true);
int readRegister(instHandle *handle,string reg,int *value,bool RAM=true);
int readRegister_32(instHandle *handle,string reg,uint32_t *value,bool RAM=true);
int setRegister (instHandle *handle,string reg,string value,bool RAM=true);
int readRegister(instHandle *handle,string reg,string *value,bool RAM=true);
int toInt(string s,int *value);
int toInt32(string s,uint32_t *value);
int trigger_move(instHandle *handle);
int trigger_abort(instHandle *handle);
void delay(int ms);


//::::::::::::::::::::
//:::   commands   :::
//::::::::::::::::::::

void serial_cmd(instHandle *handle,cmd *cc);
void serial_cmd_io(instHandle *handle,cmd *cc);
void p_status(instHandle *handle,cmd *cc);
void g_status(instHandle *handle,cmd *cc);
void abort(instHandle *handle,cmd *cc);
void read_position(instHandle *handle,cmd *cc);
void move_abs(instHandle *handle,cmd *cc);
void move(instHandle *handle,cmd *cc);
void move_no_return(instHandle *handle,cmd *cc);
void set_speed(instHandle *handle,cmd *cc);
void get_speed(instHandle *handle,cmd *cc);
void isMoving(instHandle *handle,cmd *cc);
void enable(instHandle *handle,cmd *cc);
void isEnabled(instHandle *handle,cmd *cc);
void disable(instHandle *handle,cmd *cc);

#endif
 

