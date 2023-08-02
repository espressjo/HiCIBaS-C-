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
    else 
    {
        for (int i=0;i<=31;i++)
            {
                if ( (static_cast<uint32_t>(pow(2,i)) & v0xa0) == static_cast<uint32_t>(pow(2,i)) )
                {
                    message+=R0XAO(i)+sep;
                }
            }
        
    }

    
    if (readRegister_32(handle,"0xc9",&v0xc9,true)!=0)
    {
        printf("Unable to read 0xc9\n");

    }
    else 
    {
        for (int i=0;i<=15;i++)
        {
            if ( (static_cast<uint32_t>(pow(2,i)) & v0xc9) == static_cast<uint32_t>(pow(2,i))) 
            {
                message+=R0XC9(i)+sep;
            }
        }
        
    }
    
    if (readRegister_32(handle,"0xa4",&v0xa4,true)!=0)
    {
        printf("Unable to read 0xa4\n");
        cc->respond("error",uicsCMD_ERR_NOT_SUPPORTED);
        return ;
    }
    else 
    {
        for (int i=0;i<=18;i++)
        {
            if ( (static_cast<uint32_t>(pow(2,i)) & v0xa4) == static_cast<uint32_t>(pow(2,i)) )
            {
                message+=R0XA4(i)+sep;
            }
        }    
        
    }
    cc->respond(message.substr(0,message.length()-1));
    return ;
    
}