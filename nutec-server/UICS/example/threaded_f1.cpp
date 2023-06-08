#include "threaded_f1.h"
#define SUCCESS "ready"
#define FAILED "failed"
#include "uics_config.h"


void runThread()
{
    std::string msg="";
    cmd *c = new cmd();

    int fd = create_socket(5011);
    //cmdList all(CMDCONFFILE);

    while (1) {
        c->recvCMD(fd);

        std::cout << "this is the command: " << std::endl;

        c->printCMD();


        std::cout<<"execution of thread test"<<std::endl;
        sndMsg(c->sockfd);

    }
}
