#ifndef UICS_STATE_H
#define UICS_STATE_H

#include "insthandle.h"
#include "uics_cmds.h"
#include "fhtable.h"

#include <sys/types.h>
#include <sys/socket.h>
#include "uics_config.h"
#include "uics_sndmsg.h"
#include "uics_err_code.h"
extern Log instLog;
inline void log_state_transition( std::string txt){instLog.writeto("[state] "+txt,1);}

class uics_state
{
public:

    uics_state(instHandle *handle);
    void run(int sockfd);

    void add_callback(const char *fname, void (*f)(instHandle*,cmd*));// void foo(instHandle *instH,cmd* myArguments)
    void execute(const char *fname);
    std::vector<std::string> ffname;
    void executeOnlyOnce(STATE nextState);
    void start_with(const char *fname);

private:
    bool isInList(std::string name);
    fhashtable functions;
    bool eOO;
    bool SW;
    std::string SW_fname;
    STATE eOOnextState;
    void setNextState(int state);

    
    cmd *cmd2exec;//to recv commands and dispatch to correct state
    int getNextConnection(int fd);
    instHandle *myHandle;


  

};

#endif // UICS_STATE_H
