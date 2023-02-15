#ifndef RM8DRIVER_H
#define RM8DRIVER_H


#include <stdio.h>
#include <string.h>
#include "ArcusPerformaxDriver.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define IOBUFFSIZE 64

AR_HANDLE	Handle;
int closeConnection(void);
int setMParameters(int LSPD,int HSPD,int ACC);
int isInt(char *buffer,int buffer_length);
int read_position(char *position);
int rand(void); 
int status(int *motor_status);
int closem(void);
int old_move(int);
int move_wait(int x,int *new_position);
int move(int x);
int move_resp(int, int, int, int);
int loop(int x);
int old_loop(int x);
int isOK(char *r_buffer);
int get_info(char *dev_serial,char *id,char *name);
int connect(void);
int getAnalogueInput(char *input1,char *input2);
int enable_drive(void);
int disable_drive(void);
int stopall(void);
int setPosition(int position);

#endif