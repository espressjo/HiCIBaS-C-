#include "telemetry.h"

std::string encode_telemetry(telemetry tlm)
{
	char *encodedStructure=NULL;
	size_t length = islb64EncodeAlloc((const char *)&tlm,sizeof(tlm),&encodedStructure);
	return std::to_string(length)+" "+std::string(encodedStructure);

}