#ifndef CMDS_H
#define CMDS_H

#include <string>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdarg>
#include <sys/socket.h>
#include "inst_log.h"
#include "states.h"
#include "uics_sockets.h"
#include "uics_ports.h"
#include<stdio.h>
#include<ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "b64.h"
#include "uics_sndmsg.h"

extern Log instLog;
#define PORT 5000

// STATE {ACQ,IDLE,INIT,CONFIG};
enum TYPE {INT=0, FLOAT, DOUBLE, STRING,UNKNOWN_TYPE};

inline void log_ll_cmd(std::string txt){instLog.writeto("[cmd] "+txt,2);
}//cmd log function level=2

//============================//
//          Hash Table        //
//============================//
class hashT{
	/*
	 * Simple implementation of a hash table.
	 * We can use the constructor to "split" 
	 * a string into the hash table using 2 
	 * different separator.
	 * 
	 */ 
	private:
		std::vector<std::string> key;
		std::vector<std::string> value;
		std::vector<std::string> split(std::string str,const char sep);
	public:
		hashT();
		hashT(std::string str,const char key_sep,const char value_sep);
		~hashT();
		void insert(std::string key,std::string value);
		std::string operator[](std::string key);
};

class cmd;

//============================//
//          CMD LIST          //
//============================//

class cmdList
{
private:
    std::vector<cmd> cmd_list;

    bool isSeparator(std::string line);
    bool isEmpty(std::string line);
    int populateList(std::vector<std::vector<std::string>> cmdVec);
    int readConfig(std::string configPath);
    enum cmdReadStatus
    {
        READ_SOCK,
        READ_START,
        READ_CMD,
        READ_STATE,
        READ_ARG_START,
        READ_ARG,
        READ_TYPE,
        READ_OPT,
        READ_RANGE,
        READ_VAL,
    };

public:
    cmdList(std::string configPath = "");
    void printAllCMD();
    std::string getCMDlist();
    std::string getcmdinfo(std::string cmdname);
    void printCMD(std::string name);
    cmd operator[] (std::string str);
};

//============================//
//            CMD             //
//============================//


class cmd
{
private:
    std::string serializeCMD();//create a string with the cmd parameters
    void deserializeCMD(std::string command); //populate command fomr serialized string
    int resetCmd();//clear all the paramters/arrays of cmd
    bool verifyType(std::string &str, TYPE t);
    std::vector<std::string> split(std::string line);//old function should replace with split str,sep)
    void check(std::vector<std::string> *t);
	STATE to_state(std::string state);
	TYPE to_type(std::string type);
	struct sockaddr_in *server_addr;
	std::string UDP;
	size_t udp_e_length;
	int getIP(struct sockaddr_in sa,std::string *IP);
public:
    cmd();
	bool udp;
	
    // Send/Recv Methods
    int sendCMD(uint16_t port);
    int recvCMD(int fd);
	void set_datagram(struct sockaddr_in client);
    // for testing
    void printCMD();

    // Socket Variables
    int sockfd;
    uint16_t port;

    // msgHandler Utilities
    int parseCMD(std::string msg);
    std::string operator[](const char* cmdName);

    // Main command variables
    std::string name;
    int nbArg;
    STATE state;
    std::vector<std::string> args;
    std::vector<std::string> argsVal;
    std::vector<TYPE> type;
    std::vector<bool> opt;
    std::vector<std::string> range;
	std::vector<std::string> split(std::string command,const char sep);
	
	void respond();// send OK
	void respond(std::string msg);// send OK <message>
	void respond(std::string msg,int errCode);// send NOK <error id> <error message>
	void respond(const char msg[]);
	void respond(const char msg[],int errCode);
};
#endif // CMDS_H

