#include "nutec.h"

void serial_cmd_io(instHandle *handle,cmd *cc)
{
	string mycmd="",answ="";
	if ((*cc)["cmd"].compare("")!=0){
		mycmd = (*cc)["cmd"]+"\r";
		if (handle->sport.ecrireport(mycmd)!=OK)
		{
			sndMsg(cc->sockfd,"Unable to write command",uicsCMD_ERR_VALUE);
			return ;
		}
		if (handle->sport.lirec(&answ,'\r')!=OK)
		{
			sndMsg(cc->sockfd,"Unable to write command",uicsCMD_ERR_VALUE);
			return ;
		}
		sndMsg(cc->sockfd,answ);
		return;
	}
	
}

int setup(instHandle *handle)
{
	
	handle->sport.ecrireport("g r0x32");
	string ret = "";
	int r = handle->sport.lirec(&ret,'\r');
	
	cout<<"[r="<<to_string(r)<<"] value: "<<ret<<endl;
	return 0;
}