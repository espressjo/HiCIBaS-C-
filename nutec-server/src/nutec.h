#ifndef NUTEC_H
#define NUTEC_H

#include "insthandle.h"
#include "uics.h"
#include <string>
#include <iostream>

using namespace std;

int setup(instHandle *handle);
void serial_cmd_io(instHandle *handle,cmd *cc);


#endif


