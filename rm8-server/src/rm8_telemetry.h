#ifndef RM8_TELEMETRY_H
#define RM8_TELEMETRY_H

typedef struct{

	int position;
	bool drive_enabled;
	bool lim_p;
	bool lim_n;
	bool lim_home;
	bool active;
	bool moving;
	
} rm8_telemetry;

#endif