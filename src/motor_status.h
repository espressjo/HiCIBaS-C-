#ifndef MOTOR_STATUS_H
#define MOTOR_STATUS_H

#include "utility_socket.h"
#include "insthandle.h"
#include <unistd.h>
#include "nutec_telemetry.h"
#include "rm8_telemetry.h"
#include "b64.h"
void delay(int ms);

void motor_status_t(instHandle *handle);

int decode_nutec(std::string recv,nutec_telemetry *tlm);
int decode_rm8(std::string recv,rm8_telemetry *tlm);
#endif

