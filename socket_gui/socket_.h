#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h> // for close
#include <arpa/inet.h>

#define MAXBUFFER 4096

class udp_socket_
{
public:
    udp_socket_(std::string addr,uint16_t port,int rcv_timeout,int snd_timeout);
    udp_socket_(std::string addr,uint16_t port);
    ~udp_socket_();
    int snd_msg(std::string msg);
    int rcv_msg(std::string *msg);
    int status;
private:
    struct timeval rcv_timeout;
    struct timeval snd_timeout;
    struct sockaddr_in	 servaddr;
    int sockfd;
};
class socket_
{
public:
    socket_(std::string addr,uint16_t port);
    socket_(std::string addr,uint16_t port,int timeout);
    ~socket_();
    std::string readSocket();//read until \n character is found
    std::string readSocket(size_t *size);
    int readWelcomeMessage();
    int connectSocket(std::string addr, uint16_t port);
    int closeSocket();
    int writeSocket(std::string msg);
    int status;
private:
    int time_out;
    int sock;
};

#endif // SOCKET_H