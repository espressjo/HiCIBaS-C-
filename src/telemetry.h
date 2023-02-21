#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "b64.h"
#include <string>

typedef struct{
	uint8_t limswitch;//<0> upper, <1> lower , <2> right, <3> left, <4> launch, <5> AZ-0, <6> ALT-0
	uint8_t devices;//Cam1<0>,Cam2<1> Cam3<2>, TTM<3>, Source calibration<4>, M. ALT<5> M. Az<6>
	float alt;//moteur altitude in degree
	float az;//moteru azimuth in degree
	uint16_t moteur_1;//encoder moteur #1
	uint16_t moteur_2;//encoder moteur #2
	int8_t T1;//Temperature #1
	int8_t T2;//Temperature #2
	int8_t T3;//Temperature #3
	int8_t T4;//Temperature #4
	int8_t T5;//Temperature #5
	int8_t T6;//Temperature #6
	uint8_t H1;//heater #1
	uint8_t H2;//heater #2
	uint8_t H3;//heater #3
	uint8_t H4;//heater #4
	uint8_t H5;//heater #5
	uint8_t H6;//heater #6
	float RA;//RA of the last astrometry measurement
	float DEC;//DEC of the last astrometry measurment
	uint32_t scripts;//Running Script.
} telemetry;

std::string encode_telemetry(telemetry tlm);
#endif

