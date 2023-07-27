#include "ui_config_file.h"

int ui_get_string(string c_file,string entry,string *value)
{
    ifstream config_file;
    string line_buffer="";
    string value_buffer="";
    string line_substr="";
    config_file.open(c_file);
    if (!config_file.is_open())
    {
        cout<<"Configuration file not found."<<endl;
        return -1;
    }
    //loop through all the config file lines
    while(getline(config_file,line_buffer))
    {
        if (ui_conf::get_var(line_buffer).compare(entry)==0)
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
int ui_get_bool(string c_file,string entry,bool *value)
{
    ifstream config_file;
    string line_buffer="";
    string value_buffer="";
    string line_substr="";
    config_file.open(c_file);
    if (!config_file.is_open())
    {
        cout<<"Configuration file not found."<<endl;
        return -1;
    }
    //loop through all the config file lines
    while(getline(config_file,line_buffer))
    {
        if (ui_conf::get_var(line_buffer).compare(entry)==0)
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
				if (value_buffer.compare("T")==0)
				{
					*value = true;
					return 0;
				}
                else if (value_buffer.compare("F")==0)
				{
					*value = false;
					return 0;
				}
				else {return -1;}
                
            }
        }
        
    }
    
    return -1;
    
}
int ui_get_int(string c_file,string entry,int *value)
{
    ifstream config_file;
    string line_buffer="";
    string value_buffer="";
    string line_substr="";
    config_file.open(c_file);
    if (!config_file.is_open())
    {
        cout<<"Configuration file not found."<<endl;
        return -1;
    }
    //loop through all the config file lines
    while(getline(config_file,line_buffer))
    {
        if (ui_conf::get_var(line_buffer).compare(entry)==0)
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
                if (ui_conf::is_int(value_buffer)){
                   *value = stoi(value_buffer.c_str());
                return 0; }
                else {return -1;}
                
            }
        }
        
    }
    
    return -1;
    
}
int ui_get_double(string c_file,string entry,double *value)
{
    ifstream config_file;
    string line_buffer="";
    string value_buffer="";
    string line_substr="";
    config_file.open(c_file);
	
    if (!config_file.is_open())
    {
        cout<<"Configuration file not found."<<endl;
        return -1;
    }
    //loop through all the config file lines
    while(getline(config_file,line_buffer))
    {
        if (ui_conf::get_var(line_buffer).compare(entry)==0)
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
                if (ui_conf::is_double(value_buffer)){
                   *value = stod(value_buffer.c_str());
                return 0; }
                else {return -1;}
                
            }
        }
        
    }
    return -1;
}

bool ui_conf::is_int(string str)
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
        if (!isdigit(c) && c!='-') 
        {
            return false;
        }
    }
    return true;
}
bool ui_conf::is_double(string str)
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
        if (!isdigit(c) && c!='.' && c!='-' ) 
        {
            return false;
        }
    }
    return true;
}

string ui_conf::get_var(string line)
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