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

typedef struct{//if set to -999 we will not try to reach the setpoint
    int ttm_setpoint;
    int nutec_setpoint;
    int ccam_setpoint;
    }heaters;

typedef struct{
    STATE nextState;
    STATE state;
    Path path; 
    py_manager *py;
	shared_tcs *tcs;
	bool lim_online;
	map<std::string,int> *py_config_file;
	std::string scripts_fname;
    heaters heater;
} instHandle;

#endif // INSTHANDLE_H
