#ifndef INSTHANDLE_H
#define INSTHANDLE_H

#include "states.h"
#include "serial.h"

typedef struct{

    STATE nextState;
    STATE state;
	serial sport;

} instHandle;

#endif