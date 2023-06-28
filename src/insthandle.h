#ifndef INSTHANDLE_H
#define INSTHANDLE_H

#include "states.h"
#include <string>
#include "py_manager.h"
#include "shared_tcs.h"
#include <map>

typedef struct{
  std::string config;//where all the config file are stored
  std::string log;//log folder
  std::string data;//Default data location
}Path;


typedef struct{
    STATE nextState;
    STATE state;
    Path path; 
    py_manager *py;
	shared_tcs *tcs;
	bool lim_online;
	map<std::string,int> *py_config_file;
	std::string scripts_fname;
} instHandle;

#endif // INSTHANDLE_H
