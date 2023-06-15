#include <iostream>
#include <thread>
#include "uics.h"
#include "threaded_f1.h"
#include "test.h"
using namespace std;

int main()
{
    // Begin the thread process
    std::thread t_t1(&runThread);
    t_t1.detach();

    // Begin the message handler
    msgHandler msgH;
    std::thread t_msg(&msgHandler::run,&msgH);
    t_msg.detach();
    sleep(1);

    // Begin the state handler
    instHandle handle;
    state_handler sHandler(&handle);
    sHandler.s_config->add_callback("setParam",megaTest2);
	




  


    sHandler.run();


    return 0;
}
