#ifndef LIM_SWITCH_H
#define LIM_SWITCH_H

#include <LabJackM.h>
#include <string>
#include <iostream>
#include <cmath>
using namespace std;
typedef struct {
	bool upper;
	bool lower;
	bool right;
	bool left;
	bool launch;
	bool az_zero;
	bool alt_zero;
	}limit; 
	
class lim_switch
{
public:
	lim_switch(string device,string connection_type,int serial_nb);
	~lim_switch();
	int read_lim_switch();
	limit lim;
	uint8_t compress();
	void print_status();
	bool connected;
private:
	int handle;
	
};
#endif

/*
serial: 440010529
Model: T4
Connection: Ethernet/USB/WiFi
*/