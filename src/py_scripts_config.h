#ifndef PY_SCRIPTS_CONFIG_H
#define PY_SCRIPTS_CONFIG_H

#include <string>
#include <fstream>
#include <regex>
#include <sstream>
#include <map>
#include <cmath>

bool file_exist (std::string name);
std::vector<std::string> split(std::string command,const char sep);
bool isNumber(std::string s);
uint32_t encode_scripts(std::vector<std::string> scripts,std::map<std::string,int> *myMap);
int getConf(std::string fname,std::map<std::string,int> *myMap);
std::vector<std::string> get_scripts(std::string fname);//used to upload the script from config file in HiCIBaS

#endif
