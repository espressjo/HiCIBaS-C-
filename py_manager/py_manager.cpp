#include "py_manager.h"

py_manager::py_manager(string python_interpreter_binary)
/*
 * Description
 * -----------
 *      py_manager class will manage multiple python script.
 *      You can add script with add_python_script, then lauch
 *      them with run(). Once finished, you can read the stdout
 *      and stderr with get_stdXXX() method. While running use 
 *      whos_running and whos_finished method to get a list of
 *      running and finished script.
 *      
 * Arguments
 * ---------
 *      python_interpreter_binary:   full path + binary python interpreter
 *      
 */ 
{
    interpreter = python_interpreter_binary;
}
py_manager::~py_manager()
{
    
}


int py_manager::get_loc(string py_file)
/*
 * Description
 * -----------
 *      Find the location of py_file in available_scripts
 *      vector. This function is used to navigate between
 *      the running_script, finish_scirpt and available_script
 *      vectors.
 * Return 
 * ------
 *      the location of py_file in available_scripts. If 
 *      py_file does not exist it will return -1.
 */ 
{
    for (long unsigned int i=0;i<available_scripts.size();i++)
    {
        if (strcmp(available_scripts[i]->get_script_name().c_str(),py_file.c_str())==0)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}
int py_manager::kill_script(string py_file)
{
    int loc;
    loc = get_loc(py_file);
    if (loc<0)
    {
        return -2;
    }
    python_proc *script = available_scripts[loc];
    return script->stop_child();
    
}
int py_manager::get_pid(string py_file,int *pid)
{
    int ret,loc;
    loc = get_loc(py_file);
    if (loc<0)
    {
        return -2;
    }
    python_proc *script = available_scripts[loc];
    *pid = script->get_pid();
    return 0;
    
    
}
int py_manager::run(string py_file)
/*
 * Description
 * -----------
 *      Launch a python script. The script should be made 
 *      available by calling add_python_script before this 
 *      function is called. This will also set a "running" 
 *      flag for this script and flag down the "finish" vector.
 * Return 
 * ------
 *      0  -> Successfully executed a the script
 *      -1 -> Script is already running. close it 1st or wait for the end
 *      -2 -> script is not managed, call add_python_script 1st.
 *      Any other >0 represent errno.
 */ 
{
    int ret,loc;
    loc = get_loc(py_file);
    if (loc<0)
    {
        return -2;
    }
    python_proc *script = available_scripts[loc];
    
    if (script->is_child_running())
    {   //we cannot launch an already running script.
        return -1;
    }
    ret = script->run();
    if (ret==0)
    {   //we successfully launch a script
        running_scripts[loc]=true;
        finished_scripts[loc]=false;
        return ret;
    }
    return ret;//There is an error in py_proc.run() we return errno
}
vector<string> py_manager::whos_finished()
/*
 * Description
 * -----------
 *      We check which script is finish running. 
 * Return
 * ------
 *      Return list (string vector) of all the 
 *      scripts that as run and are now finished.
 */ 
{
    vector<string> finnished;
    int loc=-1;
    whos_running();//to flag every script that as finished
    for (python_proc *script: available_scripts)
    {
        loc = get_loc(script->get_script_name());
        if (finished_scripts[loc])
        {
            finnished.push_back(script->get_script_name());
        }
    }
    return finnished;
}
vector<string> py_manager::whos_running()
/*
 * Description
 * -----------
 *      We check which script is currently running. You
 *      can use this function to update the finish flags.
 * Return
 * ------
 *      return a vector of string which list all the
 *      running script.
 * 
 * Note
 * ----
 *      While we check every script, if a script is flagged 
 *      as running but is_child_running==false, we re-set the running
 *      flag and flag the script as finished.
 */ 
{
    vector<string> running;
    int loc=-1;
    for (python_proc *script: available_scripts)
    {
        if (script->is_child_running())
        {
            running.push_back(script->get_script_name());
        }
        else 
        {
            loc = get_loc(script->get_script_name());
            if (loc<0){continue;}
            if (running_scripts[loc])
            {
                    running_scripts[loc]=false;
                    finished_scripts[loc]=true;
            }
        }
    }
    return running;
}
bool py_manager::is_running(string py_file)
/*
 * Description
 * -----------
 *      Probe directly the py_proc with name py_file
 *      if it is still running. 
 * Return
 * ------
 *      True -> script is still running, False -> 
 *      script is not running.
 */ 
{

    for (auto script:available_scripts)
    {
        if (strcmp(script->get_script_name().c_str(),py_file.c_str())==0)
        {
            return script->is_child_running();
        }
    }
    return false;
}
vector<string> py_manager::get_available_scripts()
/*
 * Description
 * -----------
 *      Create a list of all the managed script added
 *      by add_python_script call.
 * Return 
 * ------
 *      Return a list of all the managed script vector<string>
 */ 
{
    vector<string> script_name;
    for (auto script:available_scripts)
    {
        script_name.push_back(script->get_script_name());
        }
    return script_name;
}

int py_manager::get_stdout(string py_file,string *stdout)
/*  
 * Description
 * -----------
 *      Retrieve the stdout value of the registered script. 
 *      You can call this method even if the script as not
 *      been runned, but it make no sense to do so!
 * Return 
 * ------
 *      -2 -> child is currently running
 *      -1 -> there is no script registered
 *      0  -> *buffer as been set to the stdout
 */ 
{
    int loc = get_loc(py_file);
    if (loc<0)
    {//script don't exist
        return -1;
    }
    python_proc *script = available_scripts[loc];
    if (script->is_child_running())
    {//child is currently running
        return -2;
    }
    whos_running();//make sure we have the last information about the scripts
    if (finished_scripts[loc])//make sure the script as been run
    {
        *stdout = script->get_stdout();
        return 0;
    }
    return -1;//the script probably never been runned

}

int py_manager::get_stderr(string py_file,string *stderr)
/*  
 * Description
 * -----------
 *      Retrieve the stderr value of the registered script. 
 *      You can call this method even if the script as not
 *      been runned, but it make no sense to do so!
 * Return 
 * ------
 *      -2 -> child is currently running
 *      -1 -> there is no script registered
 *      0  -> *buffer as been set to the stdout
 */
{
    int loc = get_loc(py_file);
    if (loc<0)
    {//script don't exist
        return -1;
    }
    python_proc *script = available_scripts[loc];
    if (script->is_child_running())
    {//child is currently running
        return -2;
    }
    whos_running();//make sure we have the last information about the scripts
    if (finished_scripts[loc])//make sure the script as been run
    {
        *stderr = script->get_stderr();
        return 0;
    }
    return -1;//the script probably never been runned

}


int py_manager::get_return_code(string py_file,int *ret)
/*  
 * Description
 * -----------
 *      Retrieve the return code value of the registered 
 *      script. You can call this method even if the script 
 *      as not been runned, but it make no sense to do so!
 * 
 * Return 
 * ------
 *      -2 -> child is currently running
 *      -1 -> there is no script registered
 *      0  -> *buffer as been set to the stdout
 */ 
{
    for (python_proc *script: available_scripts)
    {
        if (strcmp(py_file.c_str(),script->get_script_name().c_str())==0)
        {
            if (script->is_child_running())
            {
                return -2;
            }
            *ret = script->get_return_value();
            return 0;
        }
    }
    return -1;
}



int py_manager::add_python_script(string py_file)
/*
 * Description
 * -----------
 *      Add a python script to the manager. 
 * Return 
 * ------- 
 *      -1 -> if script is already registered.
 *       0 -> successfully registered the script.
 */
{
    //check if script is registered. py_file and get_script_name
    //should be fullpath+script.py string.
    for (python_proc *script: available_scripts)
    {
        if (strcmp(py_file.c_str(),script->get_script_name().c_str())==0)
        {
            return -1;
        }
    }
    python_proc *py = new python_proc(interpreter,py_file);
    //construct the table of status
    available_scripts.push_back(py);
    running_scripts.push_back(false);//initialize the running flags
    finished_scripts.push_back(false);//initialize the finished flags
    return 0;
}