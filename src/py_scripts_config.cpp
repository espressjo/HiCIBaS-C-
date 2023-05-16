#include "py_scripts_config.h"

bool file_exist (std::string name) {
/*
 * Check if file exist.
 */ 
    std::ifstream f(name.c_str());
    return f.good();
}
std::vector<std::string> split(std::string command,const char sep)
/*
 * Split a c++ string into an string array. Separator <sep>
 * is used to do the splitting. 
 */
{
	std::vector<std::string> buffer{};
	std::string buff{};
	for (auto c : command)
    {
		if (c != sep)
				buff += c;
		else
		{
			buffer.push_back(buff);
			buff.erase();
		}
    }
    if (buff.size() != 0)
    buffer.push_back(buff);
    return buffer;
}
bool isNumber(std::string s)
/*
 * Check is string is a number.
 */ 
{
    for (auto &c : s) {
        if (isdigit(c) == 0)
            return false;
    }
    return true;
}
uint32_t encode_scripts(std::vector<std::string> scripts,std::map<std::string,int> *myMap)
/*
 * Description
 * -----------
 * 	Utility function to convert a list of scripts mapped with
 *  IDs to a 32-bits unsigned int. 
 */ 
{
	uint32_t e_scripts = 0;
	int id=0;
	for (auto _S:scripts)
	{
		id = (*myMap)[_S];
		if (id!=0)
		{
			e_scripts+=pow(2,id-1);
		}
	}
	return e_scripts;
}


void decode_scripts(uint32_t e_scripts,std::vector<std::string> *scripts,std::map<std::string,int> myMap)
/*
 * Description
 * -----------
 * 		Utility function to convert an encoded script uint32 
 * 		into a list of script name. Used to retrive the list 
 *		running or stopped script. 
 * 
 *  Parameters
 *  ----------
 * 		e_scripts	encoded scripts
 * 		*scripts	vector of script
 * 		*myMap		the map generated from config file.
 */ 
{
	
	int b_id=0,id=0;
	std::string _script="";
	scripts->clear();
	
	for (auto i:myMap)
	{
		id = i.second;
		_script = i.first;
		b_id = pow(2,id-1);
		if ((e_scripts&b_id)==b_id)
		{
			scripts->push_back(_script);
		}
	}
	
	return;
}

int getConf(std::string fname,std::map<std::string,int> *myMap)
/*
 * Description
 * -----------
 * 	Will read the configuration file (/opt/HiCIBaS/config/scripts.txt)
 * 	and store the information inside a map<string,int>. The configuration
 * 	file should have the following structure;
 *		<id>\t<script path>
 * 		----------------------------------
 * 		1	/opt/HiCIBaS/scripts/test.py
 * 		2	/opt/HiCIBaS/scripts/test2.py
 * 		[...]
 * 	The map will allow to get the script ID number for each script
 * 	used in py_manager.
 */ 
{
	std::vector<std::string> vec;
	std::string line="",line2="";
	
	myMap->clear();
	if (!file_exist(fname))
	{
		return -1;
	}
	std::ifstream infile(fname);
	while (std::getline(infile, line))
	{
		std::regex pattern("\\s+");
		line2 = std::regex_replace(line, pattern, "\t");
		std::string buff="";
		vec = split(line2,'\t');
		
		if (vec.size()!=2)
		{
			continue;
		}
		if (!isNumber(vec[0])){continue;}
		if (vec[0].substr(0,1).compare("0")==0)
		{
			continue;
		}
		else{
		myMap->insert( std::make_pair( vec[1].c_str(), std::atoi(vec[0].c_str()) ));
		}
		// process pair (a,b)
	}
	return 0;
}
int getInterpreter(std::string fname,std::string *interpreter)
/*
 * Description
 * -----------
 * 	Will try to find the interpreter (index=0) in the config 
 * file
 *		<id>\t<script path>
 * 		----------------------------------
 * 		0	/path/to/interpreter
 * 		1	/opt/HiCIBaS/scripts/test.py
 * 		2	/opt/HiCIBaS/scripts/test2.py
 * 		[...]
 * 	The map will allow to get the script ID number for each script
 * 	used in py_manager.
 */ 
{
	std::vector<std::string> vec;
	std::string line="",line2="";
	//std::map<std::string,int> *myMap
	//myMap->clear();
	if (!file_exist(fname))
	{
		return -1;
	}
	std::ifstream infile(fname);
	while (std::getline(infile, line))
	{
		std::regex pattern("\\s+");
		line2 = std::regex_replace(line, pattern, "\t");
		std::string buff="";
		vec = split(line2,'\t');
		
		if (vec.size()!=2)
		{
			continue;
		}
		if (!isNumber(vec[0])){continue;}
		if (vec[0].substr(0,1).compare("0")==0)
		{
			*interpreter = vec[1];
			return 0;
		}
		//myMap->insert( std::make_pair( vec[1].c_str(), std::atoi(vec[0].c_str()) ));
		
		// process pair (a,b)
	}
	return -1;
}
std::vector<std::string> get_scripts(std::string fname)
/*
 * Description
 * -----------
 * 	Read the scripts config file given, and return
 *  a vector containing all the scripts.
 */ 
{
	std::vector<std::string> vec;
	std::string line="",line2="";
	std::vector<std::string> scripts;
	if (!file_exist(fname))
	{
		return scripts;
	}
	std::ifstream infile(fname);
	while (std::getline(infile, line))
	{
		std::regex pattern("\\s+");
		line2 = std::regex_replace(line, pattern, "\t");
		std::string buff="";
		vec = split(line2,'\t');
		
		if (vec.size()!=2)
		{
			continue;
		}
		if (!isNumber(vec[0])){continue;}
		if ( vec[0].substr(0,1).compare("0")==0 )
		{
			continue;
		}
		
		scripts.push_back(vec[1]);
		
		// process pair (a,b)
	}
	return scripts;
	
	
}
