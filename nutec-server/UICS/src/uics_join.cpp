#include "uics_join.h"

std::string join(std::string a,std::string b)
{
    if (a[a.size()-1]!='/')
    {
        return a+"/"+b;
    }
    else {
        return a+b;
    }
}
