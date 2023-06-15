#include "uics_sockets.h"
#include "inst_log.h"


int create_socket(uint16_t port)
/*!
  * \brief Description
  * ___________
  *     This function create a socket and return the socket
  *     file descriptor.
  * \brief Return
  * ______
  *     return -1 if it failed to create the socket.
  */
{
    struct sockaddr_in server;
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock < 0 )
    {
        Perror("Failed to create socket");
        return -1;
    }
    int enable=1;

    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)//to make sure the socket as soon as the program stops
    {
        Perror("setsockopt(SO_REUSEADDR) failed");
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if(bind(sock,(struct sockaddr *)&server,sizeof(server)) )
    {

        log_socket("bind failed");
        return -1;
    }
    if (listen(sock,10)!=0)
    {
        Perror("failed to listen to socket");

        return -1;
    }
    return sock;}

int read_socket(std::string *buffer,int sockfd)
/*!
  * \brief Description
  * ___________
  *     This function read a socket file descriptor and copy
  *     the message into buffer. Max length of message is 9216
  *     characters. The message read always starts by the
  *     character L followed by 2 extra characters indicating
  *     the length of the message. This implementation is
  *     hidden, i.e., one should not care about this when using
  *     the read_socket and write_socket function of this
  *     library, but only if this function reads the buffer
  *     sent from third party fuction.
  * \brief Return
  * ______
  *     return 0 if succesfull, -1 if it fails to read the
  *     socket.
  */

{
    int sock = accept(sockfd, static_cast<struct sockaddr *>( nullptr), nullptr);
    if(sock == -1 )
    {
        Perror("accept failed");
        return -1;
    }

    long int rval=1;
    char buff[1024];
    char tobecompare[2];
    long int nbchar;
    tobecompare[0]='L';
    tobecompare[1]='\0';
    memset(buff,'\0',sizeof(buffer->c_str()));

    rval = recv(sock, buff, 1,0);
    buffer->assign(buff,static_cast<size_t>(rval));
    std::string comp = *buffer;
    std::string bb="";
    if (comp.compare(tobecompare)==0)
    {


        recv(sock, buff, 4,0);
        nbchar = static_cast<size_t>(atol(buff));
        ssize_t total = nbchar;
        ssize_t rc=0;
        std::string();
        memset(buff,'\0',1024);
        while (total>0)
        {
            rc = recv(sock, buff, 1024,0);

            total-=rc;
            bb+=std::string(buff);
            memset(buff,'\0',1024);
        }

        std::string bbb="";
        for (auto &c:bb) {
            if (c!='\n')
            {
                bbb+=c;
            }
        }
        buffer->assign(bbb);
    }//if

    if((rval) < 0 )
    {
        Perror("reading stream message error");
        return -1;
    }//if

    if (close(sock)!=0)
    {
        printf("Unable to close the communication. Please restart the program.\n");
        log_socket("Unable to close the communication. Please restart the program.");
        return -1;
    }

    return 0;
}

int read_image(unsigned int *im,size_t length,int sockfd)
/*!
  * \brief Description
  * ___________
  *     This function reads an array of uint16 of a given
  *     length from a socket file descriptor. The array is
  *     transfered in chunks of 512 bytes.
  *     This function is ideal to transfer images by socket.
  *     The size of the array must be known prior of using
  *     this function and the array must be able to contain
  *     all entire size. There is no guards to prevent memory
  *     problems.
  * \brief Return
  * ______
  *     return 0 if succesfull, -1 if it fails to read the
  *     socket.
  */
{
    int sock = accept(sockfd, static_cast<struct sockaddr *>(nullptr), nullptr);
    if(sock == -1 )
    {
        Perror("accept failed");
        return -1;
    }

    //long int rval=1;
    unsigned int buffer[512];


    size_t received=0;
    size_t it=0;
    unsigned int rcv=0;
    while (received/sizeof(unsigned int) < length)
    {
           while(rcv<sizeof(unsigned int)*512)
        {

            rcv+=recv(sock, buffer,sizeof (unsigned int)*512,0);
            received+=rcv;
            printf("recv: %u\n",rcv);
                if (rcv>0)
            {
                    for (unsigned long int i=0;i<rcv/sizeof(unsigned int);i++)
                    {
                        im[it] = buffer[i];
                        it++;
                    }
            }
                else if (rcv==0)
            {
                    break;
                }
                else
            {
                    Perror("reading stream message error");
                    return -1;
                }

            rcv=0;
        }
    }
    if (close(sock)!=0)
    {
        printf("Unable to close the communication. Please restart the program.\n");
        return -1;
    }
    return 0;
}

int read_socket(uint16_t port,std::string *buffer,std::string addr)
/*!
  * \brief Description
  * ___________
  *     Read a socket given and addresse and a port and store the
  *     message in buffer. Max length of message is 9216 characters.
  *     The message read always starts by the character L followed
  *     by 2 extra characters indicating the length of the message.
  *     This implementation is hidden, i.e., one should not care
  *     about this when using the read_socket and write_socket
  *     function of this library, but only if this function reads
  *     the buffer sent from third party fuction.
  *
  * \brief Return
  * ______
  *     returns 0 if successfull and -1 if it fails to read the
  *     socket.
  */

{
    int sock;
    struct sockaddr_in server;
    struct hostent *hp;

    sock = socket(AF_INET, SOCK_STREAM,0);
    if(sock < 0)
    {
       Perror("socket failed");
       return -1;
    }
    server.sin_family = AF_INET;
    hp = gethostbyname(addr.c_str());
    if(hp==nullptr)//modification 2018-01-13
    {
        Perror("gethostbyname failed");
        close(sock);
        return -1;
    }
    if(hp ==0)
    {
        Perror("gethostbyname failed");
        return -1;
    }
    memcpy(&server.sin_addr, hp->h_addr, static_cast<size_t>(hp->h_length));

    server.sin_port = htons(port);

    if((connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) )
    {
        Perror("connect failed");
        close(sock);
        return -1;
    }
    ssize_t rval=1;
    char buff[1024];
    char tobecompare[2];
    size_t nbchar;
    tobecompare[0]='L';
    tobecompare[1]='\0';
    memset(buff,'\0',sizeof(buff));

    rval = recv(sock, buff, 1,0);
    buffer->assign(buff,static_cast<size_t>(rval));
    std::string comp = *buffer;
    std::string bb="";
    if (comp.compare(tobecompare)==0)
    {
        recv(sock, buff, 4,0);
        nbchar = static_cast<size_t>(atol(buff));
        ssize_t total = nbchar;
        ssize_t rc=0;
        std::string();
        memset(buff,'\0',1024);

        while (total>0)
        {
            rc = recv(sock, buff, 1024,0);
            total-=rc;
            bb+=std::string(buff);
            memset(buff,'\0',1024);
        }

        std::string bbb="";
        for (auto &c:bb) {
            if (c!='\n')
            {
                bbb+=c;
            }
        }
        buffer->assign(bbb);

    }//if

    if((rval) < 0 )
    {
        Perror("reading stream message error");
        return -1;
    }//if

    if (close(sock)!=0)
    {
        printf("Unable to close the communication. Please restart the program.\n");
        log_socket("Unable to close the communication. Please restart the program.");
        return -1;
    }
    else {
    }
    return 0;


}

int read_socket(uint16_t port,std::string *buffer)
/*!
  * \brief Description
  * ___________
  *     Reads a socket on the localhost on port #. Max length of
  *     message is 9216 characters. The message read always starts
  *     by the character L followed by 2 extra characters
  *     indicating the length of the message. This implementation
  *     is hidden, i.e., one should not care about this when using
  *     the read_socket and write_socket function of this library,
  *     but only if this function reads the buffer sent from third
  *     party fuction.
  * \brief Return
  * ______
  *     return 0 if successfull and -1 if not.
  *
  */

{

    int sock;
    struct sockaddr_in server;
    struct hostent *hp;
    std::string bb="";
    sock = socket(AF_INET, SOCK_STREAM,0);
    if(sock < 0)
    {
       Perror("socket failed");
       return -1;
    }
    server.sin_family = AF_INET;
    hp = gethostbyname("localhost");
    if(hp==nullptr)//modification 2018-01-13
    {
        Perror("gethostbyname failed");
        close(sock);
        return -1;
    }
    if(hp ==0)
    {
        Perror("gethostbyname failed");
        return -1;
    }
    memcpy(&server.sin_addr, hp->h_addr, static_cast<size_t>(hp->h_length));

    server.sin_port = htons(port);
    if((connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) )
    {
        Perror("connect failed");
        close(sock);
        return -1;
    }
    ssize_t rval=1;
    char buff[1024];
    char tobecompare[2];
    size_t nbchar;
    tobecompare[0]='L';
    tobecompare[1]='\0';
    memset(buff,'\0',sizeof(buffer->c_str()));

    rval = recv(sock, buff, 1,0);
    buffer->assign(buff,static_cast<size_t>(rval));
    std::string comp = *buffer;
    if (comp.compare(tobecompare)==0)
    {
        recv(sock, buff, 4,0);
        nbchar = static_cast<size_t>(atol(buff));
        ssize_t total = nbchar;
        ssize_t rc=0;
        std::string();
        memset(buff,'\0',1024);

        while (total>0)
        {


            rc = recv(sock, buff, 1024,0);
            total-=rc;
           // std::cout<<"rc: "<<rc<<std::endl;
           // std::cout<<"buff: "<<buff<<std::endl;
            bb+=std::string(buff);
            memset(buff,'\0',1024);
        }
        std::string bbb="";
        for (auto &c:bb) {
            if (c!='\n')
            {
                bbb+=c;
            }
        }
        buffer->assign(bbb);


    }//if

    if((rval) < 0 )
    {
        Perror("reading stream message error");
        return -1;
    }//if

    if (close(sock)!=0)
    {
        printf("Unable to close the communication. Please restart the program.\n");
        log_socket("Unable to close the communication. Please restart the program.");
        return -1;
    }

    return 0;


}

int write_socket(uint16_t port, std::string command)
/*!
  * \brief Description
  * ___________
  *     Writes a socket on the localhost on port #. Max length of
  *     message is 9216 characters. The message writes always starts
  *     by the character L followed by 2 extra characters
  *     indicating the length of the message. This implementation
  *     is hidden, i.e., one should not care about this when using
  *     the read_socket and write_socket function of this library,
  *     but only if this function reads the buffer sent from third
  *     party fuction.
  * \brief Return
  * ______
  *     return 0 if successfull and -1 if not.
  *
  */

{
    if (command.size()>=9216)
    {
        Perror("string buffer will be truncated");
    }
    int sock;
    struct sockaddr_in server;
    struct hostent *hp;
    char command_to_send[9216];

    sock = socket(AF_INET, SOCK_STREAM,0);
    if(sock < 0){
        Perror("socket failed");
       return -1;
        }
    server.sin_family = AF_INET;
    hp = gethostbyname("localhost");
    if(hp==nullptr){//modification 2018-01-13
        Perror("gethostbyname failed");
        close(sock);
        return -1;	}
    if(hp ==0)
        {
        Perror("gethostbyname failed");
        return -1;
        }
    memcpy(&server.sin_addr, hp->h_addr, static_cast<size_t>(hp->h_length));

    server.sin_port = htons(port);
    if((connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) )
        {
        Perror("connect failed");
        close(sock);
        return -1;
        }

    sprintf(command_to_send,"L%4.4lu%s\n",strlen(command.c_str()),command.c_str());




    if (send(sock, command_to_send,    strlen(command_to_send),0)==-1)
    {
        printf("Unable to send to socket. Closing communications.\n");
        log_socket("Unable to send to socket. Closing communications.");
        if (close(sock)!=0)
        {

            printf("Unable to close the communication. Please restart the program.\n");
            log_socket("Unable to close the communication. Please restart the program.");
            return -1;
        }
        return -1;
    }


    if (close(sock)!=0)
    {
        printf("Unable to close the communication. Please restart the program.\n");
        log_socket("Unable to close the communication. Please restart the program.");
        return -1;
    }
    return 0;
}
int write_socket(uint16_t port, std::string command,std::string addr)
/*!
  * \brief Description
  * ___________
  *     Write a socket on the host with address addr on port #.
  *     Max length of message is 9216 characters. The message read
  *     always starts by the character L followed by 2 extra
  *     characters indicating the length of the message. This
  *     implementation is hidden, i.e., one should not care about
  *     this when using the read_socket and write_socket function
  *     of this library, but only if this function reads the buffer
  *     sent from third party fuction.
  * \brief Return
  * ______
  *     return 0 if successfull and -1 if not.
  *
  */

{

    if (command.size()>=9216)
    {
        Perror("string buffer will be truncated");
    }
    int sock;
    struct sockaddr_in server;
    struct hostent *hp;
    char command_to_send[9216];
    sock = socket(AF_INET, SOCK_STREAM,0);
    if(sock < 0){
        Perror("socket failed");
       return -1;
        }
    server.sin_family = AF_INET;
    hp = gethostbyname(addr.c_str());
    if(hp==nullptr){//modification 2018-01-13
        Perror("gethostbyname failed");
        close(sock);
        return -1;	}
    if(hp ==0)
        {
        Perror("gethostbyname failed");
        return -1;
        }
    memcpy(&server.sin_addr, hp->h_addr, static_cast<size_t>(hp->h_length));

    server.sin_port = htons(port);
    if((connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) )
        {
        Perror("connect failed");
        close(sock);
        return -1;
        }
    sprintf(command_to_send,"L%4.4lu%s\n",strlen(command.c_str()),command.c_str());

    if (send(sock, command_to_send,    strlen(command_to_send),0)==-1)
    {
        printf("Unable to send to socket. Closing communications.\n");
        log_socket("Unable to send to socket. Closing communications.");
        if (close(sock)!=0)
        {

            printf("Unable to close the communication. Please restart the program.\n");
            log_socket("Unable to close the communication. Please restart the program.");
            return -1;
        }
        return -1;
    }


    if (close(sock)!=0)
    {
        printf("Unable to close the communication. Please restart the program.\n");
        log_socket("Unable to close the communication. Please restart the program.");
        return -1;
    }
    return 0;
}

int write_image(uint16_t port, unsigned int *im,size_t length,std::string addr)
/*!
  * \brief Description
  * ___________
  *     This function writes an array of uint16 of a given
  *     length on socket with address addr and port # file
  *     descriptor. The array is transfered in chunks of
  *     512 bytes. This function is ideal to transfer images
  *     by socket. The size of the array must be known prior
  *     of using this function and the array must be able to
  *     contain all entire size. There is no guards to prevent
  *     memory problems.
  * \brief Return
  * ______
  *     return 0 if succesfull, -1 if it fails to read the
  *     socket.
  */
{
    int sock;
    struct sockaddr_in server;
    struct hostent *hp;

    sock = socket(AF_INET, SOCK_STREAM,0);
    if(sock < 0){
        Perror("socket failed");
       return -1;
        }
    server.sin_family = AF_INET;
    hp = gethostbyname(addr.c_str());
    if(hp==NULL){//modification 2018-01-13
        Perror("gethostbyname failed");
        close(sock);
        return -1;	}
    if(hp ==0)
        {
        Perror("gethostbyname failed");
        return -1;
        }
    memcpy(&server.sin_addr, hp->h_addr, static_cast<size_t>(hp->h_length));

    server.sin_port = htons(port);
    //server.sin_addr.s_addr = inet_addr("132.204.60.76");
    if((connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) )
        {
        Perror("connect failed");
        close(sock);
        return -1;
        }
    size_t it = 0;
    unsigned int buffer[512];
    //size_t sent = 0;
    while (it<length)
    {   printf("left over: %lu\n",length-it);
        for (int i=0;i<512;i++)
        {
            buffer[i] = im[it];
            it++;
        }

        if (send(sock, buffer, sizeof(unsigned int)*512,0)==-1)
        {

            printf("Unable to send to socket. Closing communications.\n");
            log_socket("Unable to send to socket. Closing communications.");
            if (close(sock)!=0)
            {
                printf("Unable to close the communication. Please restart the program.\n");
                log_socket("Unable to close the communication. Please restart the program.");
                return -1;
            }
            return -1;
        }


    }

    printf("done!\n");


    if (close(sock)!=0)
    {
        printf("Unable to close the communication. Please restart the program.\n");
        log_socket("Unable to close the communication. Please restart the program.");
        return -1;
    }
    return 0;
}

int write_socket(std::string command,int sock)
/*!
  * \brief Description
  * ___________
  *     Write a socket on the localhost with file descriptor sock.
  *     Max length of message is 9216 characters. The message read
  *     always starts by the character L followed by 2 extra
  *     characters indicating the length of the message. This
  *     implementation is hidden, i.e., one should not care about
  *     this when using the read_socket and write_socket function
  *     of this library, but only if this function reads the buffer
  *     sent from third party fuction.
  * \brief Return
  * ______
  *     return 0 if successfull and -1 if not.
  *
  */
{
    int sockfd = accept(sock,static_cast<struct sockaddr *>(nullptr), nullptr);
    if(sockfd == -1 )
    {
        Perror("accept failed");
        return -1;
    }

    char command_to_send[9216];
    sprintf(command_to_send,"L%4.4lu%s\n",strlen(command.c_str()),command.c_str());
    if (send(sockfd, command_to_send,    strlen(command_to_send),0)==-1)
    {
        printf("Unable to send to socket. Closing communications.\n");
        log_socket("Unable to send to socket. Closing communications.");
        if (close(sockfd)!=0)
        {
            printf("Unable to close the communication. Please restart the program.\n");
            log_socket("Unable to close the communication. Please restart the program.");
            return -1;
        }
        return -1;
    }
    if (close(sockfd)!=0)
    {
        printf("Unable to close the communication. Please restart the program.\n");
        log_socket("Unable to close the communication. Please restart the program.");
        return -1;
    }
    return 0;
}



