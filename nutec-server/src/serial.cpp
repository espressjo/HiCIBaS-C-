#include "serial.h"

serial::serial()
{
     idport=0;
     fd=0;
     status=CLOSE;
     usb = false;
}
int serial::set_usb()
{
    if (fd==0)
    {
        usb = true;
        return OK;
    }
    else{
        return NOK;
    }
}
int serial::ouvrirport(int port,int vitesse) /* port = "0", "1" ou "2" , Vallee fev. 2012 */

{
  std::string fichier = "";
    if (usb)
    {
        fichier="/dev/ttyUSB"+std::to_string(port);
    }
    else {
        fichier="/dev/ttyS"+std::to_string(port);
    }

  if ((fd = open(fichier.c_str(), O_RDWR)) == NOK)
  {
    fprintf(stderr,"Erreur lors de l'ouverture du port serie %s, fd =%d .\n",fichier.c_str(),fd);
    status = NOK;
    return NOK;
  }

  serie.c_iflag = IGNCR;     	/* ignore <CR> */
  serie.c_oflag = 0;
  serie.c_cflag = CS8 | CREAD | CLOCAL;
  serie.c_lflag = 0;
  serie.c_cc[VMIN] = 0;		/* nombre minimum de caracteres */
  //serie.c_cc[VTIME] = 1;	/* nombre x 0.1 s d'attente max */
                /* lors d'un appel de read */
  if(vitesse==19200)
    serie.c_cflag = serie.c_cflag | B19200;
  else if(vitesse==115200)
    serie.c_cflag = serie.c_cflag | B115200;
  else if(vitesse==38400)
    serie.c_cflag = serie.c_cflag | B38400;
  else if(vitesse==9600)
    serie.c_cflag = serie.c_cflag | B9600;
  else if(vitesse==4800)
    serie.c_cflag = serie.c_cflag | B4800;
  else if(vitesse==2400)
    serie.c_cflag = serie.c_cflag | B2400;
  else if(vitesse==57600)
    serie.c_cflag = serie.c_cflag | B57600;
  else if(vitesse==1200)
    serie.c_cflag = serie.c_cflag | B1200;
  else if(vitesse==600)
    serie.c_cflag = serie.c_cflag | B600;
  else if(vitesse==300)
    serie.c_cflag = serie.c_cflag | B300;
  else
    serie.c_cflag = serie.c_cflag | B9600;

  if (ioctl(fd,TCSETA,&serie) == NOK)
  {
    printf("port = %d\n",port);
    printf("ioctl(fd,TCSETA,&serie) = %d\n",ioctl(fd,TCSETA,&serie));
    fprintf(stderr,"Erreur lors de la configuration du port serie.\n");
    status = NOK;
    return NOK;
  }
  idport=fd;
  status = OPEN;
  return 0;
}

int serial::configport()
{
  /* configuration du port */

  if (ioctl(fd,TCGETA,&serie) == NOK)
  {
    fprintf(stderr,"Erreur lors de la configuration du port serie.\n");
    status = NOK;
    return NOK;
  }

  serie.c_iflag = 0;

  if (ioctl(fd,TCSETA,&serie) == NOK)
  {
    fprintf(stderr,"Erreur lors de la configuration du port serie.\n");
    status = NOK;
    return (NOK);
  }
  idport=fd;
  return OK;
}

/*==========================================================================
    fonction: 	int fermerport(int fd)

    description:	Ferme le port serie.
            Retourne OK ou ERREUR.

    revisions: 	Martin Riopel 22/08/2002
============================================================================*/
int serial::fermerport()
{
  if (close(fd)== NOK)
  {
    fprintf(stderr,"Erreur lors de la fermeture du port serie.\n");
    return NOK;
  }
  status = CLOSE;
  return OK;
}

/*==========================================================================
    fonction: 	int ecrireport(int fd, char *str)

    description:	Envoie une chaine sur le port serie.
            Retourne OK ou ERREUR.

    revisions: 	Martin Riopel 22/08/2002
============================================================================*/
int serial::ecrireport(std::string str)
{
  int ncar;
  ncar=strlen(str.c_str());
  if(write(fd,str.c_str(),ncar)!=ncar)
  {
    fprintf(stderr,"Erreur lors de l'ecriture sur le port serie.\n");
    return NOK;
  }
  delay(20);//we add a delay because we removed the termios VTIME flag
  return OK;
}

/*==========================================================================
    fonction: 	int lireport(fd, char *str, int ncar)

    description:	lit au plus ncar sur le port serie, copie les
            caracteres lus dans la chaine str, et ajoute '\0'.

            Retourne le nombre de caracteres lus.
            Retourne 0 si aucun caractere lu.
            Retourne ERREUR si une erreur a eu lieu.

    revisions: 	Martin Riopel 22/08/2002
============================================================================*/
int serial::lireport (std::string *str,int ncar)

{
  int i,n;
  *str = "";
  char *tmpstr = new char[ncar];
  memset(tmpstr,0,ncar);

  for(i=0;i<ncar;i++)
  {
        n = read(fd,tmpstr+i,1);
        //printf("n: %d\n",n);
        if(n==0)
        {
          tmpstr[i]='\0';
          *str = conversion(tmpstr);
          delete [] tmpstr;
          return i;
        }
  }
  tmpstr[i]='\0';
  *str = conversion(tmpstr);
  delete [] tmpstr;
  return i;
}


/*==========================================================================
    fonction: 	int lirec(int fd, char c)

    description:	Lit sur le port serie jusqu'au caractere c.
            Retourne OK ou ERREUR.

    revisions: 	Martin Riopel 22/08/2002
============================================================================*/
void serial::delay(int ms)
/*!
  *\brief Delay in ms
  * Create a delay. ms is delay time in millisecond
  */
{
    clock_t t0 = clock();
    while (clock()<t0+ms*1000)
        ;
}

int serial::lirec (std::string *str,char c)
/* Lit sur le port serie jusqu'au caractere c.
 * Retourne le nombre de charactère lu.
 */
{
	int i=0,n=0;
	*str = "";
	char *tmpstr = new char[MAX_CHAR];
	memset(tmpstr,0,MAX_CHAR);	
	
	for(i=0;i<MAX_CHAR;i++)
	{
		n = read(fd,tmpstr+i,1);

		if(tmpstr[i]==c)//should strip the \r
		{
		  break;
		}
		else if (n==0) {
			break;
		}
	}
	tmpstr[i]='\0';
	*str = conversion(tmpstr);
	delete [] tmpstr;
	return i;
}

std::string serial::conversion(std::string ss)
{
 std::string buff="";
  for (auto &c:ss)
  {
    buff+=c & 0x7F;

  }
  if (buff.length()>0){
  if (buff[buff.length()-1]=='\n')
  {
      buff[buff.length()-1] = '\0';
  }
  }
  return buff;
}


