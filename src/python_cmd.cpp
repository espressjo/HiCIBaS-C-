#include "python_cmd.h"

void python_cmd(instHandle *handle,cmd *cc)
{//test fonction
    string script="",stdout="",stderr="";
	vector<string> args;
    //1st get the script
    if ((*cc)["script"].compare("")!=0)
    {
        script = (*cc)["script"];
        
    }
	if ((*cc)["arg1"].compare("")!=0)
	{
		args.push_back((*cc)["arg1"]);
	}
	if ((*cc)["arg2"].compare("")!=0)
	{
		args.push_back((*cc)["arg2"]);
	}
	if ((*cc)["arg3"].compare("")!=0)
	{
		args.push_back((*cc)["arg3"]);
	}
	if ((*cc)["arg4"].compare("")!=0)
	{
		args.push_back((*cc)["arg4"]);
	}
	if ((*cc)["arg5"].compare("")!=0)
	{
		args.push_back((*cc)["arg5"]);
	}
	if ((*cc)["arg6"].compare("")!=0)
	{
		args.push_back((*cc)["arg6"]);
	}
    //snd stdout
    if ((*cc)["-stdout"].compare("")!=0)
    {
        if (handle->py->get_stdout(script,&stdout)!=0)
        {
			cc->respond("Cannot read stdout now.",uicsCMD_ERR_VALUE);
            return ;
        }
        else {
			cc->respond(stdout);
            return;
        }
    }
    //snd stderr
    if ((*cc)["-stderr"].compare("")!=0)
    {
        if (handle->py->get_stderr(script,&stderr)!=0)
        {
            cc->respond("Cannot read stderr now.",uicsCMD_ERR_VALUE);
            return ;
        }
        else {
            cc->respond(stderr);
            return ;
        }
    }
    //snd stderr no \n
    if ((*cc)["-stderr_no_n"].compare("")!=0)
    {
        if (handle->py->get_stderr(script,&stderr)!=0)
        {
            cc->respond("Cannot read stderr now.",uicsCMD_ERR_VALUE);
            return ;
        }
        else {
            string n_stderr="";
            for (auto &c:stderr){
                if (c=='\n')
                {n_stderr+=';';continue;}
                n_stderr+=c;
            }
            n_stderr = n_stderr.substr(0,n_stderr.length()-1)+"\n";
            cc->respond(n_stderr);
            return ;
        }
    }
    //snd stderr no \n
    if ((*cc)["-stdout_no_n"].compare("")!=0)
    {
        if (handle->py->get_stdout(script,&stdout)!=0)
        {
            cc->respond("Cannot read stderr now.",uicsCMD_ERR_VALUE);
            return ;
        }
        else {
            string n_stdout="";
            //replace all \n by ;
            for (auto &c:stdout){
                if (c=='\n')
                {n_stdout+=';';}
                else {
                n_stdout+=c;}
            }
            n_stdout = n_stdout.substr(0,n_stdout.length()-1)+"\n";
            cc->respond(n_stdout);
            return ;
        }
    }
    
    if ((*cc)["-run"].compare("")!=0)
    {	
        if (args.size()==0){
			if (handle->py->run(script)!=0)
			{
				cc->respond("Unable to start script",uicsCMD_ERR_PARAM_VALUE);
				return;
            }
            else {
                
                cc->respond();
                return;
            }
		}
		else {
			if (handle->py->run(script,args)!=0)
			{
				cc->respond("Unable to start script",uicsCMD_ERR_PARAM_VALUE);
				return;}
			else {
                cc->respond();
                return;}
			
			}
	}
    
    if ((*cc)["-is_runing"].compare("")!=0)
    {
        if (handle->py->is_running(script))
        {
            cc->respond("T");
            return;}
        else {
            cc->respond("F");
        return;}
    }
    if ((*cc)["-get_avail_script"].compare("")!=0)
    {string scripts="";
        for (auto script: handle->py->get_available_scripts())
            {
                scripts+=script+";";
            }
        scripts = scripts.substr(0,scripts.length()-1);
        cc->respond(scripts);
        return;
    }
    if ((*cc)["-whos_running"].compare("")!=0)
    {   string scripts="";
        for (auto script: handle->py->whos_running())
        {
            scripts+=script+";";
        }
        scripts = scripts.substr(0,scripts.length()-1);
        cc->respond(scripts);
        return;
    }
    if ((*cc)["-whos_stopped"].compare("")!=0)
    {   string scripts="";
        for (auto script: handle->py->whos_finished())
        {
            scripts+=script+";";
        }
        scripts = scripts.substr(0,scripts.length()-1);
        cc->respond(scripts);
        return;
    }
    if ((*cc)["-pid"].compare("")!=0)
    {   
        int pid=0;
        handle->py->get_pid(script,&pid);
        cc->respond(std::to_string(pid));
        return;
    }
    if ((*cc)["-kill"].compare("")!=0)
    {   
        int ret=0;
        ret = handle->py->kill_script(script);
        cc->respond(std::to_string(ret));
        return;
    }
	
    //technically we should not reach this line
    cc->respond("Missing arguments",uicsCMD_ERR_PARAM_FORMAT);
    return ;
}