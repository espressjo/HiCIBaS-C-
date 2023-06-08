#include "uics_state.h"
#include "uics_sockets.h"


#define FAILED "failed"
#define SUCCESS "ready"

#define EXIT 229


uics_state::uics_state(instHandle *handle)
{
    myHandle = handle;
    eOO = false;
    SW = false;
    SW_fname = "";
    eOOnextState = myHandle->state;
}

bool uics_state::isInList(std::string name)
/*!
  *Check if name if in function list.
  * */
{
    for (auto n : ffname)
    {   
        if (n.compare(name)==0){return true;}
    }
    return false;
}

void uics_state::start_with(const char *fname)
{
    SW_fname = fname;
    SW = true;
}
void uics_state::run(int sockfd)
/*!
  *\brief main function
  * The run method wait for an incoming command. Once all the check
  * up is OK (cmd exist, argument are ok,...) we check if the requested
  * callback function is defined.
  * */
{
    while(1)
    {
        //if start_with has been used, start by launching this function
        if (SW)
        {
            execute(SW_fname.c_str());
        }
        int out = getNextConnection(sockfd);

        if (out==0)
        {
            if (!cmd2exec)
            {
                //std::cout<<"super problem!!"<<std::endl;
                log_state_transition("Major issue.");
            }
            else {
                if (isInList(cmd2exec->name))
                {
                    execute(cmd2exec->name.c_str());
                    if (eOO)
                    {
                        log_state_transition("Transition " +std::to_string(myHandle->nextState)+"->"+std::to_string(eOOnextState));
                        myHandle->nextState = eOOnextState;
                    }
                    break;
                }
                else
                {
                    log_state_transition("function not recognized");
                    sndMsg(cmd2exec->sockfd,"function not recognized",uicsCMD_ERR_NOT_SUPPORTED);
                }
            }
        }
        else if (out==EXIT) {break;}
    }
}
void uics_state::execute(const char *fname)
/*!
  * \brief Description
  * method to execute a standard function. A standard function
  * accept one argument of type cmd, return void and must be
  * 1st set using the add_config_func method.
  * This method will execute the command named fname with *cc
  * argument.
  */
{
    void (*f)(instHandle*,cmd*);
    f = functions[fname];
    if (f!=nullptr)
    {
        f(myHandle,cmd2exec);
    }

    return ;
}

void uics_state::setNextState(int state)
/*!
  * Will set the handle->nextState to state value.
  * This is required when a change of state is
  * requested since the state handler must know
  * which state to run.
  * */
{
    if (ACQ==state)
    {
        myHandle->nextState = ACQ;
    }
    else if (IDLE==state)
    {
        myHandle->nextState = IDLE;
    }
    else if (INIT==state)
    {
        myHandle->nextState = INIT;
    }
    else if (CONFIG==state)
    {
        myHandle->nextState = CONFIG;
    }
}
void uics_state::executeOnlyOnce(STATE nextState)
/*!
  * If this fucntion is called, the uics_state::run
  * will only execute one function. This functionallity
  * is usefull for the acquisition combine with a
  * idle state. The uics_state(acq) only has 1 function;
  * acquisition(). Once the function return, we may
  * tell the uics_state to go to another state e.g.,
  * the idle state
  */
{
    eOO = true;
    eOOnextState = nextState;
}
int uics_state::getNextConnection(int fd)
/*!
  * \brief Description
  * method that manage incoming commands. Will return
  * -1 if something goes wrong.
  * */
{
    std::string msg="";
    read_socket(&msg,fd);
    if (msg.compare("INCOMINGCMD")==0){
        write_socket(std::string(SUCCESS),fd);

        // Receiving the command
        cmd2exec = new cmd();
        cmd2exec->recvCMD(fd);

        if (!cmd2exec)
        {
            write_socket(std::string(FAILED),fd);
            std::cout<<"big big problem"<<std::endl;
            return -1;
        }

    
    //if fail write on socket -1 else return 0 and write socket
    write_socket(std::string(SUCCESS),fd);
    return 0;
    }
    else if (msg.substr(0,4).compare("EXIT")==0) {
        //request a change of state
        int nexState = std::atoi(msg.substr(5,1).c_str());
        setNextState(nexState);//setting nex state
        write_socket(std::string(SUCCESS),fd);
        return EXIT;
    }
    else {
        write_socket(std::string(FAILED),fd);
        return -1;
    }
}

void uics_state::add_callback(const char *fname, void (*f)(instHandle *, cmd *))
/*!
  * Add a function callback to the class. A standard function
  * accept one argument of type cmd, return void and must be
  * 1st set using the add_config_func method.
  */
{
    
    functions.add_value(fname,f);
    ffname.push_back(std::string(fname));
}





