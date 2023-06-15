#include "test.h"
void megaTest(instHandle *handle,cmd *cc)
{
    for (int var = 0; var < cc->nbArg; ++var) {
        std::cout<<"arg"<<var+1<<": "<<cc->argsVal[var]<<std::endl;
    }


sndMsg(cc->sockfd);
return ;


}
void test(std::string s)
{
    std::cout<<s<<std::endl;
    return ;
}
void megaTest2(instHandle *handle,cmd *cc)
{
    //(*cc).printCMD();
    cc->printCMD();
    std::string p="";
    p = (*cc)["read"];
    if (p.compare("")!=0)
    {
        //test("read: "+p);
        std::cout<<(*cc)["read"]<<std::endl;
    }
    p = (*cc)["ramp"];
    if (p.compare("")!=0)
    {
        std::cout<<static_cast<uint>(std::atoi((*cc)["ramp"].c_str()))<<std::endl;
    }
    p = (*cc)["reset"];
    if (p.compare("")!=0)
    {
        std::cout<<static_cast<uint>(std::atoi((*cc)["reset"].c_str()))<<std::endl;
    }

sndMsg(cc->sockfd);
return ;


}
