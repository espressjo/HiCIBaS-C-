#include "nutec.h"



int setup(instHandle *handle)
{
	
	handle->sport.ecrireport("g r0x32");
	string ret = "";
	int r = handle->sport.lirec(&ret,'\r');
	
	cout<<"[r="<<to_string(r)<<"] value: "<<ret<<endl;
	return 0;
}