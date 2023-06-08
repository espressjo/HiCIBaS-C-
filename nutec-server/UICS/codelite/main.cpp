#include <iostream>
#include <thread>
#include "uics.h"

#include <unistd.h>
void test(instHandle *handle,cmd *cc)
{

	
	(*cc).respond("test");
	return ;
}

void test2(instHandle *handle,cmd *cc)
{

	sleep(2);
	//sleep(1);
	(*cc).respond("test2");
	return ;
}


using namespace std;

int main()
{
    // Begin the thread process


    // Begin the message handler
    msgHandler msgH;
	udp_msgHandler udp_msgH;
    std::thread t_msg(&msgHandler::run,&msgH);
    t_msg.detach();
    sleep(1);
	std::thread t_msg_udp(&udp_msgHandler::run,&udp_msgH);
    t_msg_udp.detach();
    sleep(1);

    // Begin the state handler
    instHandle handle;
    state_handler sHandler(&handle);
    sHandler.s_config->add_callback("test",test);
	sHandler.s_config->add_callback("test2",test2);
    sHandler.run();


    return 0;
}