#include "socket_.h"
udp_socket_::udp_socket_(std::string addr,uint16_t port,int rcv_timeout,int snd_timeout){

    status=0;
    struct hostent *hp;
    // Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
        status=-1;
		exit(EXIT_FAILURE);
	}
    //::::::::::::::::::::::::::
    //::: set socket options :::
    //::::::::::::::::::::::::::
    udp_socket_::rcv_timeout.tv_sec = rcv_timeout;
    udp_socket_::rcv_timeout.tv_usec = 0;
    udp_socket_::snd_timeout.tv_sec = snd_timeout;
    udp_socket_::snd_timeout.tv_usec = 0;
    if (setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, &rcv_timeout,sizeof rcv_timeout) < 0)
    {
        perror("setsockopt failed\n");
        status = -1;
    }
    if (setsockopt (sockfd, SOL_SOCKET, SO_SNDTIMEO, &rcv_timeout,sizeof rcv_timeout) < 0)
    {
        perror("setsockopt failed\n");
        status = -1;
    }
    
	memset(&servaddr, 0, sizeof(servaddr));
		
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
    
    hp = gethostbyname(addr.c_str());
    if(hp==nullptr)//modification 2018-01-13
    {
        perror("gethostbyname failed");

        close(sockfd);
        status = -1;
    }
    if(hp ==0)
    {
        perror("gethostbyname failed");
        close(sockfd);
        status = -1;
    }
    memcpy(&servaddr.sin_addr, hp->h_addr, static_cast<size_t>(hp->h_length));
    
}
udp_socket_::udp_socket_(std::string addr,uint16_t port){
    /*
     * Same as above but with default time out of 12 sec for recv and snd
     */ 
    status=0;
    struct hostent *hp;
    // Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
        status=-1;
		exit(EXIT_FAILURE);
	}
    //::::::::::::::::::::::::::
    //::: set socket options :::
    //::::::::::::::::::::::::::
    udp_socket_::rcv_timeout.tv_sec = 12;
    udp_socket_::rcv_timeout.tv_usec = 0;
    udp_socket_::snd_timeout.tv_sec = 12;
    udp_socket_::snd_timeout.tv_usec = 0;
    if (setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, &rcv_timeout,sizeof rcv_timeout) < 0)
    {
        perror("setsockopt failed\n");
        status = -1;
    }
    if (setsockopt (sockfd, SOL_SOCKET, SO_SNDTIMEO, &rcv_timeout,sizeof rcv_timeout) < 0)
    {
        perror("setsockopt failed\n");
        status = -1;
    }
    
	memset(&servaddr, 0, sizeof(servaddr));
		
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
    
    hp = gethostbyname(addr.c_str());
    if(hp==nullptr)//modification 2018-01-13
    {
        perror("gethostbyname failed");

        close(sockfd);
        status = -1;
    }
    if(hp ==0)
    {
        perror("gethostbyname failed");
        close(sockfd);
        status = -1;
    }
    memcpy(&servaddr.sin_addr, hp->h_addr, static_cast<size_t>(hp->h_length));
    
    }
udp_socket_::~udp_socket_(){
    close(sockfd);
}
int udp_socket_::snd_msg(std::string msg)
/*
 * Definition
 * ----------
 * 	Send a message to the server using UDP socket.
 * 
 * Return 
 * ------
 * 	On success this function return the number of byte sent.
 * 	on failure it return -1	
 */ 
{
    if (status!=0){return -1;}
    int n;
    socklen_t len;
    if (strlen(msg.c_str())>=MAXBUFFER)
    {
            msg = msg.substr(0,MAXBUFFER-1);
    }
	int ret = sendto(sockfd, (const char *)msg.c_str(), strlen(msg.c_str()),
		0, (const struct sockaddr *) &servaddr,
			sizeof(servaddr));
	
	return ret;
    }
    
int udp_socket_rcv_msg(std::string *msg){
	
return 0;
}
socket_::socket_(std::string addr,uint16_t port)
{
    socket_::time_out_sec = 2;
	socket_::time_out_msec = 0;
    status = connectSocket(addr,port);

}
socket_::socket_(std::string addr,uint16_t port,int timeout)
/*
 * timeout in second
 */ 
{
	if (timeout<1000)
	{
		socket_::time_out_sec = 0;
		socket_::time_out_msec = timeout;
	}
	else{
		socket_::time_out_sec = (int)(timeout/1000.0);
		socket_::time_out_msec = static_cast<int>(  (timeout/1000.0-(int)(timeout/1000.0)) *1000.0    );
        static_cast<int>((static_cast<float>(timeout)-static_cast<float>(socket_::time_out_sec))*1000.0);
        
    }
	
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
    timeout.tv_sec = socket_::time_out_sec;
    timeout.tv_usec = socket_::time_out_msec*1000;
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
	
	//make the socket non-blocking
	fcntl(sock, F_SETFL, O_NONBLOCK);
	fd_set fdset;
	long flags;
	/*
    if((connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) )
    {
        perror("connect failed");
        socket_::closeSocket();
        return -1;
    }
	*/ 
	connect(sock, (struct sockaddr *) &server, sizeof(server));
	FD_ZERO(&fdset);
    FD_SET(sock, &fdset);
	if (select(sock + 1, NULL, &fdset, NULL, &timeout)  ==1)
	{
		int so_error;
        socklen_t len = sizeof so_error;

        getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
        if (so_error == 0) {
			flags = fcntl(sock, F_GETFL, NULL);
			flags &= (~O_NONBLOCK);
			fcntl(sock, F_SETFL, flags);
		}	
		else 
		{
			perror("connect failed");
			socket_::closeSocket();
			return -1;
		}
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