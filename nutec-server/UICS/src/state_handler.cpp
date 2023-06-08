

#include "state_handler.h"
#include "inst_log.h"

#define SUCCESS "ready"
#define FAILED "failed"



//test function (normally not defined here)

void f_test(instHandle *handle,cmd *c)
/*!
  *\brief Default test function
  * A simple default callback call back function
  * that sent answer to client.
  */
{
    std::cout<<"Success!!!"<<std::endl;


    sndMsg(c->sockfd);
    return;
}
void f_test2(instHandle *handle,cmd *c)
/*!
  *\brief Default test function
  * A simple default callback call back function
  * that sent answer to client.
  */
{
    std::cout<<"Success!!!"<<std::endl;
sndMsg(c->sockfd);
return;
}

state_handler::state_handler(instHandle *handle)
/*!
  *\brief State handler constructor
  * Will create the port to communicate with the different
  * states and the message handler. Also upload the state
  * transition file. Initialize all the states and add the
  * callback function. By default, the first state to be
  * executed if the default state INIT.
  * */
{

    commfd = create_socket(PORT4MSG);
    sockfd = create_socket(PORT4STATE);//pour les states
    //parsing the state config file.
    parseState(join(CONFPATH,"state.conf"));

    msg = "";
    myHandle = handle;


    myHandle->nextState = INIT;
    myHandle->state = INIT;

    //init all states
    s_config= new uics_state(handle);

    s_init= new uics_state(handle);
    s_acq = new uics_state(handle);
    s_idle= new uics_state(handle);
    //add more state here

    //add a few callback..

}

void state_handler::run()
/*!
  * \brief Main method
  * Will start the incoming message thread. This is an infinite
  * loop that will run the different state. If another state
  * needs to be added, one must difine a new case for this state
  * here.
  * */
{
    std::thread t_handler(&state_handler::handler,this);
    //start the state incoming msg handler.
    t_handler.detach();
    sleep(1);
    while (1) {
        switch (myHandle->nextState) {
            case ACQ:{
                //always set the current state
                std::cout<<"Starting ACQ"<<std::endl;
                log_stateH("Starting ACQ");
                myHandle->state = ACQ;
                s_acq->run(sockfd);
                std::cout<<"Exit ACQ"<<std::endl;
                log_stateH("Exit ACQ");
                break;
            }
            case IDLE:{
                //always set the current state
                std::cout<<"Starting IDLE"<<std::endl;
                log_stateH("Starting IDLE");
                myHandle->state = IDLE;
                s_idle->run(sockfd);
                std::cout<<"Exit IDLE"<<std::endl;
                log_stateH("Exit IDLE");
                break;
            }
            case INIT:{
                //always set the current state
                std::cout<<"Starting INIT"<<std::endl;
                log_stateH("Starting INIT");
                myHandle->state = INIT;
                s_init->run(sockfd);
                std::cout<<"Exit INIT"<<std::endl;
                log_stateH("Exit INIT");
                break;
            }
            case CONFIG:{
                //always set the current state
                std::cout<<"Starting CONFIG"<<std::endl;
                log_stateH("Starting CONFIG");
                myHandle->state = CONFIG;
                s_config->run(sockfd);
                std::cout<<"Exit CONFIG"<<std::endl;
                log_stateH("Exit CONFIG");
                break;
            }

            default:{
                sleep(1);
                std::cout<<"big problem"<<std::endl;
                log_stateH("major issue");
                break;
            }

        }//switch
    }//while
}


int state_handler::checkState(cmd *cc,bool requestChange)
/*!
  *\brief Description
  * Check if the requested state is compatible with the
  * current state. If not, a failure msg will be sent to
  * the command fd.
  */
{
    if (myHandle->state!=cc->state && requestChange)
    {
        //deal with the change of state.
        //check if we are allowed to got to next state

        for (auto c: state_chart)
        {
            if (myHandle->state==c[0] && cc->state==c[1]){

                myHandle->nextState = cc->state;//set nextState
                //if there is a defined exit function in the uics_state, execute it before
                //should be eventually change with a state pointer, because for now if a new
                //state is defined, a case needs to be implemented as well.
                switch (myHandle->state) {
                case IDLE:
                {
                    s_idle->execute("exit");
                    break;
                }
                case ACQ:
                {
                    s_acq->execute("exit");
                    break;
                }
                case INIT:
                {
                    s_init->execute("exit");
                    break;
                }
                case CONFIG:
                {
                    s_config->execute("exit");
                    break;
                }
                case THREAD:
                {
                    break;
                }

                }
                write_socket(PORT4STATE,std::string("EXIT "+std::to_string(cc->state)));
                read_socket(PORT4STATE,&msg);
                if (msg.compare(SUCCESS)==0){
                    return 0;
                }
                else {
                    return -1;
                }
             }//if
        }
        return -1;
    }
    else if (myHandle->state!=cc->state and !requestChange) {
        //error message...
        log_stateH("major issue-2");
        return -1;
    }
    else {
        //actually the right state
        return 0;
    }
}



void state_handler::handler()
{
    std::string answ="";
    while (1) {
        // wait for incoming command
        read_socket(&msg,commfd);
        if (msg.compare("INCOMINGCMD")!=0)
        {
            write_socket(std::string(FAILED),commfd);//should have timeout
            continue;
        }
        else {       
            if (write_socket(std::string(SUCCESS),commfd)!=0)//should have timeout
            {
                std::cout<<"[state handler] NOK."<<std::endl;
                log_stateH("major issue-3");
                //continue???
            }
        }

        //Receiving the command
        cmd *c = new cmd();
        c->recvCMD(commfd);

        // Verify the retrieval of the command
        if (!c)
        {
            write_socket(std::string(FAILED),commfd);
            continue;
        }
        // ok the command is legit so will send READY to mHandler
        if (write_socket(std::string(SUCCESS),commfd)!=0)
        {
            std::cout<<"[state handler] NOK."<<std::endl;
            log_stateH("major issue-4");
            //continue ???
        }

        //checking if state if compatiblestd::cout<<"Maintenent!"<<std::endl;
        if (checkState(c,true)!=0)
        {
            sndMsg(c->sockfd,"wrong state or unable to change state",uicsCMD_ERR_NOT_SUPPORTED);
            log_stateH("wrong state or unable to change state");
            continue;
        }
        if (write_socket(PORT4STATE,std::string("INCOMINGCMD"))!=0)
        {
            std::cout<<"[state handler] NOK."<<std::endl;
            log_stateH("NOK");
            //continue ???
        }

        answ="";
        read_socket(PORT4STATE,&answ);

        if (answ.compare(SUCCESS)!=0)
        {

            sndMsg(c->sockfd,"Unable to execute the state",uicsCMD_ERR_NOT_SUPPORTED);
            log_stateH("Unable to execute the state");

            continue;
        }
        else {
        // Sending command to UICS_STATE
			std::cout<<"ICI"<<std::endl;
            c->sendCMD(PORT4STATE);

            read_socket(PORT4STATE,&answ);
            if (answ.compare(SUCCESS)!=0)
            {
                log_stateH("Major issue-5");
                std::cout<<"we need to fix this"<<std::endl;
                sndMsg(c->sockfd,"Unknown error",uicsCMD_ERR_NOT_SUPPORTED);
            }
        }


    }


}
std::vector<STATE> state_handler::parseStateLine(std::string s)
{   std::vector<std::string> line;
    std::string buff="";
    for (auto &c:s)
    {   if (c=='#')
        {
            break;
        }
        if (c!=' ' && c!='\n' && c!='\r' && c!='\t')
        {
            buff+=c;
        }
        else {
            line.push_back(buff);
            buff="";
        }
    }
    if (buff.size()>0)
    {
        line.push_back(buff);
    }



    std::vector<STATE> chart;
    for (int i=0;i<line.size();++i) {
        if (line[i].compare("IDLE")==0)
        {
            chart.push_back(IDLE);
        }
        else if (line[i].compare("INIT")==0)
        {
            chart.push_back(INIT);
        }
        else if (line[i].compare("ACQ")==0)
        {
            chart.push_back(ACQ);
        }
        else if (line[i].compare("CONFIG")==0)
        {
            chart.push_back(CONFIG);
        }
    }
    return chart;

}
void state_handler::parseState(std::string path)
{

   std::ifstream conf_file;
   std::string buff;
   conf_file.open(path.c_str());
   while(getline(conf_file,buff))
   {
           if (buff[0]=='#')
           {
               continue;
           }

           state_chart.push_back(parseStateLine(buff));

   }


   conf_file.close();




    return;
}
