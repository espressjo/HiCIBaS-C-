#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Feb 15 12:32:08 2023

@author: espressjo
"""
from datetime import datetime 
from os.path import basename,join,isdir,dirname
from HiCIBaS_CONF import LOGPATH,LOGLEVEL
from sys import argv,stderr

fmt = "%Y-%m-%dT%H:%M:%S"
DEBUG = 0
INFO = 1
WARNING = 2
CRITICAL = 3

class LHiCIBaS:
    """
    Description
    -----------
    Simple logging implementation for HiCIBaS. We highly recommend to pass __file__
    to the class at initialization. E.g., log = LHiCIBaS(__file__). 
    
    Main Methods
    ------------
        *info -> log general information\n
        *warnings -> log warnings\n
        *debug -> log debug information\n
        *critical -> log critical information\n
        *level -> Set the log level DEBUG, INFO,WARNING and CRITICAL\n
    NOTE:
    -----
        [1] E.g., if level is set to WARNING, we'll log WARNING and higher (CRITICAL)
        and so on.
        [2] You can also overide the log level via command line option --log=2.
        [3] default loglevel is defined in HiCIBaS_CONF
    """
    def __init__(self,name=""):
        self._create_lfile()
        
        if not isdir(dirname(self.fname)):
            print("Log path must exist.",file=stderr)
            exit(1)
        self._level = LOGLEVEL
        self.name = self.script(name) if name!="" else self.script(__file__)
    @property
    def level(self):
        self._level = INFO
    @level.setter 
    def level(self,level):
        """
        Set the log level. 
        
        Note
        ----
            command line argument have precedence over this function.

        Parameters
        ----------
        level : INT
            0 (DEBUG),1 (INFO),2 (WARNING),3 (CRITICAL).

        Returns
        -------
        None.

        """
        for arg in argv:
            if '--log' in arg:
                level = arg.replace("--log=","")
                try:
                    level = int(level)
                except:
                    level=INFO
        if level<0 or level>3:
            self._level = INFO
        else:
            self._level = level
    def tstamp(self):
        """
        create time stamp

        Returns
        -------
        TYPE
            DESCRIPTION.

        """
        return datetime.now().strftime(fmt)
    def script(self,inp:str):
        """
        Create the root name for log entries ([ROOT::INFO]). This is meant
        to be use with the __file__ attributes. If a string is given which 
        is not __file__, we'll use it as is.'

        Returns
        -------
        STR
            Root name.

        """
        if  ".py" in inp: 
            return basename(inp).replace(".py", "").upper()
        else:
            return inp.upper()
    def info(self,msg:str):
        """
        Log information

        Parameters
        ----------
        msg : str
            message to be logged.

        Returns
        -------
        None.

        """
        if INFO<self._level:
            return
        with open(self.fname,'a') as f:
            f.write(f"{self.tstamp()}\t[{self.name}::INFO]\t{msg}\n")
    def debug(self,msg:str):
        """
        Log debuge information

        Parameters
        ----------
        msg : str
            message to be logged.

        Returns
        -------
        None.

        """
        if DEBUG<self._level:
            return
        with open(self.fname,'a') as f:
            f.write(f"{self.tstamp()}\t[{self.name}::DEBUG]\t{msg}\n")
    def warning(self,msg:str):
        """
        Log warnings

        Parameters
        ----------
        msg : str
            message to be logged.

        Returns
        -------
        None.

        """
        if WARNING<self._level:
            return
        with open(self.fname,'a') as f:
            f.write(f"{self.tstamp()}\t[{self.name}::WARNING]\t{msg}\n")
    def critical(self,msg:str):
        """
        Log critical information

        Parameters
        ----------
        msg : str
            message to be logged.

        Returns
        -------
        None.

        """
        if CRITICAL<self._level:
            return
        with open(self.fname,'a') as f:
            f.write(f"{self.tstamp()}\t[{self.name}::CRITICAL]\t{msg}\n")
    def _create_lfile(self):
        self.fname = join(LOGPATH,datetime.now().strftime("%Y%m%d.py.txt"))
if '__main__' in __name__:
    log = LHiCIBaS()
    
    log.debug("debug test")
    log.level = DEBUG
    log.info("test!")
    log.debug("debug test [2]")