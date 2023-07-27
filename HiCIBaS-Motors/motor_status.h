#ifndef MOTOR_STATUS_H
#define MOTOR_STATUS_H

#include "nutec_telemetry.h"
#include "rm8_telemetry.h"
#include "b64.h"
#include <string>

int decode_nutec(std::string buff,nutec_telemetry *tlm);
int decode_rm8(std::string buff,rm8_telemetry *tlm);

#endif




