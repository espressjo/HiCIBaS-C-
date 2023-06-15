#ifndef FHTABLE_H
#define FHTABLE_H

#include <vector>
#include <string>
#include <iostream>
#include <string>

#include <vector>
#include "uics_cmds.h"
#include "insthandle.h"


typedef void (*func_ptr)(instHandle*,cmd*);

class fhashtable
{
public:

    fhashtable();//function hash table
    void add_value(const char *head, void (*f)(instHandle *, cmd *));
    func_ptr operator [](const char *S);
int isInList(std::string);
private:
    std::vector<std::string> sVal;
    std::vector<void (*)(instHandle*,cmd*)> func;
};



#endif // FHTABLE_H
