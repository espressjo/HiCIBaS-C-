#include "udp_client_socket.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
	string r="",r2="";
	int ret = 0;
	udp_client server("localhost",6555,1000);	
	cout<<"server: "<<server.send_rcv(std::string("testThread"),&r)<<endl;
	if (server.isOk()){
	cout<<"receive: "<<r<<endl;}
	else
	{cout<<"Problem"<<endl;}
	/*
	udp_client server2("localhost",6555,-1);

	server.send(std::string("test2 echo hello-world"));
	server2.send(std::string("test echo hello-world"));
	cout<<"Sending: test2"<<endl;
	
	
	cout<<"Sending: test"<<endl;
	ret = server2.recv_strip(&r2);
	if (server2.isOk()){
		cout<<"[2] recv: "<<r2<<endl;
	}
	else
	{
		cout<<"[2] NOK"<<endl;
	}
	
	
	ret = server.recv_strip(&r);
	if (server.isOk()){
		cout<<"[1] recv: "<<r<<endl;
	}
	else
	{
		cout<<"[1] NOK"<<endl;
	}
	cout<<""<<endl;
	
	
*/	

return 0;
}
