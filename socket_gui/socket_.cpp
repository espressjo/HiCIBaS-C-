#include "socket_.h"

socket_::socket_(std::string addr,uint16_t port)
{
    socket_::time_out = 2;
    status = connectSocket(addr,port);

}
socket_::socket_(std::string addr,uint16_t port,int timeout)
{
    socket_::time_out = timeout;
    status = connectSocket(addr,port);

}
int socket_::connectSocket(std::string addr,uint16_t port)
{
    struct sockaddr_in server;
    struct hostent *hp;

    sock = socket(AF_INET, SOCK_STREAM,0);
    if(sock < 0)
    {
       perror("socket failed");
       return -1;
    }

    struct timeval timeout;
    timeout.tv_sec = socket_::time_out;
    timeout.tv_usec = 0;

    if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout,sizeof timeout) < 0)
    {
        perror("setsockopt failed\n");
        return -1;
    }




    server.sin_family = AF_INET;
    hp = gethostbyname(addr.c_str());
    if(hp==nullptr)//modification 2018-01-13
    {
        perror("gethostbyname failed");

        socket_::closeSocket();
        return -1;
    }
    if(hp ==0)
    {
        perror("gethostbyname failed");
        socket_::closeSocket();
        return -1;
    }
    memcpy(&server.sin_addr, hp->h_addr, static_cast<size_t>(hp->h_length));
    server.sin_port = htons(port);
    if((connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) )
    {
        perror("connect failed");
        socket_::closeSocket();
        return -1;
    }
    return 0;
}
int socket_::writeSocket(std::string msg)
{
    if (msg[msg.length()-1]!='\n')
    {
        msg+='\n';
    }
    return send(sock, msg.c_str(),strlen(msg.c_str()),0);

}
int socket_::readWelcomeMessage()
/*
 * Read the welcome message of the server.
 */ 
{
    char buff[2];
    memset(buff,0,2);
    std::string b="";
    int rval = recv(sock, buff, 1,0);
    if (rval<0){
        //probably a timeout
        return -1;
        }
    b+=std::string(buff);
    while (buff[0]!='\n' || rval<=0)
    {

        memset(buff,0,2);
        rval = recv(sock, buff, 1,0);
        if (rval<0){return -1;}
        b+=std::string(buff);
    }
    
    return 0;

}
std::string socket_::readSocket()
{
    char buff[2];
    memset(buff,0,2);
    std::string b="";
    int rval = recv(sock, buff, 1,0);
    b+=std::string(buff);
    while (buff[0]!='\n' || rval<=0)
    {

        memset(buff,0,2);
        int rval = recv(sock, buff, 1,0);
        b+=std::string(buff);
    }

    return b;

}

std::string socket_::readSocket(size_t *size)
{
    char buff[2];
    *size+=0;
    memset(buff,0,2);
    std::string b="";
    int rval = recv(sock, buff, 1,0);
    *size+=rval;
    b+=std::string(buff);
    while (buff[0]!='\n' || rval<=0)
    {

        memset(buff,0,2);
        int rval = recv(sock, buff, 1,0);
        *size+=rval;
        b+=std::string(buff);
    }

    return b;
}
socket_::~socket_()
{//if sock is not close, close-it
    if (sock>0){
    close(sock);}
    
}
int socket_::closeSocket()
{
    close(sock);
    sock = -1;//set the fd to known bad value
    return 0;
}