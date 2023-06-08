#include "nutec.h"

void serial_cmd_io(instHandle *handle,cmd *cc)
{
	string mycmd="",answ="";
	if ((*cc)["read"].compare("")!=0){
		mycmd+="g ";
		mycmd += (*cc)["read"]+"\r";
		printf("cmd: %s\n",mycmd.c_str());
		if (handle->sport.ecrireport(mycmd)!=OK)
		{
			sndMsg(cc->sockfd,"Unable to write command",uicsCMD_ERR_VALUE);
			return ;
		}
		if (handle->sport.lirec(&answ,'\r')==0)
		{
			sndMsg(cc->sockfd,"Unable to read command",uicsCMD_ERR_VALUE);
			return ;
		}
		sndMsg(cc->sockfd,answ);
		return;
	}
	if ((*cc)["write"].compare("")!=0 && (*cc)[">"].compare("")!=0){
                
		mycmd="s "+(*cc)[">"]+" "+(*cc)["write"]+"\r";
                
                //printf("cmd: %s\n",mycmd.c_str());
		
                if (handle->sport.ecrireport(mycmd)!=OK)
                {
                        sndMsg(cc->sockfd,"Unable to write command",uicsCMD_ERR_VALUE);
                        return ;
                }
                if (handle->sport.lirec(&answ,'\r')==0)
                {
                        sndMsg(cc->sockfd,"Unable to read command",uicsCMD_ERR_VALUE);
                        return ;
                }
                sndMsg(cc->sockfd,answ);
                return;
		}
sndMsg(cc->sockfd,"read the doc",uicsCMD_ERR_VALUE);
	return ;	
}

int setup(instHandle *handle)
{
	
	handle->sport.ecrireport("g r0x32");
	string ret = "";
	int r = handle->sport.lirec(&ret,'\r');
	
	cout<<"[r="<<to_string(r)<<"] value: "<<ret<<endl;
	return 0;
}
