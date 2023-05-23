#ifndef SHARED_TELEMETRY_H
#define SHARED_TELEMETRY_H

#include <string>
#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#include "telemetry.h"

#define BUF_SIZE 1024

typedef struct{
	uint8_t limswitch;//<0> upper, <1> lower , <2> right, <3> left, <4> launch, <5> AZ-0, <6> ALT-0
	bool cam1;
	bool cam2;
	bool cam3;
	bool nutec_active;
	bool nutec_enable;
	bool rm8_enable;
	bool rm8_active;
	bool lim_activte;
	bool TTM;
	bool source_calibration;
	bool alt_moving;
	bool az_moving;
	float alt;//moteur altitude in degree
	float az;//moteru azimuth in degree
	uint16_t moteur_1;//encoder moteur #1
	uint16_t moteur_2;//encoder moteur #2
	int T1;//Temperature #1
	int T2;//Temperature #2
	int T3;//Temperature #3
	int T4;//Temperature #4
	int T5;//Temperature #5
	int T6;//Temperature #6
	int H1;//heater #1
	int H2;//heater #2
	int H3;//heater #3
	int H4;//heater #4
	int H5;//heater #5
	int H6;//heater #6
	float RA;//RA of the last astrometry measurement
	float DEC;//DEC of the last astrometry measurment
	uint32_t r_scripts;//Running Script.
	uint32_t s_scripts;//Running Script.
	
	bool connected;
} shm_telemetry;

class shared_telemetry
{
public:
    shared_telemetry(uint shm_key);
    ~shared_telemetry();
    shm_telemetry *shmp;
private:
	void clear_shared_memory();
    int get_shared_memory();
    int remove_shared_memory();
    size_t PAGE_SIZE;
    uint shm_key;
    int shmid;

};

#endif // SHARED_TCS_H