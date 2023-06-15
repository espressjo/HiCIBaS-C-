#ifndef INSTHANDLE_H
#define INSTHANDLE_H
#include "states.h"

typedef struct{
    STATE nextState;
    STATE state;
} instHandle;

#endif // INSTHANDLE_H
