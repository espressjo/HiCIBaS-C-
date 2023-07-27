#ifndef NUTEC_H
#define NUTEC_H

#include "insthandle.h"
#include "uics.h"
#include <string>
#include <iostream>
#include <mutex>
#include <thread>
#include "nutec_telemetry.h"
#include "nutec_config.h"


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

//::::::::::::::::::::::::::::
//:::   conversion utils   :::
//::::::::::::::::::::::::::::

double vel_rpm(int counts);
int vel_counts(double rpm);
int acc_counts(double rps);
double acc_rps(int count);

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
void status_t(instHandle *handle);
void position_status_t(instHandle *handle);
void set_acceleration(instHandle *handle,cmd *cc);
void get_acceleration(instHandle *handle,cmd *cc);
void deduce_moving(instHandle *handle,cmd *cc);
#endif
 

