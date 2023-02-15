#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
example code to read/write in shared memory.

Created on Tue Nov 15 14:35:20 2022

@author: Jonathan St-Antoine
"""

import sysv_ipc
import struct

class hicibas_shm():
    """
    hicibas_shm class
    =========
    Example of a telescope control system shared structure
    with c++/c code. 
    
    Members
    -------
    *cnt:           Number of char for message\n
    *message:       char array, max 1024 characters\n
    *complete:      something!!\n
    *dec:           Telescope DEC\n
    *ra:            Telescope RA\n
    
    Methods
    --------
    *read->read the structure in shared memory.\n
    *write_message->Write message to shared memory.\n
    *write_ra->Write RA in shared memory.\n
    *write_dec->Write DEC in shared memory.\n
    *write_complete->Write complete in shared memory.\n
    *detach->detach from shared memory.\n
    Example
    -------
    ..  code-block:: python
        :caption: Simple code to read tcs shared memory values.
        :linenos:
        
        tcs_shm = hicibas_shm(0x0001)
        print(tcs_shm) #will create the shared memory space if does not exist
        #can also use;
        tcs_shm.read()
        print(tcs_shm.message)
        
        #write a new value of DEC
        tcs_shm.write_dec(132.932)
        
        #detach from shared memory
        tcs_shm.detach()
        
    """
    
    
    
    def __init__(self,shared_memory_address=1):
        self.limits = 0
        self.devices = 0
        self.dec = 0.0
        self.ra = 0.0

        #mode 420->644 in octal, PAGE_SIZE==4096 you may want to fit the stucture sizeof to multiple of PAGE_SIZE
        self.shm = sysv_ipc.SharedMemory(shared_memory_address,sysv_ipc.IPC_CREAT, mode = 420,size = sysv_ipc.PAGE_SIZE)
    def set_lim_switch(self,encoded:int):
        """
        Write bit-encoded value into the share memory in the
        limit switch elements. ONLY USE FOR TROUBLESHOOTING
        PURPPOSES. This section should only be write by the 
        HiCIBaS main server.

        Parameters
        ----------
        encoded : int (BIT encoded)
            <0> upper, <1> lower , <2> right, <3> left, 
            <4> launch, <5> AZ-0, <6> ALT-0

        Returns
        -------
        None.

        """
        b = struct.pack('B',encoded)
        self.shm.write(b,offset=0)
    def set_alt(self,alt:float):
        """
        Write telescope RA in shared memory.

        Parameters
        ----------
        ra : float
            Right ascension.

        Returns
        -------
        None.

        """
        _alt_byte = struct.pack('d',alt)
        self.shm.write(_alt_byte,offset=8)

    def set_az(self,az:float):
        """
        Write telescope DEC in shared memory.

        Parameters
        ----------
        dec : float
            declination

        Returns
        -------
        None.

        """
        _az_byte = struct.pack('d',az)
        self.shm.write(_az_byte,offset=16)

    def set_alt_moving(self):
        buff = self.shm.read(1,offset=1)
        status = struct.unpack('B',buff)[0]
        if (status & 2 == 2):
            _byte = struct.pack('B',3)
        else:
            _byte = struct.pack('B',1)       
        self.shm.write(_byte,offset=1)
    def unset_alt_moving(self):
        buff = self.shm.read(1,offset=1)
        status = struct.unpack('B',buff)[0]
        if (status & 2 == 2):
            _byte = struct.pack('B',2)
        else:
            _byte = struct.pack('B',0)       
        self.shm.write(_byte,offset=1)
    def unset_az_moving(self):
        
        buff = self.shm.read(1,offset=1)
        status = struct.unpack('B',buff)[0]
        if (status & 1 == 1):
            _byte = struct.pack('B',1)
        else:
            _byte = struct.pack('B',0)       
        self.shm.write(_byte,offset=1)
    def set_az_moving(self):
        buff = self.shm.read(1,offset=1)
        status = struct.unpack('B',buff)[0]
        if (status & 1 == 1):
            _byte = struct.pack('B',3)
        else:
            _byte = struct.pack('B',2)       
        self.shm.write(_byte,offset=1)

    def detach(self):
        """
        Detach from shared memory. If the shared

        Returns
        -------
        None.

        """
        self.shm.detach()

    def read(self):
        buff = self.shm.read()
        #self.cnt = int.from_bytes(buff[0:1],"little",signed=True)
        self.limits = struct.unpack('B',buff[0:1]) #bit-encoded: upper<0>,lower<1>,right<2>,left<3>,launch<4>,az_zero<5>,alt_zero<6>     
        self.devices = struct.unpack('B',buff[1:2]) #<0> alt moving?, <1> az moving ?
        self.alt = struct.unpack('d', buff[8:16])[0]
        self.az = struct.unpack('d', buff[16:24])[0]    
        #self.complete = int.from_bytes(buff[19:20],"little",signed=True)
        #self.message = buff[20:1044].decode()
    def __str__(self):
        self.read()
        txt=""
        txt+="cnt: %d\n"%self.cnt 
        txt+="complete: %d\n"%self.complete 
        txt+="message (short): %s\n"%self.message[:10] 
        txt+="ra: %f\n"%self.ra 
        txt+="dec: %f\n"%self.dec 
        
        return txt 

if '__main__' in __name__:

    shm = hicibas_shm()
    from time import sleep
    i=0
    while(1):
        sleep(1)
        shm.write_dec(11.1+i)
        i+=1
