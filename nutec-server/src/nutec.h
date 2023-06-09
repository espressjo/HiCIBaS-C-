#ifndef NUTEC_H
#define NUTEC_H

#include "insthandle.h"
#include "uics.h"
#include <string>
#include <iostream>

using namespace std;

int setup(instHandle *handle);

void serial_cmd_io(instHandle *handle,cmd *cc);
int setRegister (instHandle *handle,string reg,int value,bool RAM=true);
int readRegister(instHandle *handle,string reg,int *value,bool RAM=true);
int setRegister (instHandle *handle,string reg,string value,bool RAM=true);
int readRegister(instHandle *handle,string reg,string *value,bool RAM=true);
int toInt(string s,int *value);
int trigger_move(instHandle *handle);
int trigger_abort(instHandle *handle);
void read_position(instHandle *handle,cmd *cc);
void move_abs(instHandle *handle,cmd *cc);
void abort(instHandle *handle,cmd *cc);
void move(instHandle *handle,cmd *cc);
void set_speed(instHandle *handle,cmd *cc);
void get_speed(instHandle *handle,cmd *cc);
void isMoving(instHandle *handle,cmd *cc);
#endif



