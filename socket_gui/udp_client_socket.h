#ifndef UDP_CLIENT_SOCKET_H
#define UDP_CLIENT_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

#define MAXLINE 1024

class udp_client
{
private:
	std::string IP;
	uint16_t port;
	int timeout;//recvfrom timeout in second
	int sockfd;
	struct sockaddr_in servaddr;
	void strip(std::string *str,const char term_c);
	bool ok;
	void strip_header(std::string *str);
public:
	udp_client(std::string IP,int port,int timeout);//recvfrom timeout in second. Timeout can be set to -1 for no timeout
	~udp_client();//close the sockfd
	int send(std::string command);//send a command
	int recv(std::string *reply);//recv a reply from a command. the \n will be striped
	int recv_strip(std::string *reply);//recv a reply from a command. the \n will be striped as well as the OK/NOK
	int send_rcv(std::string command,std::string *reply);//Utility function to send and recv a command in the same call.
	int send_rcv_strip(std::string command,std::string *reply);
	bool isOk();//set to true if OK is received during recv_strip or recv call.
};


#endif