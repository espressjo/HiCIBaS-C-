#include <iostream>
#include "python_proc.h"
#include <string.h>
#include <cstring>
#include <unistd.h>
using namespace std;

#define PY_INT "/home/noboru/miniconda3/bin/python"
int main()
{
cout<<"PID: "<<getpid()<<endl;
    

    python_proc py1(PY_INT);
    py1.run("/home/noboru/Documents/HiCIBaS/HiCIBaS-C++/py_proc/prog1.py");

    //python_proc py2(PY_INT);
    //py2.run("/home/noboru/Documents/HiCIBaS/HiCIBaS-C++/py_proc/prog2.py");
    string test;
    //cout<<"starting py2"<<endl;
    //py2.run_wait("/home/noboru/Documents/HiCIBaS/HiCIBaS-C++/py_proc/prog2.py");
    char buff[64];
    
    while(1)
    {
        memset(buff,0,64);
        cout<<"Speak to me:"<<endl;
        //scanf("%s",buff);
        cin>>test;
        cout<<"I say "<<test<<endl;
        if (strcmp(test.c_str(),"run")==0)
        {
            cout<<"Child running: "<<py1.is_child_running()<<endl;
        }
        else if (strcmp(test.c_str(),"end")==0)
        {
            break;
        }
        else if (strcmp(test.c_str(),"kill")==0)
        {
            cout<<"Kill: "<<py1.stop_child()<<endl;
        }
        else {cout<<"WTF?"<<endl;}
    }
    
    sleep(1);
    cout<<"py1: "<<endl;
    cout<<"stdout: \n"<<py1.get_stdout()<<endl;
    cout<<"stderr: \n"<<py1.get_stderr()<<endl;

    

    return 0;
}
