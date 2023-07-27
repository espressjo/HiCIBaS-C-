#include "motor_status.h"
int decode_nutec(std::string buff,nutec_telemetry *tlm)
/*
 * Will receice the output of snd_cmd. Meaning 
 * we expect buff to look like: 16 ==sjhey37623d35s 
 */ 
{
    std::string b="";
    nutec_telemetry *_tlm;
    size_t length=0,outlenght=0;
    char *decoded=nullptr;
    int i=0;
    
    for (auto &c:buff)
    {
        i+=1;
        if (c!=' ')
        {
            b+=c;
            
        }
        else {break;}
    }
    //check b is not empty
    if (b.length()==0){return -1;}
    //check b is Int
    
    for (auto &c:b)
    {
        if (!isdigit(c)){return -1;}
    }
    length = static_cast<size_t>(std::atoi(b.c_str()));
    buff = buff.substr(i,buff.length());
    if (islb64DecodeAlloc(buff.c_str(),length,&decoded,&outlenght)!=0)
	{
		return -1;
	}
    _tlm = (nutec_telemetry*)decoded;
    *tlm = *_tlm;
    return 0;
}

int decode_rm8(std::string buff,rm8_telemetry *tlm)
/*
 * Will receice the output of snd_cmd. Meaning 
 * we expect buff to look like: 16 ==sjhey37623d35s 
 */ 
{
    std::string b="";
    rm8_telemetry *_tlm;
    size_t length=0,outlenght=0;
    char *decoded=nullptr;
    int i=0;
    
    for (auto &c:buff)
    {
        i+=1;
        if (c!=' ')
        {
            b+=c;
            
        }
        else {break;}
    }
    //check b is not empty
    if (b.length()==0){return -1;}
    //check b is Int
    
    for (auto &c:b)
    {
        if (!isdigit(c)){return -1;}
    }
    length = static_cast<size_t>(std::atoi(b.c_str()));
    buff = buff.substr(i,buff.length());
    if (islb64DecodeAlloc(buff.c_str(),length,&decoded,&outlenght)!=0)
	{
		return -1;
	}
    _tlm = (rm8_telemetry*)decoded;
     *tlm = *_tlm;
    return 0;
}