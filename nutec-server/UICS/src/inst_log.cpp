#include "inst_log.h"
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <cstdio>
#include <fstream>
#include <string.h>
#include <sys/stat.h>//mkdir
#include <libgen.h>
#include <string>

#include <algorithm>



Log::Log()
{
    Log::path = "/var/tmp/";//in case something goes wrong dump the log in /var/tmp
    Log::level = -1;//by default we do not log anything
}
void Log::setLevel(int level)
{
    Log::level = level;//by default we do not log anything
}
void Log::setPath(std::string path)
/*!
  * Set log path for class methods, if the path
  * does not exist it will create it.
  */
{
    if (path[path.size()-1]!='/')
    {
    	path+='/';
    }
    Log::path=path;
    if (Log::isFolder(Log::path)!=1)
    {
        Log::createFolder(Log::path);
    }
}
void Log::writeto(std::string text)
/*!
  * will log and entry in a log file (yyyymmdd.txt)
  * with timestamp yyyy-mm-ddThh:mm:ss and write
  * the file in class::path
  */
{

  if (Log::isFolder(Log::path)!=1){return;}
  std::string time = ts_now_gmt();
  time.erase(std::remove(time.begin(), time.end(), '\n'),time.end());

  int y=0,m=0,d=0;
  sscanf(time.c_str(),"%d-%d-%dT",&y,&m,&d);
  char t[13];
  memset(t,0,13);
  sprintf(t,"%d%2.2d%2.2d.txt",y,m,d);

  std::ofstream ofs;
  ofs.open (Log::path+t, std::ofstream::out | std::ofstream::app);
  ofs <<time+'\t'+text<<'\n';
  ofs.flush();
  ofs.close();
}
void Log::writeto(std::string text,int level)
/*!
  * will log and entry in a log file (yyyymmdd.txt)
  * with timestamp yyyy-mm-ddThh:mm:ss and write
  * the file in class::path
  * If the log level is <= Log::level, the text
  * will be logged.
  */
{

  if (level>Log::level)
  {
      return;
  }
  if (Log::isFolder(Log::path)!=1){return;}
  std::string time = ts_now_gmt();
  time.erase(std::remove(time.begin(), time.end(), '\n'),time.end());

  int y=0,m=0,d=0;
  sscanf(time.c_str(),"%d-%d-%dT",&y,&m,&d);
  char t[13];
  memset(t,0,13);
  sprintf(t,"%d%2.2d%2.2d.txt",y,m,d);

  std::ofstream ofs;
  ofs.open (Log::path+t, std::ofstream::out | std::ofstream::app);
  ofs <<time+'\t'+text<<'\n';
  ofs.flush();
  ofs.close();
}

void Log::writetoVerbose(std::string text)
/*!
  * will log and entry in a log file (yyyymmdd.txt)
  * with timestamp yyyy-mm-ddThh:mm:ss and write
  * the file in class::path
  */
{

  if (Log::isFolder(Log::path)!=1){return;}
  std::string time = ts_now_gmt();
  time.erase(std::remove(time.begin(), time.end(), '\n'),time.end());
  std::cout<<time+'\t'+text<<std::endl;
  int y=0,m=0,d=0;
  sscanf(time.c_str(),"%d-%d-%dT",&y,&m,&d);
  char t[13];
  memset(t,0,13);
  sprintf(t,"%d%2.2d%2.2d.txt",y,m,d);

  std::ofstream ofs;
  ofs.open (Log::path+t, std::ofstream::out | std::ofstream::app);
  ofs <<time+'\t'+text<<'\n';
  ofs.flush();
  ofs.close();
}

int Log::createFolder(std::string path){
  char *subpath, *fullpath;

  fullpath = strdup(path.c_str());
  subpath = dirname(fullpath);
  if (strlen(subpath) > 1)
      Log::createFolder(subpath);
  mkdir(path.c_str(),S_IRWXU | S_IRWXG | S_IRWXO);
  free(fullpath);
  return 0;
}


bool Log::isFolder(std::string path){
  return ( access( path.c_str(), F_OK ) != -1 );
}
bool Log::isFile(std::string path){

 return ( access( path.c_str(), F_OK ) != -1 );
}

