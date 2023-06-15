#ifndef INSTHANDLE_H
#define INSTHANDLE_H

#include "states.h"
#include "ArcusPerformaxDriver.h"

typedef struct{

    STATE nextState;
    STATE state;
	AR_HANDLE handle;
	int position;
	bool drive_enabled;
	bool lim_p;
	bool lim_n;
	bool lim_home;
	bool active;
	bool moving;
	uint16_t mst;
	
} instHandle;

#endif