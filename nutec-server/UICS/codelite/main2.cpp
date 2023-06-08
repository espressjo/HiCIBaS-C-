
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include "b64.h"
#include <vector>



using namespace std;



int main(int argc, char **argv)
{
	
	string test="hello-world";
	test.clear();
	cout<<"test: "<<endl;
	
	/*
	struct sockaddr_in client;
	struct sockaddr_in *test;
		int sockfd;
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
		
	memset(&client, 0, sizeof(client));
		
	// Filling server information
	client.sin_family = AF_INET; // IPv4
	client.sin_addr.s_addr = INADDR_ANY;
	client.sin_port = htons(5555);
	cout<<"sizeof: "<<sizeof(client)<<endl;
	cout<<client.sin_port<<endl;
	char *encodedStructure=NULL;
	size_t outlen = islb64EncodeAlloc((const char *)&client,sizeof(client),&encodedStructure);
	std::string enc = std::string(encodedStructure);
	cout<<enc<<endl;
	cout<<encodedStructure<<endl;
	
	//int islb64DecodeAlloc(const char *in, size_t inlen,char **out, size_t * outlen);
	size_t outlen2=0;
	islb64DecodeAlloc(enc.c_str(), sizeof(enc.c_str()),(char **)&test, &outlen2);
	cout<<"sizeof: "<<sizeof(*test)<<endl;
	cout<<test->sin_port<<endl;
	 */
	return 0;
}

