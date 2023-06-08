#include "uics_cmds.h"
#include <algorithm>

//============================//
//          CMD LIST          //
//============================//

cmdList::cmdList(std::string configPath)
{
    if (configPath == "")
    {
        std::cout << "No config path. CMD list is empty" << std::endl;
        log_ll_cmd("No config path. CMD list is empty");
        return;
    }

    if (readConfig(configPath) == -1)
    {
        std::cout << "Formatting error in config file." << std::endl;
        log_ll_cmd("Formatting error in config file.");
        cmd_list = std::vector<cmd>();
        return;
    }
    return;
}

int cmdList::readConfig(std::string configPath)
{
    std::vector<std::string> rawLines{};
    std::vector<std::vector<std::string>> rawChunks{};
    std::vector<std::vector<std::string>> cmdChunks{};

    // Extraction of lines from config file
    {
        std::fstream file(configPath);
        std::string buffer;
        while (getline(file, buffer))
        {
            rawLines.push_back(buffer);
        }
        file.close();
    }

    // Chunking sets of lines based on ":::" separator
    {
        std::vector<std::string> buffer;
        for (auto line : rawLines)
        {
            if (isEmpty(line))
            {
                continue;
            }
            else if (isSeparator(line) && buffer.size() == 0)
            {
                continue;
            }
            else if (isSeparator(line))
            {
                rawChunks.push_back(buffer);
                buffer = std::vector<std::string>{};
            }
            else
            {
                buffer.push_back(line);
            }
        }
        if (buffer.size() != 0)
            rawChunks.push_back(buffer);
    }

    // Splitting chunks into individual words
    {
        std::vector<std::string> buffer;
        for (auto vec : rawChunks)
        {
            for (auto line : vec)
            {
                std::string buff;
                for (auto c : line)
                {
                    if (c == '#')
                        break;
                    if (c!=' ' && c!='\n' && c!='\r' && c!='\t' && c!= '\0')
                        buff+=c;
                    else if (buff.size() != 0)
                    {
                        buffer.push_back(buff);
                        buff.erase();
                    }
                }
                if (buff.size() != 0)
                    buffer.push_back(buff);
            }
            cmdChunks.push_back(buffer);
            buffer = std::vector<std::string>{};
        }
    }

    // Add commands to cmd_list
    if (populateList(cmdChunks) == -1)
        return -1;

    return 0;
}

int cmdList::populateList(std::vector<std::vector<std::string>> cmdVec)
{
    for (auto vec : cmdVec)
    {
        int readStatus = READ_START;
        cmd buffer{};
        for (auto str : vec)
        {
            switch (readStatus)
            {
            case (READ_START):
                if (str == "CMD")
                {
                    readStatus = READ_CMD;
                }
                else
                {
                    std::cout << "Error in formatting of CMD line in config file" << std::endl;
                    log_ll_cmd("Error in formatting of CMD line in config file");
                    return -1;
                }
                break;
            case (READ_CMD):
                buffer.name = str;
                readStatus = READ_STATE;
                break;
            case (READ_STATE):
                if (str == "acq")
                    buffer.state = ACQ;
                else if (str == "idle")
                    buffer.state = IDLE;
                else if (str == "init")
                    buffer.state = INIT;
                else if (str == "config")
                    buffer.state = CONFIG;
                else if (str.substr(0, 6).compare("thread") == 0)
                {
                    std::string port_buff{};
                    //++buffer.nbArg;
                    readStatus = READ_ARG;
                    //buffer.range.push_back("inf");
                    buffer.state = THREAD;
                    for (auto c : str)
                    {
                        if (isdigit(c))
                            port_buff += c;
                    }
                    if (port_buff.compare("") != 0){
                        buffer.port = std::atoi(port_buff.c_str());}

                }
                else
                {
                    std::cout << "Error in formatting of STATE in config file" << std::endl;
                    log_ll_cmd("Error in formatting of STATE in config file");
                    return -1;
                }
                readStatus = READ_ARG_START;
                break;
            case (READ_ARG_START):
                if (str == "ARG")
                {
                    readStatus = READ_ARG;
                    ++buffer.nbArg;
                }
                else
                {
                    std::cout << "Error in formatting of ARG line config file" << std::endl;
                    log_ll_cmd("Error in formatting of ARG line config file");
                    return -1;
                }
                break;
            case (READ_ARG):
                buffer.args.push_back(str);
                readStatus = READ_OPT;
                break;
            case (READ_OPT):
                if (str == "optional")
                {
                    buffer.opt.push_back(true);
                    readStatus = READ_TYPE;
                    break;
                }

                else if (str == "int")
                {
                    buffer.opt.push_back(false);
                    buffer.type.push_back(INT);
                    readStatus = READ_RANGE;
                }
                else if (str == "float")
                {
                    buffer.opt.push_back(false);
                    buffer.type.push_back(FLOAT);
                    readStatus = READ_RANGE;
                }
                else if (str == "double")
                {
                    buffer.opt.push_back(false);
                    buffer.type.push_back(DOUBLE);
                    readStatus = READ_RANGE;
                }
                else if (str == "string")
                {
                    buffer.opt.push_back(false);
                    buffer.type.push_back(STRING);
                    readStatus = READ_RANGE;
                }
                else
                {
                    std::cout << "Error in formatting of OPT in config file" << std::endl;
                    log_ll_cmd("Error in formatting of OPT in config file");
                    return -1;
                }
                break;
            case (READ_TYPE):
                if (str == "int")
                    buffer.type.push_back(INT);
                else if (str == "float")
                    buffer.type.push_back(FLOAT);
                else if (str == "double")
                    buffer.type.push_back(DOUBLE);
                else if (str == "string")
                    buffer.type.push_back(STRING);
                else
                {
                    std::cout << "Error in formatting of TYPE in config file" << std::endl;
                    log_ll_cmd("Error in formatting of TYPE in config file");
                    return -1;
                }
                readStatus = READ_RANGE;
                break;
            case (READ_RANGE):
                if (str == "ARG")
                {
                    ++buffer.nbArg;
                    readStatus = READ_ARG;
                    buffer.range.push_back("inf");
                }
                else if (str.find_first_of('[') != std::string::npos && str.find_first_of(']') != std::string::npos)
                {
                    buffer.range.push_back(str);
                    readStatus = READ_ARG_START;
                }
                else
                {
                    std::cout << "Error in formatting of RANGE in config file" << std::endl;
                    log_ll_cmd("Error in formatting of RANGE in config file");
                    return -1;
                }
                break;
            }
        }
        if (buffer.args.size() > buffer.range.size())
            buffer.range.push_back("inf");
        cmd_list.push_back(buffer);
        buffer = cmd{};
    }
    return 0;
}

bool cmdList::isSeparator(std::string line)
{
    std::string buff="";

    if (line[0]=='#')
    {
        return true;
    }

    if (line.size()>3)
    {
        for (int i=0;i<3;i++) {

            if (line[i]==':')
            {
                buff+=line[i];
            }
        }
    }
    else {
        return false;
    }

    if (buff.compare(":::")==0)
    {
        return true;
    }
    return false;
}

bool cmdList::isEmpty(std::string line)
{
    if (line.find_first_not_of(" \t\n\v\f\r") != std::string::npos)
        return false;
    else
        return true;
}
std::string cmdList::getCMDlist()
/*
 * Used to send to client a list of all available cmd
 */
{
    std::string list="";
    for (auto v: cmd_list)
    {
        list+=v.name+"\n";
    }
    return list;
}
std::string cmdList::getcmdinfo(std::string cmdname)
{
    std::string buffer="";
    std::string state="",optional="";
    if (cmdname.size()<1)
    {
        return "information not found";
    }
    if (cmdname[0]=='?')
    {
        cmdname.replace(0,1,"");
    }
    for (unsigned int ii{0}; ii < cmd_list.size(); ++ii)
    {
        if (cmd_list[ii].name.compare(cmdname) == 0)
        {//ACQ=0,IDLE,CONFIG,INIT,THREAD
            if (cmd_list[ii].state==0)
            {
                state="acquisition";
            }
            else if (cmd_list[ii].state==1) {
                state="idle";
            }
            else if (cmd_list[ii].state==2) {
                state="configuration";
            }
            else if (cmd_list[ii].state==3) {
                state="init";
            }
            else if (cmd_list[ii].state==4) {
                state="thread";
            }

            buffer+="\n-------------------------\n";
            buffer+="CMD name: "+cmdname+"\n";
            buffer+="State: "+state+"\n";
            buffer+="-------------------------\n";
            buffer+="Arg:\trange:\topt.:\n";

            for (int i=0;i<cmd_list[ii].args.size();i++) {
                if (cmd_list[ii].opt[i])
                {
                    optional="optional";
                }
                else {
                    optional="";
                }
                buffer+=cmd_list[ii].args[i]+"\t"+cmd_list[ii].range[i]+"\t"+optional+"\n";
            }
            //buffer+="\n";
            return buffer;
        }


    }
    return "information not found";

}
void cmdList::printAllCMD()
{
    for (auto v: cmd_list)
    {
        std::cout<<":::::::::::::"<<std::endl;
        std::cout<<"cmd: "<<v.name<<", STATE: "<<v.state<<std::endl;
        for (unsigned int i=0;i<v.args.size();++i) {
            std::cout<<"arg: "<<v.args[i]<<", type: "<<v.type[i]<<", range: "<<v.range[i];
            if (v.opt[i])
                std::cout << " OPTIONAL";
            std::cout << std::endl;
        }
    }
    std::cout << ":::::::::::::" << std::endl;
}

void cmdList::printCMD(std::string name)
{
    for (auto v: cmd_list)
    {
        if (v.name == name)
        {
            std::cout<<":::::::::::::"<<std::endl;
            std::cout<<"cmd: "<<v.name<<", STATE: "<<v.state<<std::endl;
            for (int i=0;i<v.nbArg;++i)
            {
                std::cout<<"arg: "<<v.args[i]<<", type: "<<v.type[i]<<", range: "<<v.range[i]<<std::endl;
            }
            std::cout << ":::::::::::::" << std::endl;
            return;
        }
    }
    {
        std::cout << ":::::::::::::" << std::endl;
        std::cout << "This command does not exist." << std::endl;
        std::cout << ":::::::::::::" << std::endl;
    }
}

cmd cmdList::operator[] (std::string str)
{
    for (unsigned int ii{0}; ii < cmd_list.size(); ++ii)
    {
        if (cmd_list[ii].name == str)
            return cmd_list[ii];
    }
    std::cout << "Command does not exist." << std::endl;
    log_ll_cmd("Command does not exist.");
            return cmd();

}

//============================//
//            CMD             //
//============================//
cmd::cmd()
{
    sockfd = -0;
    port = 0;
    // Main command variables
    name="";
    nbArg=0;
	udp_e_length= 0;
    args.clear();
    argsVal.clear();
    type.clear();
    opt.clear();
    range.clear();
	UDP="";
	udp=false;
	//init datagram structure
	server_addr=NULL;
	
}
void cmd::set_datagram(struct sockaddr_in client){
	//size_t islb64EncodeAlloc(const char *in, size_t inlen, char **out);
	char *encodedStructure=NULL;
	udp_e_length = islb64EncodeAlloc((const char *)&client,sizeof(client),&encodedStructure);
	UDP = std::string(encodedStructure);
	std::cout<<"[sending ... ] "<<encodedStructure<<std::endl;
	//:::::::::::::::::::
	//:::   testing   :::
	//:::::::::::::::::::
	struct sockaddr_in *client2;
	size_t len=0;
	islb64DecodeAlloc(encodedStructure,udp_e_length,( char **)&client2,&len);
	std::cout<<"[out length] "<<udp_e_length<<std::endl;
	std::cout<<"[length] "<<len<<std::endl;
	std::string ip = "";
	getIP(client,&ip);
	std::cout<<"[set datagram client 1] "<<ip<<std::endl;
	
	getIP(*client2,&ip);
	std::cout<<"[set datagram client 2] "<<ip<<std::endl;
	//::::::::::::::::::::::::::::::::::::::::::
	return ;
}
int cmd::getIP(struct sockaddr_in sa,std::string *IP){//inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),s, maxlen);
/*
 * For troubleshooting purpose
 * 
 */ 
	
	char s[1024];
	memset(s,0,1024);
	inet_ntop(AF_INET, &((&sa)->sin_addr),s, (socklen_t)1024);
	*IP = std::string(s);
	return 0;
}
STATE cmd::to_state(std::string state)
{
	switch(std::atoi(state.c_str())) {
	  case 0:{
		return ACQ;
	  break;}
	  case 1:{
		return IDLE;
	  break;}
	  case 2:{
		return CONFIG;
	  break;}
	  case 3:{
		return INIT;
	  break;}
	  case 4:{
		return THREAD;
	  break;}
}
return UNKNOWN_STATE;	
}
TYPE cmd::to_type(std::string type)
{//enum TYPE {INT, FLOAT, DOUBLE, STRING};
	switch(std::atoi(type.c_str())) {
	  case 0:{
		return INT;
	  break;}
	  case 1:{
		return FLOAT;
	  break;}
	  case 2:{
		return DOUBLE;
	  break;}
	  case 3:{
		return STRING;
	  break;}
}
return UNKNOWN_TYPE;	
}
std::string cmd::serializeCMD()
/*
 * Description
 * -----------
 * 	Will generate a string with all the command
 *  elements (name, sockfd, argument <range>, <type>
 *  <arg name>, state, UDP parameters
 * 
 * NOTE
 * ----
 * 	SOCKFD:sockfd;CMD:myname;STATE:state;ARG1:name-type-range;ARG2:name-value-type-opt-range;UDP:false
 *  (1) Argument will have an increment (ARG1, ARG2, ...) 
 *  (2) the argument paramters are separated by <~>
 *  (3) THhe command paramters are separated by <;>
 *  (4) The key-values are separated by <:>
 * 
 */ 
{
    std::string output{};
	//1st we create the head of the string <sockfd>:<cmd_name>:<cmd_state>
	output+="SOCKFD:"+std::to_string(sockfd)+";";
	output+= "CMD:"+name+";";
	output+= "STATE:"+std::to_string(state)+";";
	int ii=1;
	for (int i=0;i<args.size();i++)
	{
		if (argsVal[i].compare("")!=0){
		output+="ARG"+std::to_string(ii)+":"+args[i]+"~"+argsVal[i]+"~"+std::to_string(type[i])+"~"+std::to_string(opt[i])+"~"+range[i]+";";
		ii++;
		}
	}
	output+="UDP:"+std::to_string(udp)+";";
	output+="UDPELENGTH:"+std::to_string(udp_e_length)+";";
	output+="UDPSTRUCT:"+UDP;
	std::cout<<"output: "<<output<<std::endl;
    return output;
}

void cmd::deserializeCMD(std::string command)
{
	resetCmd();//clear everything in the cmd vectors
    hashT H(command,';',':');
	
	sockfd = std::atoi(H["SOCKFD"].c_str());
	name = H["CMD"];
	state = to_state(H["STATE"]);
	int i=1;
	std::string _arg="";
	std::vector<std::string> buffer{};
	nbArg=0;
	while(1)
	{
		_arg = "ARG"+std::to_string(i);

		if (H[_arg]==""){break;}
		
		buffer = split(H[_arg],'~');
		if (buffer.size()!=5){std::cout<<"major problem!!!!"<<std::endl;continue;}
		args.push_back(buffer[0]);
		argsVal.push_back(buffer[1]);
		type.push_back(to_type(buffer[2]));
		opt.push_back( ( (buffer[3].compare("1")==0) ? true : false  ));
		range.push_back(buffer[4]);
		i++;
		nbArg+=1;		
	}
	if (H["UDP"].compare("1")==0)
	{
		udp = true;
		UDP = H["UDPSTRUCT"];
		udp_e_length = (size_t)std::atoi(H["UDPELENGTH"].c_str());
		size_t out=0;
		islb64DecodeAlloc(H["UDPSTRUCT"].c_str(), udp_e_length,(char **)&server_addr, &out);
		std::string ip="";
		getIP(*server_addr,&ip);
	}
	else
	{
		udp = false;
	}
	

}

std::vector<std::string> cmd::split(std::string command,const char sep)
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

void cmd::respond(){
	if (udp){
		
		sndMsg(sockfd,server_addr);
	}
	else{
	sndMsg(sockfd);}
	return ;
}
void cmd::respond(std::string msg){
	if (udp){sndMsg(sockfd,msg,server_addr);}
	else{sndMsg(sockfd,msg);}
	return ;
}
void cmd::respond(std::string msg,int errCode){
	if (udp){sndMsg(sockfd,msg,errCode,server_addr);}
	else{sndMsg(sockfd,msg,errCode);}
	return ;
}
void cmd::respond(const char msg[]){
	if (udp){sndMsg(sockfd,msg,server_addr);}
	else{sndMsg(sockfd,msg);}
	return;
}
void cmd::respond(const char msg[],int errCode){
	if (udp){sndMsg(sockfd,msg,errCode,server_addr);}
	else{sndMsg(sockfd,msg,errCode);}
	return;
}


int cmd::resetCmd()
{
    name = std::string{};
    nbArg = int{};
    state = STATE{};
    args = std::vector<std::string>{};
    argsVal = std::vector<std::string>{};
    type = std::vector<TYPE>{};
    range = std::vector<std::string>{};
	udp = false;
	server_addr = NULL;
    return 0;
}

int cmd::sendCMD(uint16_t port)
{
    std::string command {serializeCMD()};
    log_ll_cmd("sending cmd: "+cmd::name+" to port "+std::to_string(port));
    write_socket(port, command);
    return 0;
}

int cmd::recvCMD(int fd)
{
    std::string buffer{};
    read_socket(&buffer, fd);
    deserializeCMD(buffer);
    log_ll_cmd("rcv cmd: "+cmd::name+" from sockfd "+std::to_string(fd));
    return 0;
}

void cmd::printCMD()
{
	std::cout <<":::::::::::::::::::::::::::"<<std::endl;
    std::cout << "SOCKET: " << sockfd << std::endl;
    std::cout << "CMD: " << name << "   STATE: ";
    switch (state)
    {
    case 0:
        std::cout << "ACQ" << std::endl;
        break;
    case 1:
        std::cout << "IDLE" << std::endl;
        break;
    case 2:
        std::cout << "INIT" << std::endl;
        break;
    case 3:
        std::cout << "CONFIG" << std::endl;
        break;
    case 4:
        std::cout << "THREAD" << std::endl;
        break;
    default:
        std::cout << "N/A" << std::endl;
        break;
    }
    for (int ii{0}; ii < nbArg; ++ii)
    {
        std::cout << "ARG: " << args[ii] << "   TYPE: ";
        switch (type[ii])
        {
        case 0:
            std::cout << "INT" << std::endl;
            break;
        case 1:
            std::cout << "FLOAT" << std::endl;
            break;
        case 2:
            std::cout << "DOUBLE" << std::endl;
            break;
        case 3:
            std::cout << "STRING" << std::endl;
            break;
        default:
            std::cout << "N/A" << std::endl;
            break;
        }
        if (argsVal.size() != 0)
            std::cout << "VAL: " << argsVal[ii] << "    Range: " << range[ii] << std::endl;
        else
            std::cout << "VAL: " << "N/A" << "    Range: " << range[ii];
    }
	if (udp)
	{
		std::cout<<"UDPSTRUCT: "<<UDP<<std::endl;
	}
}

// msgHandler Utilities
// msgHandler Utilities
int cmd::parseCMD(std::string msg)
{
    std::vector<std::string> buffArg{};
    std::vector<std::string> buffVal{};
    //replace all ',' by ' '
    std::replace( msg.begin(), msg.end(), ',', ' ');
    log_ll_cmd("received: "+msg);
    std::vector<std::string> sstr = split(msg);
    // Populate the lists of args and corresponding values
    check(&sstr);
    //After check() is called we should always have odd number of element
    if (sstr.size()%2==0)
    {
        log_ll_cmd("number of argument received does not match");
        return -2;
    }
    if (sstr.size()>1)
    {
        for (int i=1;i<sstr.size();i++) {
            if (i%2==0)
            {
             buffVal.push_back(sstr[i]);
            }
            else {
                buffArg.push_back(sstr[i]);
            }


        }
    }

    // Verify the number of arguments
    {
        int numOpt{0};
        for (int ii=0; ii < nbArg; ++ii)
            if (opt[ii])
                ++numOpt;
        if ((int)buffArg.size() > nbArg)
            return -2;
        if ((int)buffArg.size() < (nbArg- numOpt) && (int)buffArg.size() > (nbArg))
            return -2;
    }

    // Resize the argument vector
    argsVal.resize(nbArg);
    for (auto str : argsVal)
        str = "";

    // Verify existance of arguments, populate argVals and verify types
    {
        for (unsigned int ii{0}; ii < buffArg.size(); ++ii)
        {
            bool match{false};
            for (int jj{0}; jj < nbArg; ++jj)
            {
                if (buffArg[ii] == args[jj])
                {
                    match = true;
                    argsVal[jj] = buffVal[ii];
                    if (!verifyType(argsVal[ii], type[ii]))
                        return -4;
                }
            }
            if (!match)
                return -3;
        }
    }
    // Verify if non-optional values are filled
    {
        for (int ii{0}; ii < nbArg; ++ii)
            if (argsVal[ii] == "" && !opt[ii])
                return -2;
    }

    return 0;
}

bool cmd::verifyType(std::string &str, TYPE t)
{
    if (t == STRING)
        return true;

    char * endptr = nullptr;

    if (t == INT)
        std::strtol(str.c_str(), &endptr, 10);
    else if (t == FLOAT)
        std::strtof(str.c_str(), &endptr);
    else if (t == DOUBLE)
        std::strtod(str.c_str(), &endptr);

    if (*endptr)
        return false;

    return true;
}

std::string cmd::operator[](const char *cmdName)
{
    for (int ii{0}; ii < nbArg; ++ii)
    {
        if (args[ii] == cmdName)
        {
            return argsVal[ii];
        }
    }
    return "";
}
std::vector<std::string> cmd::split(std::string line)
/*!
 * Split a string with space.
 */
{
    std::vector<std::string> ll;
    std::string buff="";
    for (auto &c:line)
    {
        if (c!=' ' && c!='\t' && c!='\n')
        {
            buff+=c;
        }
        else if (!buff.empty()){
            ll.push_back(buff);
            buff.clear();
        }
    }
    if (!buff.empty())
    {
        ll.push_back(buff);
    }
    return ll;
}
void cmd::check(std::vector<std::string> *t)
/*!
 * for each argument with a dash in front
 * i.e., -t4, the argument will become -t4 -
 * to keep matching pairs. e.g.,
 * setParam read 1 -setunique, will become
 * setParam read 1 -setunique -
 */
{
    std::vector<std::vector<std::string>::iterator> where;
    for (std::vector<std::string>::iterator it = t->begin(); it!= t->end(); it++) {
        if (it->size()>2)
        {
            if ((*it)[0]=='-' && isalpha((*it)[1]))
            {
                where.push_back(it);
            }
        }
    }
    for (auto w:where)
    {
        *t->insert(w+1,"-");
    }
    return;
}

//-----------------------//
//       Hash Table      //
//-----------------------//

hashT::hashT()
{
	value.clear();
	key.clear();
}
hashT::~hashT()
{
	value.clear();
	key.clear();
}
hashT::hashT(std::string str,const char key_sep,const char value_sep)
{
	std::vector<std::string> buffer = hashT::split(str,key_sep);
	std::vector<std::string> key_value{};
	for (auto kv : buffer)
	{
		key_value = hashT::split(kv,value_sep);
		if (key_value.size()!=2){key_value.clear();continue;}
		hashT::insert(key_value[0],key_value[1]);
		key_value.clear();
	}
}
std::string hashT::operator[](std::string key)
{	
	int i=0;
	
	for (auto v:hashT::key)
	{
		if (key.compare(v)==0)
		{
			return value[i];			
		}
		i++;
	}
	return "";
}
void hashT::insert(std::string key,std::string value)
{
	hashT::key.push_back(key);
	hashT::value.push_back(value);
}
std::vector<std::string> hashT::split(std::string str,const char sep)
/*
 * Split a c++ string into an string array. Separator <sep>
 * is used to do the splitting. 
 */ 
{
	std::vector<std::string> buffer{};
	std::string buff{};
	for (auto c : str)
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