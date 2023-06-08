#include "fhtable.h"

fhashtable::fhashtable()
{

}
func_ptr fhashtable::operator [](const char *S){

    int ret = this->isInList(std::string(S));
    if (ret<0){
        return nullptr;
    }
    return func[static_cast<size_t>(ret)];
}

int fhashtable::isInList(std::string S){
    for (size_t i=0;i<sVal.size();i++ ){
        if (sVal[i].compare(S)==0)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void fhashtable::add_value(const char *head, void (*f)(instHandle*,cmd*))
{
    sVal.push_back(std::string(head));

    func.push_back(f);
    return ;
}


//cmdhashtable::cmdhashtable()
//{
//this->add_value("montest",3,CONFIG);
//this->add_value("montest2",2,CONFIG);
//}

//CMD *cmdhashtable::operator [](const char *S){

//    int ret = this->isInList(std::string(S));

//    if (ret<0){

//        return nullptr;
//    }
//    return func[static_cast<size_t>(ret)];
//}

//int cmdhashtable::isInList(std::string S){
//    for (size_t i=0;i<sVal.size();i++ ){
//        if (sVal[i].compare(S)==0)
//        {
//            return static_cast<int>(i);
//        }
//    }
//    return -1;
//}

//void cmdhashtable::add_value(const char *head,int nbArg, STATE state)
//{   CMD *cmd = new (CMD);
//    //CMD cc;
//    cmd->nbArg = nbArg;
//    cmd->state = state;
//    //cmd = &cc;
//    sVal.push_back(std::string(head));

//    func.push_back(cmd);
//    return ;
//}

