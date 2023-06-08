#ifndef NUTEC_CONFIG_H
#define NUTEC_CONFIG_H

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int ui_get_string(string c_file,string entry,string *value);
int ui_get_double(string c_file,string entry,double *value);
int ui_get_int(string c_file,string entry,int *value);
int ui_get_bool(string c_file,string entry,bool *value);

namespace ui_conf {
	string get_var(string line);//return the 1st word of a config line. 
	bool is_double(string str);
	bool is_int(string str);
}
#endif