#ifndef UICS_SNDMSG_H
#define UICS_SNDMSG_H


#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <string.h>
#include "inst_log.h"

#include <arpa/inet.h>
#include <netinet/in.h>

extern Log instLog;
inline void log_msg(std::string txt){instLog.writeto("[sndMsg] "+txt,2);}
enum msgState {SUCCESS=0,FAILED=1};


void sndMsg(int fd);// send OK
void sndMsg(int fd,std::string msg);// send OK <message>
void sndMsg(int fd,std::string msg,int errCode);// send NOK <error id> <error message>
void sndMsg(int fd,const char msg[]);
void sndMsg(int fd,const char msg[],int errCode);
void sndMsg(int fd,struct sockaddr_in *dg);// send OK
void sndMsg(int fd,std::string msg,struct sockaddr_in *dg);// send OK <message>
void sndMsg(int fd,std::string msg,int errCode,struct sockaddr_in *dg);// send NOK <error id> <error message>
void sndMsg(int fd,const char msg[],struct sockaddr_in *dg);
void sndMsg(int fd,const char msg[],int errCode,struct sockaddr_in *dg);

#endif // UICS_SNDMSG_H

