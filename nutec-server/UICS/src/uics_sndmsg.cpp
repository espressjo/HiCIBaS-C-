#include "uics_sndmsg.h"

//send(fd,msg.c_str(),strlen(msg.c_str()),0);
std::string strip(std::string txt)
{
    if (txt.size()==0)
    {
        return txt;
    }
    if (txt[txt.size()-1]=='\n'){
        std::string newString = txt;
        newString.replace(newString.size()-1,1,"");
        return newString;
    }
    return txt;
}
void sndMsg(int fd,std::string msg)
{
    std::string m="";
    if (msg.compare("")==0)
    {   m = "OK\n";
        send(fd,m.c_str(),strlen(m.c_str()),0);

    }
    else {
        m = "OK "+msg+"\n";
        send(fd,m.c_str(),strlen(m.c_str()),0);
    }
    log_msg(strip(m)+" -> "+std::to_string(fd));
    return ;
}
void sndMsg(int fd)
{
       std::string m = "OK\n";
       send(fd,m.c_str(),strlen(m.c_str()),0);
        log_msg(strip(m)+" -> "+std::to_string(fd));
    return ;
}
void sndMsg(int fd,std::string msg,int errCode)
{
    std::string m = "NOK "+std::to_string(errCode)+" "+msg+"\n";
    send(fd,m.c_str(),strlen(m.c_str()),0);
    log_msg(strip(m)+" -> "+std::to_string(fd)+ " with error: "+std::to_string(errCode));
    return ;
}
void sndMsg(int fd,const char msg[])
{
    std::string m = "OK "+std::string(msg)+"\n";
    send(fd,m.c_str(),strlen(m.c_str()),0);
    log_msg(strip(m)+" -> "+std::to_string(fd));
    return ;
}
void sndMsg(int fd,const char msg[],int errCode)
{
    std::string m= "NOK "+std::to_string(errCode)+" "+std::string(msg)+"\n";
    send(fd,m.c_str(),strlen(m.c_str()),0);
    log_msg(strip(m)+" -> "+std::to_string(fd)+ " with error: "+std::to_string(errCode));
    return ;
}

//------------------------//
//      UDP Datagram      //
//------------------------//
void sndMsg(int fd,std::string msg,struct sockaddr_in *dg)
{
    std::string m="";
	
    if (msg.compare("")==0)
    {   m = "OK\n";
		sendto(fd,m.c_str(),strlen(m.c_str()),MSG_CONFIRM,(const struct sockaddr *)dg,(socklen_t)sizeof(*dg));
    }
    else {
        m = "OK "+msg+"\n";
        sendto(fd,m.c_str(),strlen(m.c_str()),MSG_CONFIRM,(const struct sockaddr *)dg,(socklen_t)sizeof(*dg));
    }
    log_msg(strip(m)+" -> "+std::to_string(fd));
    return ;
}
void sndMsg(int fd,struct sockaddr_in *dg)
{
	//std::cout<<"Inside sndmsg(): "<<dg->sin_port<<std::endl;
       std::string m = "OK\n";
       sendto(fd,m.c_str(),strlen(m.c_str()),MSG_CONFIRM,(const struct sockaddr *)dg,(socklen_t)sizeof(*dg));
	log_msg(strip(m)+" -> "+std::to_string(fd));
    return ;
}
void sndMsg(int fd,std::string msg,int errCode,struct sockaddr_in *dg)
{
    std::string m = "NOK "+std::to_string(errCode)+" "+msg+"\n";
    sendto(fd,m.c_str(),strlen(m.c_str()),MSG_CONFIRM,(const struct sockaddr *)dg,(socklen_t)sizeof(*dg));
    log_msg(strip(m)+" -> "+std::to_string(fd)+ " with error: "+std::to_string(errCode));
    return ;
}
void sndMsg(int fd,const char msg[],struct sockaddr_in *dg)
{
    std::string m = "OK "+std::string(msg)+"\n";
    sendto(fd,m.c_str(),strlen(m.c_str()),MSG_CONFIRM,(const struct sockaddr *)dg,(socklen_t)sizeof(*dg));
    log_msg(strip(m)+" -> "+std::to_string(fd));
    return ;
}
void sndMsg(int fd,const char msg[],int errCode,struct sockaddr_in *dg)
{
    std::string m= "NOK "+std::to_string(errCode)+" "+std::string(msg)+"\n";
    sendto(fd,m.c_str(),strlen(m.c_str()),MSG_CONFIRM,(const struct sockaddr *)dg,(socklen_t)sizeof(*dg));
    log_msg(strip(m)+" -> "+std::to_string(fd)+ " with error: "+std::to_string(errCode));
    return ;
}
