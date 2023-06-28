#include "utility_socket.h"

int snd_cmd(std::string cmd,std::string *value_returned,int port,int udp_port,std::string host,bool tcpip,int timeout)
/*
 * Description
 * -----------
 *      First a connection is established with the server. Handshake is
 *      performed, then cmd is sent. <value_returned> will be set to the 
 *      server response to <cmd>. This function can only be used with 
 *      server response for a single "sentence" (up to the first \n read).
 *      The connection will be closed whenever the function return. 
 * 
 *      value_returned will be set to whatever is follows the OK (we strip
 *      the OK for you)
 * 
 *              
 * 
 * NOTE
 * ----
 *      Since the connection is establish (with handshake) and close at 
 *      the end of all call, this is not ideal for slow or unstable connections.
 * 
 * Return 
 * ------
 *      OK (0)       -> successfully read, 
 *      NOK (-1)     -> We receive NOK
 *      CONNECTION_P -> Connection problem (-2)
 */
{
        timeout;
        if (tcpip){
                //-------------//
                //    TCP/IP   //
                //-------------//
                std::string buff="";
                int byte_sent=0;//number of bytes sent
                socket_ sock(host,static_cast<uint16_t>(port),timeout);
                if (sock.status!=0){return CONNECTION_P;}//if cannot connect return False
                if (sock.readWelcomeMessage()!=0){sock.closeSocket(); return CONNECTION_P;}
                //make sure there is a \n at the end of the command
                if (cmd[cmd.length()-1]!='\n')
                {
                        cmd+='\n';
                }
                byte_sent = sock.writeSocket(cmd);
                if (byte_sent!=cmd.length()){sock.closeSocket(); return CONNECTION_P;}
                *value_returned = sock.readSocket();
				sock.closeSocket();
                //make sure we strip \n \r
                std::string buff2="";
                for (auto &c: *value_returned)
                {
                        if (c!='\n' and c!='\r'){buff+=c;}
                }
                *value_returned=buff;
                if (value_returned->substr(0,2).compare("OK")==0){
                        //we strip the OK
                        if (value_returned->length()<3){*value_returned="";}
                        else{*value_returned = value_returned->substr(3,value_returned->length());}
                        return OK;
                }
                //we received an NOK answer
                return NOK;
        }
        //-------------//
        //     UDP     //
        //-------------//
        else {
                udp_client server(host,udp_port,timeout);
                int ret = server.send_rcv_strip(cmd,value_returned);
                if (ret==0){return OK;}
                return CONNECTION_P;

        }


}
