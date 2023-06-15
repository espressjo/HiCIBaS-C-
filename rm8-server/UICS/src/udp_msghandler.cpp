#include "udp_msghandler.h"
#include "uics_sockets.h"
#include "uics_config.h"
#include "inst_log.h"

#define FAILED "failed"
#define READY "ready"

extern Log instLog;

#include "uics_cmds.h"



udp_msgHandler::udp_msgHandler()
{

    opt = TRUE;

    //init toutes les commandes
    allcmds = new cmdList(join(CONFPATH,"cmd.conf"));
    std::cout<<"UDP server is using cmd list: "<<join(CONFPATH,"cmd.conf")<<std::endl;


		
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
		
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(UDPCLIENTPORT);
		
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Waiting for UDP connection on port %d \n", UDPCLIENTPORT);
    
}

void udp_msgHandler::run()
{   //a message
    char connection_buffer[256];
    //instLog.writeto("[UICS] Starting UICS",1);
	char buffer[MAXLINE];
	char hello[24] = "Hello from server";
	while (true)
	{
		int n;
		socklen_t len;
		std::string buff="";
		len = sizeof(cliaddr);
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
		//std::cout<<"recv: "<<buffer<<std::endl;
		std::string msg{};
		std::string cmdName;
		
		
		buffer[n] = '\0';
		for (int i=0;i<n;++i)
		{
			if (buffer[i]!='\n' && buffer[i]!='\r' && buffer[i]!='\0')
			{
				buff+=buffer[i];
			}
		}
		
		
		
		for (auto c : buffer)
		{
		   if (c == ' ' || c == '\n' || c == '\0' || c == '\t' || c == '\r')
			   break;
		   else
			   cmdName.push_back(c);
		}
		//std::cout<<"command: "<<cmdName<<std::endl;
		instLog.writeto("[msg H] Incoming command: "+cmdName,1);
		
		// Get the command from list
		if (cmdName.compare("lscmd")==0)
		{
			buff = allcmds->getCMDlist();
			sndMsg(sockfd,buff,&cliaddr);
			continue;
		}
	   if (cmdName.size()>0 && cmdName[0]=='?')
	   {
		   buff = allcmds->getcmdinfo(cmdName);
		   sndMsg(sockfd,buff,&cliaddr);
		   continue;
	   }


	   
		cmd cm = (*allcmds)[cmdName];
		cmd *c = &cm;
		if (c->name.compare("")==0)
		{
		  
		   instLog.writeto("[msg H] Unknown command",1);
		   sndMsg(sockfd,"unknown command",uicsCMD_ERR_UNKNOWN_KEYW,&cliaddr);
		   continue;
		}

		// Add arguments to the command
		int s = c->parseCMD(buff);
		//std::cout<<"buff: "<<buff<<std::endl;
		//std::cout<<"s: "<<s<<std::endl;
		if (s == -1)
		{
			sndMsg(sockfd, "Wrong formating of command. Missing argument names or values",&cliaddr);
			instLog.writeto("[msg H] Wrong formating of command. Missing argument names or values",1);
			continue;
		}
		else if (s == -2)
		{
			sndMsg(sockfd,"Wrong number of arguments",uicsCMD_ERR_PARAM_FORMAT,&cliaddr);
			instLog.writeto("[msg H] Wrong number of arguments",1);
			continue;
		}
		else if (s == -3)
		{
		   sndMsg(sockfd, "Wrong argument names",&cliaddr);
		   instLog.writeto("[msg H] Wrong argument names",1);
		   continue;
		}
		else if (s == -4)
		{
			sndMsg(sockfd,"Wrong argument types",uicsCMD_ERR_PARAM_VALUE,&cliaddr);
			instLog.writeto("[msg H] Wrong argument types",1);
			continue;
		}

		// Assigning socket fd to the command
		c->sockfd = sockfd;
		instLog.writeto("[msg H] sockfd"+std::to_string(sd)+" assigned to "+cmdName,1);
		c->set_datagram(cliaddr);
		c->udp = true;
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
		   sndMsg(sockfd,"problem with the state handler. Restart the software",uicsCMD_ERR_VALUE,&cliaddr);
		   instLog.writeto("[msg H] problem with the state handler. Restart the software",1);
		   continue;
		}

		// Sending command to state_handler
		c->sendCMD(PORT4MSG);

	   // Validation from state_handler
	   read_socket(PORT4MSG,&msg);
	   if (msg.compare(READY)!=0)
	   {
		   sndMsg(sockfd,"problem with the state handler. Restart the software",uicsCMD_ERR_VALUE,&cliaddr);
		   instLog.writeto("[msg H] problem with the state handler. Restart the software",1);
		   continue;
	   }

	}
	/*
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
    }*/
}


