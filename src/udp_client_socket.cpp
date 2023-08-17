#include "udp_client_socket.h"
udp_client::udp_client(std::string IP,int port,int timeout)
/*
 * Itnitialize the UDP datagram for <IP> address and <port>.
 * A timeout can be set on the recvfrom call (in milliseconds). If
 * you don't want a timeout, set timeout to -1.
 *  
 */ 
{
	struct hostent *hp;
	connected=false;
	udp_client::IP = IP;
	udp_client::port = static_cast<uint16_t>(port);
	udp_client::timeout = timeout;
	
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
        //connected = false;
        return ;
	}
	memset(&servaddr, 0, sizeof(servaddr));
	
	// convert ms to second and microseconds
	float t_ms = timeout/1000.0; 
	int t_s,t_us ;
	t_s = (long)t_ms;
	t_ms = t_ms - (long)t_ms;
	t_ms = t_ms*1000000;
	t_us = (long)t_ms;
	if (timeout!=-1){
		struct timeval tv;
		tv.tv_sec = t_s;
		tv.tv_usec = t_us;
		if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
				perror("Error");
		}
	
	}
    
	hp = gethostbyname(IP.c_str());
    
    if(hp==nullptr)//modification 2018-01-13
    {
        
        perror("gethostbyname failed");
        close(sockfd);
        //exit(EXIT_FAILURE);
        return ;
    }
    if(hp ==0)
    {
        perror("gethostbyname failed");
        //exit(EXIT_FAILURE);
        return ;
    }
    memcpy(&servaddr.sin_addr, hp->h_addr, static_cast<size_t>(hp->h_length));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(udp_client::port);
	connected = true;
	
}
udp_client::~udp_client()
/*
 * Make sure the socket filedescriptor is closed
 */ 
{
	close(sockfd);
}
int udp_client::send(std::string command)
/*
 * Definition
 * ----------
 * send a command to the UICS server.
 * 
 * Return 
 * ------
 *  0  if succesfull, 
 * -1  if failed.
 */ 
{
	
	int n=0;

	n = sendto(sockfd, command.c_str(), strlen(command.c_str()),MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));
	if (n!=command.length())
	{
		return -1;}
	return 0;
}
void udp_client::strip(std::string *str,const char term_c)
/*
 * Description
 * -----------
 * Strip character term_c from c++ string *str.
 */ 
{
	if ((*str)[str->length()-1]==term_c)
	{
		*str = str->substr(0,str->length()-1);
	}
	return ;
}
int udp_client::recv(std::string *reply)
/*
 * Definition
 * ----------
 * Receive a reply from from the server. The received
 * data will be written in *reply.
 * 
 * Return 
 * ------
 *  0 -> successfully recv datagram
 * -1 -> Failed to recv datagram
 * -2 -> Try again, maybe because a timeout happened  
 */ 
{
	
	char buffer[MAXLINE];
	ok=false;
	unsigned int len;
	
	reply->clear();
	memset(buffer,0,MAXLINE);
	
	ssize_t n = recvfrom(sockfd, (char *)buffer, MAXLINE,
				MSG_WAITALL, (struct sockaddr *) &servaddr,
				&len);
	std::cout<<"n="<<n<<std::endl;
	std::cout<<"errno: "<<errno<<std::endl;
	
	if (n<0 && errno==11){
		ok = false;
		return -2;		
	}
	else if (n<0){
		ok = false;
		return -1;
	}

	*reply = std::string(buffer);
	strip(reply,'\n');
	
	if (reply->length()<2){
		//Don't know whats going if this is the case
		ok=false;
		return -1;
	}
	if (reply->substr(0,2).compare("OK")!=0)
	{
		ok=false;		
	}
	else{
		ok=true;
	}
	return 0;
}
bool udp_client::isConnected()
{
    return connected;
}
bool udp_client::isOk()
/*
 * Description
 * -----------
 * After one of the recceive function is called, 
 * one can use this function to verify if OK or 
 * NOK was present in the received string.
 * 
 * NOTE:
 * -----
 * Functions are; (1) recv, (2) recv_strip, 
 * (3) send_rcv, and (4) send_rcv_strip.
 */ 
{
	return ok;
}
int udp_client::recv_strip(std::string *reply)
/*
 * Definition
 * ----------
 * Receive a reply from from the server. If we receive
 * OK/NOK it will be striped from the reply pointer.
 * Return 
 * ------
 *  0 -> successfully recv datagram
 * -1 -> Failed to recv datagram
 * -2 -> Try again, maybe because a timeout happened  
 */
{
	reply->clear();
	int ret=udp_client::recv(reply);
	
	if (ret==0)
	{
		strip_header(reply);
	}
	return ret;	
}
void udp_client::strip_header(std::string *str)
/*
 * Desciption
 * ----------
 * Will strip the OK or NOK at the begining of
 * the string *str. 
 */ 
{
	//don't know what we received!
	if (str->length()<2) 
	{return;}
	if (str->substr(0,2).compare("OK")==0)
	{
		if (str->length()<3){*str="";}
		else {
			*str = str->substr(3,str->length());
		}
		return;
	}
	if (str->length()<3){return ;}//don't know whats going one here!
	if (str->substr(0,3).compare("NOK")==0){
		*str = str->substr(4,str->length());
	}
	return;
}
int udp_client::send_rcv(std::string command,std::string *reply)
/*
 * Definition
 * ----------
 * Send and receive a reply from from the server. 
 * 
 * Return 
 * ------
 *  0 -> successfully recv datagram
 * -1 -> Failed to recv datagram
 * -2 -> Try again, maybe because a timeout happened  
 * -3 -> failed to send the command to teh server
 */
 {
	if (udp_client::send(command)!=0){return -3;}
	int ret = udp_client::recv(reply);
	return ret;
}
int udp_client::send_rcv_strip(std::string command,std::string *reply)
/*
 * Definition
 * ----------
 * Same as send_rcv, but the *reply will be striped from
 * OK/NOK header.
 * 
 * Return 
 * ------
 *  0 -> successfully recv datagram
 * -1 -> Failed to recv datagram
 * -2 -> Try again, maybe because a timeout happened  
 * -3 -> failed to send the command to teh server
 */
 {
	if (udp_client::send(command)!=0){return -3;}
	int ret = udp_client::recv_strip(reply);
	return ret;
}
