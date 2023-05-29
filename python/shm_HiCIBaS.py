#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
example code to read/write in shared memory.

Created on Tue Nov 15 14:35:20 2022

@author: Jonathan St-Antoine
"""

import sysv_ipc
import struct
def set_bit(number,pos:int):
    _p = 2**pos
    return number | _p
def unset_bit(number,pos:int):
    _p = ~(2**pos)
    return number & _p
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
        self.shm = sysv_ipc.SharedMemory(shared_memory_address,sysv_ipc.IPC_CREAT, mode = 420,size = sysv_ipc.PAGE_SIZE)
    def read_bit(self,bit_offset,mem_offset,length_in_bytes=1):
        something = self.shm.read(length_in_bytes,offset=mem_offset)
        number = int.from_bytes(something,'little')
        _b = 2**bit_offset
        if (_b & number) !=0:
            return 1
        else:
            return 0
    def write_bit_1(self,bit_offset,mem_offset,length_in_bytes=1):
        something = self.shm.read(length_in_bytes,offset=mem_offset)
        
        if length_in_bytes==1:
            F = 'B'
        elif length_in_bytes==2:
            F = 'H'
        elif length_in_bytes==4:
            F = 'I'
        else:
            from sys import exit
            exit(0)
        number = int.from_bytes(something,'little')    
        number = set_bit(number, bit_offset)        
        self.shm.write(struct.pack(F,number),offset=mem_offset)
    def write_bit_0(self,bit_offset,mem_offset,length_in_bytes=1):
        something = self.shm.read(length_in_bytes,offset=mem_offset)
        
        if length_in_bytes==1:
            F = 'B'
        elif length_in_bytes==2:
            F = 'H'
        elif length_in_bytes==4:
            F = 'I'
        else:
            from sys import exit
            exit(0)
        number = int.from_bytes(something,'little')    
        number = unset_bit(number, bit_offset)        
        self.shm.write(struct.pack(F,number),offset=mem_offset)
        
class temperature(hicibas_shm):
    def __init__(self,shared_memory_address=1):
        hicibas_shm.__init__(self,shared_memory_address)

    @property
    def T1(self):
        _b = self.shm.read(1,offset=34)
        t = struct.unpack('b',_b)
        if len(t)<1:
            return 0
        return t[0] 
    @T1.setter
    def T1(self,temp:int):
        t = struct.pack('b',temp)
        self.shm.write(t,offset=34)
        return 
    @property
    def T2(self):
        _b = self.shm.read(1,offset=35)
        t = struct.unpack('b',_b)
        if len(t)<1:
            return 0
        return t[0] 
    @T2.setter
    def T2(self,temp:int):
        t = struct.pack('b',temp)
        self.shm.write(t,offset=35)
        return
    @property
    def T3(self):
        _b = self.shm.read(1,offset=36)
        t = struct.unpack('b',_b)
        if len(t)<1:
            return 0
        return t[0] 
    @T3.setter
    def T3(self,temp:int):
        t = struct.pack('b',temp)
        self.shm.write(t,offset=36)
        return
    @property
    def T4(self):
        _b = self.shm.read(1,offset=37)
        t = struct.unpack('b',_b)
        if len(t)<1:
            return 0
        return t[0] 
    @T4.setter
    def T4(self,temp:int):
        t = struct.pack('b',temp)
        self.shm.write(t,offset=37)
        return
    @property
    def T5(self):
        _b = self.shm.read(1,offset=38)
        t = struct.unpack('b',_b)
        if len(t)<1:
            return 0
        return t[0] 
    @T5.setter
    def T5(self,temp:int):
        t = struct.pack('b',temp)
        self.shm.write(t,offset=38)
        return
    @property
    def T6(self):
        _b = self.shm.read(1,offset=39)
        t = struct.unpack('b',_b)
        if len(t)<1:
            return 0
        return t[0] 
    @T6.setter
    def T6(self,temp:int):
        t = struct.pack('b',temp)
        self.shm.write(t,offset=39)
        return   
    @property
    def H1(self):
        _b = self.shm.read(1,offset=40)
        t = struct.unpack('B',_b)
        if len(t)<1:
            return 0
        return t[0] 
    @H1.setter
    def H1(self,temp:int):
        t = struct.pack('B',temp)
        self.shm.write(t,offset=40)
        return 
    @property
    def H2(self):
        _b = self.shm.read(1,offset=41)
        t = struct.unpack('B',_b)
        if len(t)<1:
            return 0
        return t[0] 
    @H2.setter
    def H2(self,temp:int):
        t = struct.pack('B',temp)
        self.shm.write(t,offset=41)
        return
    @property
    def H3(self):
        _b = self.shm.read(1,offset=42)
        t = struct.unpack('B',_b)
        if len(t)<1:
            return 0
        return t[0] 
    @H3.setter
    def H3(self,temp:int):
        t = struct.pack('B',temp)
        self.shm.write(t,offset=42)
        return
    @property
    def H4(self):
        _b = self.shm.read(1,offset=43)
        t = struct.unpack('B',_b)
        if len(t)<1:
            return 0
        return t[0] 
    @H4.setter
    def H4(self,temp:int):
        t = struct.pack('B',temp)
        self.shm.write(t,offset=43)
        return
    @property
    def H5(self):
        _b = self.shm.read(1,offset=44)
        t = struct.unpack('B',_b)
        if len(t)<1:
            return 0
        return t[0] 
    @H5.setter
    def H5(self,temp:int):
        t = struct.pack('B',temp)
        self.shm.write(t,offset=44)
        return
    @property
    def H6(self):
        _b = self.shm.read(1,offset=45)
        t = struct.unpack('B',_b)
        if len(t)<1:
            return 0
        return t[0] 
    @H6.setter
    def H6(self,temp:int):
        t = struct.pack('B',temp)
        self.shm.write(t,offset=45)
        return
class devices(hicibas_shm):
    def __init__(self,shared_memory_address=1):
        hicibas_shm.__init__(self,shared_memory_address)
    @property
    def devices_encoded(self):
        _b = self.shm.read(1,offset=33)
        t = struct.unpack('B',_b)
        if len(t)<1:
            return 0
        return t[0]
    @devices_encoded.setter
    def devices_encoded(self,lim:int):
        t = struct.pack('B',lim)
        self.shm.write(t,offset=33)
        return
    @property
    def cam1(self):
        return self.read_bit(0,33)
    @cam1.setter 
    def cam1(self,b:bool):
        if b:
            self.write_bit_1(0, 33)
        else:
            self.write_bit_0(0, 33)
    @property
    def cam2(self):
        return self.read_bit(1,33)
    @cam2.setter 
    def cam2(self,b:bool):
        if b:
            self.write_bit_1(1, 33)
        else:
            self.write_bit_0(1, 33)
    @property
    def cam3(self):
        return self.read_bit(2,33)
    @cam3.setter 
    def cam3(self,b:bool):
        if b:
            self.write_bit_1(2, 33)
        else:
            self.write_bit_0(2, 33)
    @property
    def TTM(self):
        return self.read_bit(3,33)
    @TTM.setter 
    def TTM(self,b:bool):
        if b:
            self.write_bit_1(3, 33)
        else:
            self.write_bit_0(3, 33)
    @property
    def source(self):
        return self.read_bit(4,33)
    @source.setter 
    def source(self,b:bool):
        if b:
            self.write_bit_1(4, 33)
        else:
            self.write_bit_0(4, 33)
    @property
    def rm8(self):
        return self.read_bit(5,33)
    @rm8.setter 
    def rm8(self,b:bool):
        if b:
            self.write_bit_1(5, 33)
        else:
            self.write_bit_0(5, 33)
    @property
    def nutec(self):
        return self.read_bit(6,33)
    @nutec.setter 
    def nutec(self,b:bool):
        if b:
            self.write_bit_1(6, 33)
        else:
            self.write_bit_0(6, 33)
class limits(hicibas_shm):
    def __init__(self,shared_memory_address=1):
        hicibas_shm.__init__(self,shared_memory_address)
    @property
    def lim_encoded(self):
        _b = self.shm.read(1,offset=32)
        t = struct.unpack('B',_b)
        if len(t)<1:
            return 0
        return t[0]
    @lim_encoded.setter
    def lim_encoded(self,lim:int):
        t = struct.pack('B',lim)
        self.shm.write(t,offset=32)
        return
    @property
    def upper(self):
        return self.read_bit(0,32)
    @upper.setter 
    def upper(self,b:bool):
        if b:
            self.write_bit_1(0, 32)
        else:
            self.write_bit_0(0, 32)
    @property
    def lower(self):
        return self.read_bit(1,32)
    @lower.setter 
    def lower(self,b:bool):
        if b:
            self.write_bit_1(1, 32)
        else:
            self.write_bit_0(1, 32)
    @property
    def right(self):
        return self.read_bit(2,32)
    @right.setter 
    def right(self,b:bool):
        if b:
            self.write_bit_1(2, 32)
        else:
            self.write_bit_0(2, 32)
    @property
    def left(self):
        return self.read_bit(3,32)
    @left.setter 
    def left(self,b:bool):
        if b:
            self.write_bit_1(3, 32)
        else:
            self.write_bit_0(3, 32)
    @property
    def launch(self):
        return self.read_bit(4,32)
    @launch.setter 
    def launch(self,b:bool):
        if b:
            self.write_bit_1(4, 32)
        else:
            self.write_bit_0(4, 32)
    @property
    def az0(self):
        return self.read_bit(5,32)
    @az0.setter 
    def az0(self,b:bool):
        if b:
            self.write_bit_1(5, 32)
        else:
            self.write_bit_0(5, 32)
    @property
    def alt0(self):
        return self.read_bit(6,32)
    @alt0.setter 
    def alt0(self,b:bool):
        if b:
            self.write_bit_1(6, 32)
        else:
            self.write_bit_0(6, 32)
class telescope(hicibas_shm):
    def __init__(self,shared_memory_address=1):
        hicibas_shm.__init__(self,shared_memory_address)
    @property
    def nutec_moving(self):
        return self.read_bit(0,42)
    @nutec_moving.setter
    def nutec_moving(self,b:bool):
        if b:
            self.write_bit_1(0, 42)
        else:
            self.write_bit_0(0, 42)
    @property
    def rm8_moving(self):
        return self.read_bit(1,43)
    @rm8_moving.setter
    def rm8_moving(self,b:bool):
        if b:
            self.write_bit_1(1, 43)
        else:
            self.write_bit_0(1, 43)
    @property
    def nutec_enabled(self):
        return self.read_bit(2,42)
    @nutec_enabled.setter
    def nutec_enabled(self,b:bool):
        if b:
            self.write_bit_1(2, 42)
        else:
            self.write_bit_0(2, 42)
    @property
    def rm8_enabled(self):
        return self.read_bit(3,42)
    @rm8_enabled.setter
    def rm8_enabled(self,b:bool):
        if b:
            self.write_bit_1(3, 42)
        else:
            self.write_bit_0(3, 42)
    @property
    def alt(self):
        _b = self.shm.read(4,offset=0)
        _f = struct.unpack('f',_b)
        if len(_f)<1:
            return 0
        return _f[0]
    @alt.setter
    def alt(self,alt:float):
        _b = struct.pack('f',alt)
        self.shm.write(_b,offset=0)
    @property
    def az(self):
        _b = self.shm.read(4,offset=4)
        _f = struct.unpack('f',_b)
        if len(_f)<1:
            return 0
        return _f[0]
    @az.setter
    def az(self,az:float):
        _b = struct.pack('f',az)
        self.shm.write(_b,offset=4)
    @property
    def RA(self):
        _b = self.shm.read(4,offset=8)
        _f = struct.unpack('f',_b)
        if len(_f)<1:
            return 0
        return _f[0]
    @RA.setter
    def RA(self,ra:float):
        _b = struct.pack('f',ra)
        self.shm.write(_b,offset=8)
    @property
    def DEC(self):
        _b = self.shm.read(4,offset=12)
        _f = struct.unpack('f',_b)
        if len(_f)<1:
            return 0
        return _f[0]
    @DEC.setter
    def DEC(self,dec:float):
        _b = struct.pack('f',dec)
        self.shm.write(_b,offset=12)
    @property
    def alt_encoder(self):
        _b = self.shm.read(4,offset=24)
        _f = struct.unpack('i',_b)
        if len(_f)<1:
            return 0
        return _f[0]
    @alt_encoder.setter
    def alt_encoder(self,dec):
        _b = struct.pack('i',int(dec))
        self.shm.write(_b,offset=24)
    @property
    def az_encoder(self):
        _b = self.shm.read(4,offset=28)
        _f = struct.unpack('i',_b)
        if len(_f)<1:
            return 0
        return _f[0]
    @az_encoder.setter
    def az_encoder(self,dec):
        _b = struct.pack('i',int(dec))
        self.shm.write(_b,offset=28)
        
# class h_python(hicibas_shm):
#     def __init__(self,shared_memory_address=1):
#         hicibas_shm.__init__(self,shared_memory_address)
#     @property
#     def r_script(self):
#         _b = self.shm.read(4,offset=16)
#         _f = struct.unpack('I',_b)
#         if len(_f)<1:
#             return 0
#         return _f[0]
#     @r_script.setter
#     def r_script(self,encoded:int):
#         _b = struct.pack('I',encoded)
#         self.shm.write(_b,offset=16)
#     @property
#     def s_script(self):
#         _b = self.shm.read(4,offset=20)
#         _f = struct.unpack('I',_b)
#         if len(_f)<1:
#             return 0
#         return _f[0]
#     @s_script.setter
#     def s_script(self,encoded:int):
#         _b = struct.pack('I',encoded)
#         print(_b)
#         self.shm.write(_b,offset=20)
        

if '__main__' in __name__:

    dev = devices()
    tel = telescope()
    temp = temperature()
    lim = limits()

