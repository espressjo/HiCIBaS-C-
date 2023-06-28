#ifndef UTILITY_SOCKET_H
#define UTILITY_SOCKET_H

#include <string>
#include "socket_.h"
#include "udp_client_socket.h"

#define OK 1
#define NOK -1
#define CONNECTION_P -2

int snd_cmd(std::string cmd,std::string *value_returned,int port,int udp_port,std::string host,bool tcpip=true,int timeout=2);



#endif

