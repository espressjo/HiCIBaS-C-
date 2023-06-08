#ifndef SERIAL_H
#define SERIAL_H
#include <stdio.h>		/* fprintf */
#include <stdlib.h>
#include <string.h>		/* strcpy, strlen */
#include <ctype.h>		/* tolower */
#include <fcntl.h>      	/* open */
#include <unistd.h>		/* read, write, close */
#include <termio.h>		/* ioctl */
#include <string>

#define DELAIMAX 5000	/* delai maximum pour l'attente en ms*/
#define MAX_CHAR 4096   /* nombre de charactere maximal à lire*/

#define NOK -1            	/* valeur de retour avec erreur */
#define OK 0
#define OPEN 1
#define CLOSE -2

class serial
{
public:
    serial();
    int ouvrirport(int port,int vitesse) ;
    int configport();
    int fermerport();
    int ecrireport(std::string str);
    int lireport (std::string *str, int ncar);
    int lirec(std::string *str,char c);
    int verification(std::string str);
    int changeBaudRate(int baudrate);
    int status;
    int set_usb(void);
private:
    struct termio serie;		/* pour la configuration du port serie */
    int idport;
    bool usb;
    int fd;
    std::string conversion(std::string ss);
    void delay(int ms);

};

#endif // LAKESHORE_RS232_H
