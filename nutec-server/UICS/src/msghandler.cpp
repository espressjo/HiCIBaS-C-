#include "msghandler.h"
#include "uics_sockets.h"
#include "uics_config.h"
#include "inst_log.h"
#define FAILED "failed"
#define READY "ready"

Log instLog;

#include "uics_cmds.h"

bool msgHandler::compstrip(char *buffer,const char* str )
{
    std::string buff="";
    int i=0;
    while (buffer[i]!='\0') {

        if (buffer[i]!='\n' && buffer[i]!='\r')
        {
            buff+=buffer[i];
        }
        i++;
    }
    if (buff.compare(str)==0)
    {
        return true;
    }
    return false;
}
//msgHandler::msgHandler(std::string log, int level)
//{
//    //instLog.setLevel(level);
//    //instLog.setPath(log);
//    opt = TRUE;
//    max_clients = 30;
//    //sockfd1 = s1;
//    //sockfd2 = s2;



//    //init toutes les commandes
//    allcmds = new cmdList(CMDCONFFILE);

//    //initialise all client_socket[] to 0 so not checked
//    for (i = 0; i < max_clients; i++)
//    {
//        client_socket[i] = 0;
//    }

//    //create a master socket
//    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
//    {
//        perror("socket failed");
//        exit(EXIT_FAILURE);
//    }

//    //set master socket to allow multiple connections ,
//    //this is just a good habit, it will work without this
//    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
//        sizeof(opt)) < 0 )
//    {
//        perror("setsockopt");
//        exit(EXIT_FAILURE);
//    }

//    //type of socket created
//    address.sin_family = AF_INET;
//    address.sin_addr.s_addr = INADDR_ANY;
//    address.sin_port = htons( CLIENTPORT );

//    //bind the socket to localhost port 8888
//    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
//    {
//        perror("bind failed");
//        exit(EXIT_FAILURE);
//    }
//    printf("Listener on port %d \n", CLIENTPORT);

//    //try to specify maximum of 3 pending connections for the master socket
//    if (listen(master_socket, 3) < 0)
//    {
//        perror("listen");
//        exit(EXIT_FAILURE);
//    }

//    //accept the incoming connection
//    addrlen = sizeof(address);
//    puts("Waiting for connections ...");
//}
void msgHandler::setlPath(std::string path)
{
    //instLog.setLevel(level);
    instLog.setPath(path);
}
void msgHandler::setllevel(int level)
{
    instLog.setLevel(level);
}
msgHandler::msgHandler()
{

    opt = TRUE;
    max_clients = 30;
    //sockfd1 = s1;
    //sockfd2 = s2;


    //init toutes les commandes
    allcmds = new cmdList(join(CONFPATH,"cmd.conf"));
    std::cout<<"Using cmd list: "<<join(CONFPATH,"cmd.conf")<<std::endl;
    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections ,
    //this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
        sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( CLIENTPORT );

    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", CLIENTPORT);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");
}
void msgHandler::run()
{   //a message
    const char *message = "ECHO Daemon v1.0 \r\n";
    char connection_buffer[256];
    //instLog.writeto("[UICS] Starting UICS",1);

    while(TRUE)
    {
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            instLog.writeto("[msg H] select error",1);
            printf("select error");
        }

        //If something happened on the master socket ,
        //then its an incoming connection

        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {

                perror("accept");
                instLog.writeto("[msg H] accept",1);
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                (address.sin_port));
            memset(connection_buffer,0,256);
            sprintf(connection_buffer,"New connection , socket fd is %d , ip is : %s , port : %d" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                    (address.sin_port));
            instLog.writeto("[msg H] "+std::string(connection_buffer),1);
            //send new connection greeting message
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                instLog.writeto("[msg H] send",1);
                perror("send");
            }
            puts("Welcome message sent successfully");
            instLog.writeto("[msg H] Welcome message sent successfully",1);
            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                    instLog.writeto("[msg H] Adding to list of sockets as "+std::to_string(i),1);
                    break;
                }
            }
        }

        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the
                //incoming message
                valread = read( sd , buffer, 1024);
                if (valread == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" ,
                        inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                    memset(connection_buffer,0,256);
                    sprintf(connection_buffer,"Host disconnected , ip %s , port %d" ,inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                    //Close the socket and mark as 0 in list for reuse
                    instLog.writeto("[msg H] "+std::string(connection_buffer),1);
                    close( sd );
                    client_socket[i] = 0;
                }
		/* If read return value < 0 it probably means that the connection is reset by peer.
                 * We will still log the errno to investigate why.
                 */
                else if (valread < 0)
                {
                    instLog.writetoVerbose("[msg H] read() error, errno="+std::to_string(errno));
                    instLog.writetoVerbose("[msg H] "+std::string(strerror(errno)));
                    instLog.writetoVerbose("[msg H] Closing socket: "+std::to_string(sd));
                    close(sd);
                    client_socket[i] = 0;
                    continue;
                }
                //Echo back the message that came in
                else
                {

                    std::string buff="";
                    buffer[valread] = '\0';
                    for (int i=0;i<valread;++i)
                    {
                        if (buffer[i]!='\n' && buffer[i]!='\r' && buffer[i]!='\0')
                        {
                            buff+=buffer[i];
                        }
                    }
                   

                   std::string msg{};

                   // Get the command name
                   std::string cmdName;
                   for (auto c : buffer)
                   {
                       if (c == ' ' || c == '\n' || c == '\0' || c == '\t' || c == '\r')
                           break;
                       else
                           cmdName.push_back(c);
                   }

                   instLog.writeto("[msg H] Incoming command: "+cmdName,1);
                   // Get the command from list
                    if (cmdName.compare("lscmd")==0)
                    {
                        buff = allcmds->getCMDlist();
                        sndMsg(sd,buff);
                        continue;
                    }
                   if (cmdName.size()>0 && cmdName[0]=='?')
                   {
                       buff = allcmds->getcmdinfo(cmdName);
                       sndMsg(sd,buff);
                       continue;
                   }

                   cmd cm = (*allcmds)[cmdName];
                   cmd *c = &cm;
                   if (c->name.compare("")==0)
                   {
                      // sendError(sd,"Unknow command");
                       instLog.writeto("[msg H] Unknown command",1);
                       sndMsg(sd,"unknown command",uicsCMD_ERR_UNKNOWN_KEYW);
                   // instLog.writeto("[UICS] unknown command: "+cmdName,1);
                       continue;
                   }

                   // Add arguments to the command

                   int s = c->parseCMD(buff);
                   if (s == -1)
                   {
                       sndMsg(sd, "Wrong formating of command. Missing argument names or values");

                      instLog.writeto("[msg H] Wrong formating of command. Missing argument names or values",1);
                       continue;
                   }
                   else if (s == -2)
                   {
                       sndMsg(sd,"Wrong number of arguments",uicsCMD_ERR_PARAM_FORMAT);
                       instLog.writeto("[msg H] Wrong number of arguments",1);
                      // instLog.writeto("[UICS] Wrong number of arguments",1);
                       continue;
                   }
                   else if (s == -3)
                   {
                       sndMsg(sd, "Wrong argument names");
                       instLog.writeto("[msg H] Wrong argument names",1);
                       //instLog.writeto("[UICS] Wrong argument names",1);

                       continue;
                   }
                   else if (s == -4)
                   {
                       sndMsg(sd,"Wrong argument types",uicsCMD_ERR_PARAM_VALUE);
                       instLog.writeto("[msg H] Wrong argument types",1);
                       //instLog.writeto("[UICS] Wrong argument types",1);
                       continue;
                   }

                   // Assigning socket fd to the command
                   c->sockfd = sd;
                   instLog.writeto("[msg H] sockfd"+std::to_string(sd)+" assigned to "+cmdName,1);

                   // Sending to thread if required
                   if (c->state==THREAD)
                   {
                        instLog.writeto("[msg H] "+cmdName+" is thread based, cmd is transfered to port "+std::to_string(c->port),1);
                       c->sendCMD(c->port);

                       continue;
                   }
                   write_socket(PORT4MSG,std::string("INCOMINGCMD"));
                   read_socket(PORT4MSG,&msg);
                   if (msg.compare(READY)!=0)
                   {
                       sndMsg(sd,"problem with the state handler. Restart the software",uicsCMD_ERR_VALUE);
                       instLog.writeto("[msg H] problem with the state handler. Restart the software",1);
                       continue;
                   }
                  

                   // Sending command to state_handler
                   c->sendCMD(PORT4MSG);

                   // Validation from state_handler
                   read_socket(PORT4MSG,&msg);
                   if (msg.compare(READY)!=0)
                   {
                       sndMsg(sd,"problem with the state handler. Restart the software",uicsCMD_ERR_VALUE);
                       instLog.writeto("[msg H] problem with the state handler. Restart the software",1);
                       continue;
                   }


                }
            }
        }
    }
}


