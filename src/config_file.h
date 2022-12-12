#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include <string>
#include "insthandle.h"
#include <fstream>
#include <iostream>
using namespace std;

int get_entry(string c_file,string entry,string *value);
int get_entry(string c_file,string entry,int *value);
int get_entry(string c_file,string entry,double *value);
string get_var(string line);//return the 1st word of a config line. 
bool is_double(string str);
bool is_int(string str);
//Main Function 
void init_handler(string c_file,instHandle *handler);//initialize the HiCIBaS handler
#endif


