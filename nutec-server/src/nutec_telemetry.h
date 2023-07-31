#ifndef NUTEC_TELEMETRY_H
#define NUTEC_TELEMETRY_H



typedef struct{

	int position;
	bool enabled;
	bool active;//the serial communication is established, position is updated.
	bool moving;
	bool lim_p;
	bool lim_n;
	bool home;
	bool phase_error;
} nutec_telemetry;

#endif