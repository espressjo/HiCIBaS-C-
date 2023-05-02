#include "config_file.h"

void init_handler(string c_file,instHandle *handler)
{
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    //:: Initialize all the variables then lookup the config file ::
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    std::string sfile="";
    //the path elements
    handler->path.log="";
    handler->path.config="";
    handler->path.data="";
	handler->py_config_file = new map<std::string,int>;
	handler->scripts_fname = "";
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    //:: now let set some of the hanlder members from the config file ::
    //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
    if (get_entry(c_file,"LOG",&handler->path.log)!=0)
        {cout<<"Unable to set log path from config file"<<endl;}
	if (get_entry(c_file,"SCRIPTS",&sfile)!=0)
        {cout<<"script file not defined."<<endl;}
	handler->scripts_fname = sfile;
    if (sfile.compare("")!=0)
	{
		if (getConf(sfile,handler->py_config_file)!=0)
		{
			cout<<"Not able to parse script file."<<endl;
		}
	}
}
string get_var(string line)
/*
 * Description
 * -----------
 *      This will function will return the 1st word
 *      of a string sentence given that the sentence
 *      does not start by a space or tab character.
 * Return 
 * ------
 *      Word or an empty string.  
 */ 
{
    string buff="";
    for (auto &c:line)
    {
        if (c==' ' || c=='\t')
        {
            break;    
        }     
    buff+=c;
    }    
    return buff;
}
int get_entry(string c_file,string entry,string *value)
{
    ifstream config_file;
    string line_buffer="";
    string value_buffer="";
    string line_substr="";
    config_file.open(c_file);
    if (!config_file.is_open())
    {
        cout<<"Configuration file not found, but get_entry is called!!! Abort"<<endl;
        exit(0);
    }
    //loop through all the config file lines
    while(getline(config_file,line_buffer))
    {
        if (get_var(line_buffer).compare(entry)==0)
        {
                //return whatever is after entry and is not a comment.
                //...
            if (line_buffer.find(entry)!=string::npos)
            {
                line_substr = line_buffer.substr(line_buffer.find(entry)+entry.length(),line_buffer.size());  
                for (auto &c:line_substr)
                {
                    if (c!='\t' && c!=' ' && c!='#')
                    {
                       value_buffer+=c; 
                    }
                    if (c=='#'){break;}
                }
                *value = value_buffer;
                return 0;
            }
        }
        
    }
    
    return -1;
    
}
int get_entry(string c_file,string entry,int *value)
{
    ifstream config_file;
    string line_buffer="";
    string value_buffer="";
    string line_substr="";
    config_file.open(c_file);
    if (!config_file.is_open())
    {
        cout<<"Configuration file not found, but get_entry is called!!! Abort"<<endl;
        exit(0);
    }
    //loop through all the config file lines
    while(getline(config_file,line_buffer))
    {
        if (get_var(line_buffer).compare(entry)==0)
        {
                //return whatever is after entry and is not a comment.
                //...
            if (line_buffer.find(entry)!=string::npos)
            {
                line_substr = line_buffer.substr(line_buffer.find(entry)+entry.length(),line_buffer.size());  
                for (auto &c:line_substr)
                {
                    if (c!='\t' && c!=' ' && c!='#')
                    {
                       value_buffer+=c; 
                    }
                    if (c=='#'){break;}
                }
                if (is_int(value_buffer)){
                   *value = stoi(value_buffer.c_str());
                return 0; }
                else {return -1;}
                
            }
        }
        
    }
    
    return -1;
    
}
int get_entry(string c_file,string entry,double *value)
{
    ifstream config_file;
    string line_buffer="";
    string value_buffer="";
    string line_substr="";
    config_file.open(c_file);
    if (!config_file.is_open())
    {
        cout<<"Configuration file not found, but get_entry is called!!! Abort"<<endl;
        exit(0);
    }
    //loop through all the config file lines
    while(getline(config_file,line_buffer))
    {
        if (get_var(line_buffer).compare(entry)==0)
        {
                //return whatever is after entry and is not a comment.
                //...
            if (line_buffer.find(entry)!=string::npos)
            {
                line_substr = line_buffer.substr(line_buffer.find(entry)+entry.length(),line_buffer.size());  
                for (auto &c:line_substr)
                {
                    if (c!='\t' && c!=' ' && c!='#')
                    {
                       value_buffer+=c; 
                    }
                    if (c=='#'){break;}
                }
                if (is_double(value_buffer)){
                   *value = stod(value_buffer.c_str());
                return 0; }
                else {return -1;}
                
            }
        }
        
    }
    
    return -1;
    
}
bool is_int(string str)
/*
 * Description
 * -----------
 *      Check if a string is a integer. 
 * Return 
 * ------
 *      True/False
 */ 
{

    
    for (auto &c:str)
    {   
        if (!isdigit(c) ) 
        {
            return false;
        }
    }
    return true;
}
bool is_double(string str)
/*
 * Description
 * -----------
 *      Check if a string is a double. 
 * Return 
 * ------
 *      True/False
 */ 
{

    
    for (auto &c:str)
    {   
        if (!isdigit(c) ) 
        {
            if (c!='.'){
            return false;}
        }
    }
    return true;
}
