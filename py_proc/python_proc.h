#ifndef PYTHON_PROC_H
#define PYTHON_PROC_H

#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <vector>

#define MAX_STD_BUFFER 2500//max. characters read from stdout and stderr

using namespace std;

class python_proc
{
public:
    //constructure
    python_proc(string python_interpreter_binary);
    python_proc(string python_interpreter_binary,string py_script);
    ~python_proc();
    
    int run();//run the script as a child. py_script initialize with constructer
	int run(vector<string> args);//same as run() but with arguments
    int run(string py_script);//run the script as a child
    int run_wait();//run the script as a child. Wait for the child to finish running. 
    int run_wait(string py_script);//run the script as a child. Wait for the child to finish running. py_script initialize with constructer
    bool is_child_running();//true -> child still running, false -> child not running anymore.
    string get_stdout();//once child is done, return stdout. return empty string if no stdout
    string get_stderr();//once child is done, return stderr. return empty string if no stderr
//    static void signal_test(int signum);//example of signal.
    int stop_child();//stop a running child
    int get_return_value();//get return value. Must be called after a child has exited
    string get_script_name();//return the fullpath +name of the script
    int get_pid();//return child pid
private:
	vector<string> args;
    int return_value;
    pid_t child_pid;
    std::string py_script_name,py_script_location;
    int fd_stdout[2];//file descriptor for stdout
    int fd_stderr[2];//file descriptor for stderr
    std::string get_program_name(std::string py_file);//return name of python script (/home/user/name.py -> name)
    string py_bin;//location of python interpreter
    pid_t parent_pid;//pid of the parent and child (python script) process
    int spawn(pid_t *chld_pid);
    int newfd_stderr,newfd_stdout;
    void clean_up_child_process (int signal_number);
};

#endif // PYTHON_PROC_H


