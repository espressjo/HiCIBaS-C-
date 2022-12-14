#ifndef PYTHON_MANAGER_H
#define PYTHON_MANAGER_H

#include "python_proc.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class py_manager
{
public:
    py_manager(string python_interpreter_binary);
    ~py_manager();
    int add_python_script(string py_file);
    bool is_running(string py_file);
    int get_stdout(string py_file,string *stdout);
    int get_stderr(string py_file,string *stderr);
    int get_return_code(string py_file,int *ret);
    int get_pid(string py_file,int *pid);
    int kill_script(string py_file);
    int run(string py_file);
    vector<string> get_available_scripts();
    vector<string> whos_running();
    vector<string> whos_finished();
private:
    bool file_exist(std::string  fname);
    int get_loc(string py_file);//get the location in vector of py_file
    string interpreter;
    vector<python_proc*> available_scripts; //Avail. scr. |  running   |  finished
                                           //------------------------------------
    vector<bool> running_scripts;          //script1.py  |  T         |  F
    vector<bool> finished_scripts;         //script2.py  |  F         |  T
                                           //script3.py  |  F         |  F
};



#endif // PYTHON_MANAGER_H


