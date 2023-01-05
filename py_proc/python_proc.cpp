#include "python_proc.h"

//void signal_test(int signum);//test to generate a signal


//void python_proc::signal_test(int signum)
/*
 * Function executed whenever SIGCHLD is emited.
 */
//{
//    cin.clear();
//}


python_proc::python_proc(string python_interpreter_binary)
/* 
 * Description
 * -----------
 *      Class to manage the execution of a python
 *      script, it's output (stdout, stderr and 
 *      return value).
 * Argument
 * --------
 *      *python_interpreter_binary: STR
 *          full path where the python interpreter
 *          is installed.
 */
{
    py_bin = python_interpreter_binary;//full path to python interpreter binary file
    py_script_name = "";//name of the script [script].py. Used to set th 1st argument of exec
    py_script_location = "";//location of the python script (include the python script.py name)
    return_value =0;//child return value
}
python_proc::python_proc(string python_interpreter_binary,string py_script)
/* 
 * Description
 * -----------
 *      Class to manage the execution of a python
 *      script, it's output (stdout, stderr and 
 *      return value).
 * Argument
 * --------
 *      *python_interpreter_binary: STR
 *          full path where the python interpreter
 *          is installed.
 */
{
    py_bin = python_interpreter_binary;//full path to python interpreter binary file
    py_script_location = py_script;//location of the python script (include the python script.py name)
    py_script_name = get_program_name(py_script);//name of the script [script].py. Used to set th 1st argument of exec
    return_value =0;//child return value
}
string python_proc::get_script_name()
/*
 * Description
 * -----------
 *      Will return the fullpath+name of the 
 *      python script i.e., /home/user/script.py
 */ 
{
    
    return py_script_location;
}
//TODO add DOC here
int python_proc::get_pid(){return static_cast<int>(child_pid);}

int python_proc::run()
/*
 * Description
 * -----------
 *      run a python script. This method
 *      will return immediately. Use the 
 *      is_child_running() method to pole
 *      the status of the script. Assume 
 *      py_script was initialize with constructor
 * Return
 * ------
 *      If we cannot spawn the script, we return
 *      errno, otherwise we return 0 if successful. 
 *      if py_script is not defined i.e., you used
 *      the wrong constructor or method we will return
 *      -1.
 */ 
{
	args.clear();
    if (strcmp(py_script_location.c_str(),"")==0)//you used wrong constructor
    {
        return -1;
    }    
    if (spawn(&child_pid)!=0)
    {
        return errno;    
    }
    
    return 0;
}
int python_proc::run(vector<string> args)
/*
 * Description
 * -----------
 *      run a python script. This method
 *      will return immediately. Use the 
 *      is_child_running() method to pole
 *      the status of the script. Assume 
 *      py_script was initialize with constructor
 * Return
 * ------
 *      If we cannot spawn the script, we return
 *      errno, otherwise we return 0 if successful. 
 *      if py_script is not defined i.e., you used
 *      the wrong constructor or method we will return
 *      -1.
 */ 
{
	python_proc::args = args;
    if (strcmp(py_script_location.c_str(),"")==0)//you used wrong constructor
    {
        return -1;
    }    
    if (spawn(&child_pid)!=0)
    {
        return errno;    
    }
    
    return 0;
}
int python_proc::run(std::string py_script)
/*
 * Description
 * -----------
 *      run a python script. This method
 *      will return immediately. Use the 
 *      is_child_running() method to pole
 *      the status of the script. 
 * Arguments
 * ---------
 *     *py_script: STR
 *      full path+ name of the python script
 * Return
 * ------
 *      If we cannot spawn the script, we return
 *      errno, otherwise we return 0 if successful. 
 */ 
{
    /* Handle SIGCHLD by calling clean_up_child_process.  */
    py_script_name = get_program_name(py_script);
    py_script_location = py_script;
    
    if (spawn(&child_pid)!=0)
    {
        return errno;    
    }
    
    return 0;
}
int python_proc::run_wait()
/*
 * Description
 * -----------
 *      run a python script. This method
 *      will wait for the script to terminate
 *      before returning. 

 * Return 
 * ------
 *      Technically, this method should always
 *      return 0. However, if a problem should 
 *      occured it will return -1 as well as if 
 *      you used the wrong constructor/method. 
 *      If we are unable to fork the child we 
 *      will return errno.
 */ 
{
    
    if (strcmp(py_script_location.c_str(),"")==0)
    {
        return -1;
    }
    sig_atomic_t stat_loc;
    
    int ret = spawn(&child_pid);
    if (ret!=0)
    {
        return errno;
    }
    pid_t c_pid = waitpid(child_pid,&stat_loc,0);
    
    //by design it should always be true
    if (c_pid==child_pid)
    {
        return_value = stat_loc;//set the return value
        return 0;
    }
    //otherwise we might want to try to kill the process
    cout<<"This should not be happening. Lets try to kill the child"<<endl;
    stop_child();
    return -1;
}
int python_proc::run_wait(std::string py_script)
/*
 * Description
 * -----------
 *      run a python script. This method
 *      will wait for the script to terminate
 *      before returning. 
 * 
 * Arguments
 * ---------
 *     *py_script: STR
 *      full path+ name of the python script
 * Return 
 * ------
 *      Technically, this method should always
 *      return 0. However, if a problem should 
 *      occured it will return -1. If we are
 *      unable to fork the child we will return errno.
 */ 
{
    
    py_script_name = get_program_name(py_script);
    py_script_location = py_script;
    sig_atomic_t stat_loc;
    
    int ret = spawn(&child_pid);
    if (ret!=0)
    {
        return errno;
    }
    pid_t c_pid = waitpid(child_pid,&stat_loc,0);
    
    //by design it should always be true
    if (c_pid==child_pid)
    {
        return_value = stat_loc;//set the return value
        return 0;
    }
    //otherwise we might want to try to kill the process
    cout<<"This should not be happening. Lets try to kill the child"<<endl;
    stop_child();
    return -1;
}
string python_proc::get_stdout()
/*
 * Description
 * -----------
 *      Once the child process finishes, a signal is emited
 *      and is_child_running will return false. Only then,
 *      get_stdout can be used to read the stdout. This method
 *      can only be called once. Once called, the pipe is emptied
 * 
 * Return
 * ------
 *      child stdout as a string. Max MAX_STD_BUFFER characters.
 */
{
    char buff_c[MAX_STD_BUFFER];//buffer where the FD will be copied.
    stringstream err; 
    //set the file descriptor in non-blocking mode
    int flags = fcntl(fd_stdout[0], F_GETFL, 0);//get the flags of fd_stdout
    if (flags==-1)
    {
        err<<"[py_proc] Error accessing the stdout file descriptor, errno: "<<errno<<endl;
        return err.str();
    }
    if (fcntl(fd_stdout[0], F_SETFL, flags | O_NONBLOCK)==-1)
    {
        err<<"[py_proc] Error accessing the stdout file descriptor, errno: "<<errno<<endl;
        return err.str();
    } 
    memset(buff_c,0,MAX_STD_BUFFER);
    //we don't test in case there is really nothing in the pipe
    read(fd_stdout[0],buff_c,sizeof(buff_c));
    
   
    return std::string(buff_c);
}
string python_proc::get_stderr()
/*
 * Description
 * -----------
 *      Once the child process finishes, a signal is emited
 *      and is_child_running will return false. Only then,
 *      get_stderr can be used to read the stdout. This method
 *      can only be called once. Once called, the pipe is emptied
 * 
 * Return
 * ------
 *      child stderr as a string. Max MAX_STD_BUFFER characters.
 */
{
    char buff_c[MAX_STD_BUFFER];//buffer where the FD will be copied.
    stringstream err; 
    int flags = fcntl(fd_stderr[0], F_GETFL, 0);
    if (flags==-1)
    {
        err<<"[py_proc] Error accessing the stderr file descriptor, errno: "<<errno<<endl;
        return err.str();
    }
    if (fcntl(fd_stderr[0], F_SETFL, flags | O_NONBLOCK)==-1)
    {
        err<<"[py_proc] Error accessing the stderr file descriptor, errno: "<<errno<<endl;
        return err.str();
    }
    
    memset(buff_c,0,MAX_STD_BUFFER);
    //we don't test read() in case there is really nothing in the pipe
    read(fd_stderr[0],buff_c,sizeof(buff_c));
    
    return std::string(buff_c);
}
std::string python_proc::get_program_name(std::string py_file)
/*
 * Description
 * -----------
 *      Given a string which include a python script name e.g.,
 *      /home/user/test.py, this function will return the name
 *      of the script i.e., test. Argument py_file may or may not
 *      include an absolute path. If no xxx.py string is found,
 *      an empty string will be returned.
 * Return 
 * ------
 *      return the name of a script i.e., /home/user/<name>.py
 */
{
    char *ptr; // declare a ptr pointer
    ptr = strtok((char*)py_file.c_str(), "/");
    std::string p_name = "";
    size_t pos;
    while (ptr != nullptr)
    {
        pos = std::string(ptr).find(".py");
        if (pos!=std::string::npos)
        {
            p_name = std::string(ptr).substr(0,pos);
            break;
        }
        ptr = strtok (nullptr, "/");
    }
    return p_name;
}

int python_proc::stop_child()
/*
 * Description
 * -----------
 *      Stop a script by sending ctrl-c signal  
 *      to the child process. Once this method
 *      is called, use get_stdout, get_stderr
 *      and get_return_code to read the the 
 *      output. However, you should only get
 *      information from stderr.
 * Return
 * ------
 *      0 means at least one signal was sent, 
 *      any other output is the correspondin 
 *      linux errno. 
 */ 
{
    if (kill(child_pid,SIGINT)!=0)
    {
        return errno;
    }
    return 0;
}
int python_proc::spawn(pid_t *chld_pid)
/*
 * Description
 * -----------
 *      fork and exec the script. child_pid will
 *      be set as well as and the child sigaction
 *      callback. Two sets of pipe are set. One is
 *      for the stdout and the other for the stderr.
 *      Child running status can be pole with the 
 *      is_child_running method.
 * Return 
 * ------
 *      return the pid of the child. 
 */ 
{
    //setup the SIGCHLD signal callback
    
    //struct sigaction sigchld_action;
    //memset (&sigchld_action, 0, sizeof (sigchld_action));
    //sigchld_action.sa_handler = &python_proc::signal_test;
    //sigaction (SIGCHLD, &sigchld_action, NULL);
    
    //create pipes to communicate between parent and child
    if (pipe(fd_stderr)!=0)
    {//errno will be set
        *chld_pid=0;
        cout<<"Unable to create a pipe"<<endl;
        return errno;
    }
    if (pipe(fd_stdout)!=0)
    {//errno will be set
        *chld_pid=0;
        cout<<"Unable to create a pipe"<<endl;
        return errno;
    }
    
    pid_t ch_pid = fork();
    if (ch_pid<0)
    {
        *chld_pid=0;
        cout<<"Unable to fork()"<<endl;
        return errno;  
    }
    if (ch_pid!=0)
    {
        //Parent process
        parent_pid = getpid();
        close(fd_stdout[1]);//we don't intend to write from parent
        close(fd_stderr[1]);//we don't intend to write from parent
        *chld_pid = ch_pid;
        return 0;
    }
    else
    {
        //child process
        //cout<<"Child pid: "<<getpid()<<endl;

        //Deal with the pipes
        close(fd_stdout[0]);//we don't intend to read from child
        close(fd_stderr[0]);//we don't intend to read from child
        newfd_stdout = dup2(fd_stdout[1],STDOUT_FILENO);//redirect stdout
        newfd_stderr = dup2(fd_stderr[1],STDERR_FILENO);//redirect stderr
        close(fd_stdout[1]);
        close(fd_stderr[1]);

        //launch the script here
		if (args.size()==0){
			execlp(py_bin.c_str(),py_script_name.c_str(),py_script_location.c_str(),NULL);
		}
		else if (args.size()==1)
		{
			execlp(py_bin.c_str(),py_script_name.c_str(),py_script_location.c_str(),args[0].c_str(),NULL);
			
		}
		else if (args.size()==2)
		{
			execlp(py_bin.c_str(),py_script_name.c_str(),py_script_location.c_str(),args[0].c_str(),args[1].c_str(),NULL);

		}
		else if (args.size()==3)
		{
			execlp(py_bin.c_str(),py_script_name.c_str(),py_script_location.c_str(),args[0].c_str(),args[1].c_str(),args[2].c_str(),NULL);

		}
		else if (args.size()==4)
		{
			execlp(py_bin.c_str(),py_script_name.c_str(),py_script_location.c_str(),
			args[0].c_str(),args[1].c_str(),args[2].c_str(),args[3].c_str(),NULL);
			
		}
		else if (args.size()==5)
		{
		execlp(py_bin.c_str(),py_script_name.c_str(),py_script_location.c_str(),
			args[0].c_str(),args[1].c_str(),args[2].c_str(),args[3].c_str(),args[4].c_str(),NULL);	
		}
		else if (args.size()==6)
		{
			execlp(py_bin.c_str(),py_script_name.c_str(),py_script_location.c_str(),
			args[0].c_str(),args[1].c_str(),args[2].c_str(),args[3].c_str(),args[4].c_str(),args[5].c_str(),NULL);	
		}
		return 0;//this will never happen.
    }
}
int python_proc::get_return_value()
/* 
 * Description
 * ------------
 *      Get the return value of the child
 * return
 * -------
 *      Will return -1 if child is still running, otherwise 
 *      it will return the return value of the child.
 */ 
{
    sig_atomic_t stat_loc=0;
    pid_t ret = waitpid(child_pid,&stat_loc,WNOHANG);//should return the pid of the child
  
    if (ret==0)
    {
        cout<<"Process not yet finish"<<endl;
        cout<<"errno: "<<errno<<endl;
        return -1;
    }
    else if (ret==child_pid)
    {//
        cout<<"Process still exist but is finished"<<endl;
        return WEXITSTATUS(stat_loc);
    }
    else if (ret<0)
    {//in this case the child is already executed and 
    //probably waitpid was called by is_child_running.  
        cout<<"waitpid==-1, errno: "<<errno<<endl;
        return return_value;
    }
    return -1;
    }
bool python_proc::is_child_running()
/*
 * Description
 * ------------
 *      Check if the child is still running.
 * Return 
 * ------
 * running: BOOL
 *      Return T/F if the child process is still running.
 * Notes
 * ------
 *      There is 3 cases for waitpid. 1) ret==0, the 
 *      process is still running, 2) ret==-1, the process
 *      does not exist, and 3 ) ret==child_pid, the process
 *      is finished and the return value is poited by stat_loc.
 */
{
    pid_t ret;
    sig_atomic_t stat_loc;
    ret = waitpid(child_pid,&stat_loc,WNOHANG);
    if (ret==0 && child_pid!=0)
    {   //child have not yet changed state, careful it might mean 
        //it's still running or it's state has not change i.e., 
        //script if finish -> still finish
        return true;
    }
    else if (ret<0)
    {//process does not exist anymore
        return false;
    }
    else if (ret==child_pid)
    {//process finished, and this is the 1st call to waitpid since it ended
        return_value = WEXITSTATUS(stat_loc);
        return false;
    }
    return false;
}
python_proc::~python_proc()
/*
 * Make sure the file descriptor are closed.
 */ 
{
    //close all file descriptor
    close(fd_stdout[0]);
    close(fd_stderr[0]);
    close(newfd_stderr);
    close(newfd_stdout);
}
