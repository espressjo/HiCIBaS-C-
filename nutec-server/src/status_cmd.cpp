#include "status_cmd.h"


void get_status_code(instHandle *handle,cmd *cc)
{
    uint32_t v0xa0=0,v0xc9=0,v0xa4=0;
    string check="";
    string message="";
    string sep=";";
    if (readRegister_32(handle,"0xa0",&v0xa0,true)!=0)
    {
        printf("Unable to read r0xa0\n");
    }
    if (readRegister(handle,"0xa0",&check,true)!=0)
    {
        printf("Unable to read r0xa0 (string)\n");
    }
    cout<<"[0xa0] : "<<v0xa0<<" - "<<check<<endl;
    
    if (readRegister_32(handle,"0xc9",&v0xc9,true)!=0)
    {
        printf("Unable to read 0xc9\n");
    }
    if (readRegister(handle,"0xc9",&check,true)!=0)
    {
        printf("Unable to read 0xc9 (string)\n");
    }
    cout<<"[0xc9] : "<<v0xc9<<" - "<<check<<endl;
    
    if (readRegister_32(handle,"0xa4",&v0xa4,true)!=0)
    {
        printf("Unable to read 0xa4\n");
    }
    if (readRegister(handle,"0xa4",&check,true)!=0)
    {
        printf("Unable to read 0xa4 (string)\n");
    }
    cout<<"[0xa4] : "<<v0xa4<<" - "<<check<<endl;
    
    for (int i=0;i<=31;i++)
    {
        if ( (static_cast<uint32_t>(pow(2,i)) & v0xa4) == static_cast<uint32_t>(pow(2,i)) )
        {
            message+=R0XAO(i)+sep;
        }
    }
    
    for (int i=0;i<=15;i++)
    {
        if ( (static_cast<uint32_t>(pow(2,i)) & v0xc9) == static_cast<uint32_t>(pow(2,i))) 
        {
            message+=R0XC9(i)+sep;
        }
    }
    for (int i=0;i<=18;i++)
    {
        if ( (static_cast<uint32_t>(pow(2,i)) & v0xa4) == static_cast<uint32_t>(pow(2,i)) )
        {
            message+=R0XA4(i)+sep;
        }
    }
    
    
    
    sndMsg(cc->sockfd,message.substr(0,message.length()-1));
    return ;
    
}