#include <stdio.h>
#include "py_manager.h"

#define PY_INT "/home/noboru/miniconda3/bin/python"
#define SCRIPT1 "/home/noboru/Documents/HiCIBaS/HiCIBaS-C++/py_proc/prog1.py"
#define SCRIPT2 "/home/noboru/Documents/HiCIBaS/HiCIBaS-C++/py_proc/prog2.py"
#define SCRIPT3 "/home/noboru/Documents/HiCIBaS/HiCIBaS-C++/py_proc/prog3.py"

int main(int argc, char **argv)
{
    py_manager manager(PY_INT);
    string answer,script;
    
    manager.add_python_script(SCRIPT1);
    manager.add_python_script(SCRIPT2);
    manager.add_python_script(SCRIPT3);
    
    
    char myInput[128];
    
    cout<<"Test: "<<endl;
    for (auto c: manager.whos_running())
    {
        cout<<c<<endl;}
    
    string stdout="",stderr="";
    cout<<"list\tlist all avail. script"<<endl;
    cout<<"run\tlist running script"<<endl;
    cout<<"end\tlist all finished script"<<endl;
    cout<<"read\Read std i/o running script"<<endl;
    cout<<""<<endl;
    while(1)
    {        
        cout<<"What you want to do?"<<endl;
        memset(myInput,0,128);
        scanf("%s",myInput);
        
        if (strcmp(myInput,"stop")==0)
        {
            break;
        }
        else if (strcmp(myInput,"launch")==0)
        {
            cout<<"Which script you want to launch?"<<endl;
            memset(myInput,0,128);
            scanf("%s",myInput); 
            cout<<"Manager: "<<manager.run(myInput)<<endl;                
        }
        else if (strcmp(myInput,"list")==0)
        {
            for (auto c: manager.get_available_scripts())
            {
                cout<<"\t"<<c<<endl;
            }            
        }
        else if (strcmp(myInput,"run")==0)
        {
            for (auto c: manager.whos_running())
            {
                cout<<c<<endl;
            }            
        }
        else if (strcmp(myInput,"end")==0)
        {
            for (auto c: manager.whos_finished())
            {
                cout<<c<<endl;
            }            
        }
        else if (strcmp(myInput,"read")==0)
        {
            cout<<"Which script you want to read?"<<endl;
            memset(myInput,0,128);
            scanf("%s",myInput); 
            stdout="";
            stderr="";
            manager.get_stdout(myInput,&stdout);
            manager.get_stderr(myInput,&stderr);
            cout<<"::::::stdout::::::::\n"<<stdout<<endl; 
            cout<<"::::::stderr::::::::\n"<<stderr<<endl;        
        }
        else{
            cout<<"I don'T know!!!"<<endl;}
    }
    //manager.whos_finished();
    
    
    
    
    
    
    /*
    cout<<"[0]\texit"<<endl;
    cout<<"[1]\tList script"<<endl;
    cout<<"[2]\tRun script"<<endl;
    cout<<"[3]\tList running script"<<endl;
    cout<<"[4]\tList finnished script"<<endl;
    while(1)
    {
    cout<<"What do you want to do?"<<endl;
    cin>>answer;
    cout<<"Doing: "<<answer<<endl;
    switch(stoi(answer.c_str())){
        
    case 0:
        return 0;
        
    case 1:{
        cout<<"\n"<<endl;
        for (auto script:manager.get_available_scripts())
        {
            cout<<"\t"<<script<<endl;
        }
        cout<<""<<endl;
        break;
        }
     case 2: {
         cout<<"\nWhich script: "<<endl;
         cin>>script;
         cout<<":::"<<script<<":::"<<endl;
         cout<<"\tmanager: "<<manager.run(script)<<endl;
         break;
         }   
    case 3: {
        cout<<"Running: "<<endl;
        for (auto c: manager.whos_running())
        {
            cout<<"\t"<<c<<endl;
            cout<<""<<endl;
        }
        break;
        }
    case 4: {
        cout<<"Finnished: "<<endl;
        for (auto c: manager.whos_finished())
        {
            cout<<"\t"<<c<<endl;
            cout<<""<<endl;
        }
        break;
        }
        
    }//switch

    }//while
    */
	return 0;
}
