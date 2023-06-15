#ifndef UICS_SOCKETS_H
#define UICS_SOCKETS_H

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h> // for close
#include <arpa/inet.h>
#include "inst_log.h"

extern Log instLog;
inline void log_socket(std::string txt){instLog.writeto("[socket] "+txt,3 );}
inline void Perror(std::string txt){log_socket(txt);perror(txt.c_str());}
#define h_addr h_addr_list[0]//to be posix complient

int create_socket(uint16_t port);

int read_socket(std::string *buffer,int sockfd);
int read_socket(uint16_t port,std::string *buffer);
int read_socket(uint16_t port,std::string *buffer,std::string addr);
int read_image(unsigned int *im,size_t length,int sockfd);

int write_image(uint16_t port, unsigned int *im,size_t length,std::string addr);
int write_socket(uint16_t port, std::string command,std::string addr);
int write_socket(uint16_t port, std::string command);
int write_socket(std::string command,int sock);

/*
#define MAXLINE 1024

class upd_server{
	
private:
	struct sockaddr_in servaddr,cliaddr;
	int sockfd;
	uint16_t port;
	char buffer[MAXLINE];
public:
	upd_server(int port);
	recvfrom(std::);
	~upd_server();
	
};
upd_server::upd_server(int port){
	upd_server::port = port;
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
		
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(upd_server::port);
		
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
}
upd_server::~upd_server(){
	
	close(sockfd);
}

*/
#endif // UICS_SOCKETS_H

