#ifndef INSTHANDLE_H
#define INSTHANDLE_H

#include "states.h"
#include "serial.h"

typedef struct{

    STATE nextState;
    STATE state;
	serial sport;
	int serial_port;
	int baudrate;
	int position;
	uint32_t xa0;
	bool enabled;
	bool active;//the serial communication is established, position is updated.
	bool moving;
	bool lim_p;
	bool lim_n;
	bool home;
	bool phase_error;
    bool deduce_moving;
} instHandle;

#endif