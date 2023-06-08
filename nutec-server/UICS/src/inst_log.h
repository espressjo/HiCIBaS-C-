#ifndef INST_LOG_H
#define INST_LOG_H

#include <iostream>
#include "inst_time.h"
class Log
{
 std::string fileName;

public:
    Log();
    void setLevel(int level);
    std::string path;
    void writeto(std::string text);
    void writeto(std::string text,int level);
    void writetoVerbose(std::string text);
    int createFolder(std::string path);
    bool isFolder(std::string path);
    bool isFile(std::string path);
    void setPath(std::string path);

private:
   int level;
};

#endif 
