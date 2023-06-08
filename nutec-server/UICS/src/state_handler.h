#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include "uics_sockets.h"
#include "states.h"
#include "insthandle.h"
#include <sys/types.h>
#include <sys/socket.h>
#include "uics_cmds.h"
#include "uics_state.h"
#include <thread>
#include <unistd.h>
#include "uics_config.h"
#include "uics_sndmsg.h"
#include "uics_join.h"
#include "uics_err_code.h"
extern Log instLog;
inline void log_stateH(std::string txt){instLog.writeto("[state H] "+txt,2);}
class state_handler
{
public:
    state_handler(instHandle *handle);
    void run();
    // add states here
    uics_state *s_config;
    uics_state *s_idle;
    uics_state *s_init;
    uics_state *s_acq;
private:
    STATE defaultState;
    STATE nextState;

   
    void handler();
    instHandle *myHandle;
    int sockfd,commfd;
    std::string msg;

    // Utility Functions
    int checkState(cmd *cc,bool requestChange);
    std::vector<std::vector<STATE>> state_chart;//states chart for transition
    std::vector<STATE> parseStateLine(std::string s);//utility to read all the states from config file
    void parseState(std::string path);//needed by parseStateLine



};

#endif // STATE_HANDLER_H
