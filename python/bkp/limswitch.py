from lrio_labjack_wrapper import lrio_labjack
from multiprocessing import Process
from sys import stderr
from HiCIBaS_CONF import LOGPATH
from os.path import join
import traceback
from signal import signal, SIGTERM, SIGINT
from Hlog import LHiCIBaS
logging = LHiCIBaS(__file__)
from config_get import get_string,get_int

def eprint(error:str):
    print(error,file=stderr)

def read_ip_address(model="ANY",mode="ANY",serial="ANY"):
    from labjack import ljm
    import struct,socket
    handle = ljm.openS(model, mode, serial)
    info = ljm.getHandleInfo(handle)
    print("info: ",info)
    results = ljm.eReadAddress(handle, 49100, ljm.constants.UINT32)
    print("Uint32: ",int(results))
    ip = socket.inet_ntoa(struct.pack('>L',int(results)))
    ljm.close(handle)
    return ip
def write_ip_address(ip,model="ANY",mode="ANY",serial="ANY"):
    from labjack import ljm
    import struct,socket
    from time import sleep
    handle = ljm.openS(model, mode, serial)
    info = ljm.getHandleInfo(handle)
    print("info: ",info)
    ip_uint32 = socket.inet_aton(ip)
    ip_uint32 = struct.unpack("!L", ip_uint32)[0]
    print("uint32: ",int(ip_uint32))
    ljm.eWriteAddress(handle, 49150, ljm.constants.UINT32,int(ip_uint32))
    ljm.eWriteAddress(handle, 49160, ljm.constants.UINT16,0)
    ljm.eWriteAddress(handle, 49190, ljm.constants.UINT32,1)
    print("Uint32: ",ip_uint32)
    sleep(2)
    ljm.close(handle)
    return 0

class Labjack_lim():
    """
    Description
    ------------
        Class to interface the labjack. There is 3 types of connection; 1) USB, 
    2) Ethernet, 3) WiFi. We highly recommend using the Ethernet connection. 
    
    Note
    ----
        Multiple connection from different process can be established if the
        Ethernet connection is chosen. This might be highly usefull to read 
        the status of the lim. switch at all time.
    
    Methods
    -------
        *execute_with_limswitch -> Execute a function while checking the lim. switch\n
        *check_lim_switch -> To be use in loop to check if a lim. switch. is activated\n
        *read -> Read the state of all lim. switch\n
        *close -> Release the USB resource. \n
        *anySwitchActivated -> return boolean if any switch is active.\n
    
    Shared Memory
    -------------
        Some methods will write directly into the shared memory. Use the shm_HiCIBaS
        module to read the limit switch status
        ::: Warning :::
            Be very carefull, the lim.switch status are only updated when the read or 
            the check_lim_switch methods are called. If you want to "update" the lim.
            switch status into the shared memry, make sure to call read() 1st
    Arguments
    ---------
        You can give a protocole (USB/Ethernet) in argument. e.g., --protocol=USB
    Example
    --------
        The following describe how to define a function <test> which will be executed 
        while we check the lim. switch. If a lim. switch is triggered, the SIGTERM or
        SIGTERM will be sent to the function. Make sure the function you are executing 
        have good signals management in case the process is terminated or interupted 
        i.e., if a lim. switch is activated. Look at the move() methods in the moteurs.py 
        script for a good example. 
        
        .. code-block:: python
            :linenos:
            def test():
                #function which perform some telescope
                #movement
                with moteurs() as mot:
                    mot.move(0,12.1,wait=True)
                exit(0)
            with Labjack_lim() as lj: #Acquire the USB resources.                
                lj.execute_with_limswitch(test)
        
    """
    def __init__(self,connection="USB"):
        self.device = lrio_labjack()
        self.counter_lim_1 = 0
        self.counter_lim_2 = 0
        self.counter_lim_3 = 0
        self.counter_lim_4 = 0
        # Print device info

        #print(device.device_type)
        #print(device.connection_type)
        # Close device
        serial = get_string("/opt/HiCIBaS/config/network.conf","LJSERIAL",default="470015647")
        model = get_string("/opt/HiCIBaS/config/network.conf","LJMODEL",default="T7")
        mode = get_string("/opt/HiCIBaS/config/network.conf","LJMODE",default="Ethernet")
        # Connect specific device (device type, connection type and serial number)
        self.device = lrio_labjack(intitate_handle_arguments=(model, mode, serial))
    def _handle_interrupt(self,signal_received, frame):
        """
        Use to make sure we disconnect from hardware.

        Parameters
        ----------
        signal_received : TYPE
            DESCRIPTION.
        frame : TYPE
            DESCRIPTION.

        Returns
        -------
        None.

        """
        exit(1)
    def __enter__(self):
        '''
        Since connection to hardware is made, its better to use with statement i.e., 
        
        with labjack_lim() as lj:
            lj.read()
            
        Returns
        -------
        SELF
        '''
        signal(SIGINT, self._handle_interrupt)
        signal(SIGTERM, self._handle_interrupt)
        return self
    def __exit__(self,exc_type, exc_value, tb): 
        """
        Exit method. USE IT!

        Parameters
        ----------
        exc_type : TYPE
            DESCRIPTION.
        exc_value : TYPE
            DESCRIPTION.
        tb : TYPE
            DESCRIPTION.

        Returns
        -------
        None.

        """
        self.close()
        if exc_type is not None:
            traceback.print_exception(exc_type, exc_value, tb)
            logging.critical("Something is wrong in the closing sequence")
    def execute_with_limswitch(self,func,args=None,skip_az_zero=True,skip_alt_zero=True):
        """
        Description
        -----------
            Execute a function while monitoring the limit switch. Make sure
            the function you are executing have good signals management in 
            case the process is terminated or interupted i.e., if a lim.
            switch is activated. 
            Look at the move() methods in the moteurs.py script for a 
            good example.
        
        Example
        -------
            .. code-block:: python
                :linenos:
                def test():
                    #function which perform some telescope
                    #movement
                    with moteurs() as mot:
                        mot.move(0,12.1,wait=True)
                    exit(0)
                with Labjack_lim() as lj: #Acquire the USB resources.                
                    lj.execute_with_limswitch(test)
                
        
        Parameters
        ----------
        func : function
            Fucntion to be executed while we monitor the lim. switch.
        args: tuple
            tuple of arguments. If you only need 1 argument don't forget
            the comma in the tuple i.e.,args=(arg1,)
        skip_az_zero: BOOL default True
            Will also stop the process if set to False. It can be usefull to
            find the zero switch.
        skip_alt_zero: BOOL default True
            Will also stop the process if set to False. It can be usefull to
            find the zero switch.
        
        Returns
        -------
        c : STR Array
            An empty string array is returned if the function exit without touching a lim. switch.
            If a limit switch is triggered, this function will return which lim. switch
            has been triggered. [left, right, upper, lower,alt_zero,az_zero]
        p1: INT
            Process Exit code
        """
        #first we make sure no lim. switch are activated
        c = self.check_lim_switch()

        #now we start the process
        if args!=None:
            p1 = Process(target=func,args=args)
        else:
            p1 = Process(target=func)
        p1.start()
        c=''
        while(p1.is_alive()):
            c = self.check_lim_switch()
            #stop the process as soon as possible
            #check for the limit switch 1st
            already_term = False
            if any(['left' in c,'right' in c,'upper' in c,'lower' in c]):
                p1.terminate()
                p1.join()
                already_term = True
            if all([not skip_az_zero, 'az_zero' in c,not already_term]):
                p1.terminate()
                p1.join()
            if all([not skip_alt_zero, 'alt_zero' in c,not already_term]):
                p1.terminate()
                p1.join()
                
            if 'left' in c:
                logging.info('Telescope reached left limit switch')
                logging.info('Correction sequence initiated(-2 degree in x)')
                with open(join(LOGPATH,'which_lim.txt'), 'w') as f: #Write whcih limit switch was trigerred
                    f.write('left')
            elif 'right' in c:
                logging.info('Telescope reached right limit switch')
                logging.info('Correction sequence initiated(+2 degree in x)')
                with open(join(LOGPATH,'which_lim.txt'), 'w') as f: #Write whcih limit switch was trigerred
                    f.write('right')
            elif 'upper' in c:
                logging.info('Telescope reached upper limit switch')
                logging.info('Correction sequence initiated(+2 degree in y)')
                with open(join(LOGPATH,'which_lim.txt'), 'w') as f: #Write whcih limit switch was trigerred
                    f.write('upper')
            elif 'lower' in c:
                logging.info('Telescope reached lower limit switch')
                logging.info('Correction sequence initiated(-2 degree in y)')
                with open(join(LOGPATH,'which_lim.txt'), 'w') as f: #Write whcih limit switch was trigerred
                    f.write('lower')
            if 'az_zero' in c and not skip_az_zero:
                logging.info('Telescope reached az-zero switch')
            if 'alt_zero' in c and not skip_alt_zero:
                logging.info('Telescope reached alt-zero limit switch')
               
        return c,p1.exitcode
    def check_lim_switch(self):
        """
        You should use this method to read the lim. switch in a loop. If 
        a lim.switch is triggered, we will return which one in an array.

        Returns
        -------
        string array
            [upper, lower, right,left,az_zero,alt_zero] will be returned if a lim. switch is triggered
            Other whise an empty string array is returned.

        """
        lim_switch = [];
        lnch_lim,tmax6_lower_lim,yzero_lim,rm_right_lim,rm_left_lim,tmax6_upper_lim,xzero_lim = self.read()

        if yzero_lim == 0:
            lim_switch.append('alt_zero')        
        if rm_left_lim == 0:
            lim_switch.append('left')
        if rm_right_lim == 0:
            lim_switch.append('right')
        if tmax6_upper_lim == 0:
            lim_switch.append('upper')
        if tmax6_lower_lim == 0:
            lim_switch.append('lower')
        if yzero_lim == 0:
            lim_switch.append('alt_zero')
        if xzero_lim == 0:
            lim_switch.append("az_zero")
        return lim_switch
    def anySwitchActivated(self):
        """
        Check the status of all the switch. Usefull to quickly check
        if any switch is activated

        Returns
        -------
        hardware_switch : BOOL
            Hardware switch active (upper, lower,right,left).
        zero_switch : BOOL
            Zero switch active (azimuth, altitude).

        """
        all_active_lim = self.check_lim_switch()
        zero_switch = any(['az_zero' in all_active_lim,'alt_zero' in all_active_lim])
        hardware_switch = any(['left' in all_active_lim,'right' in all_active_lim,'upper' in all_active_lim,'lower' in all_active_lim])
        return hardware_switch,zero_switch
    def read(self):
        """
        Description
        -----------
            this method do 2 things, 1) read the labjack and update the "colored" element 
            and 2) It write into the shared memory the status of the limit switchs.

        Returns
        -------
        list
            [lnch_lim,tmax6_lower_lim,yzero_lim,rm_right_lim,rm_left_lim,tmax6_upper_lim,xzero_lim]

        """
        mat = self.device.read_analog_input("DIO6") # No more mat, but a normal switch could be placed there instead
        yellow = self.device.read_analog_input("DIO8")
        green = self.device.read_analog_input("DIO9")
        orange = self.device.read_analog_input("DIO10")
        red = self.device.read_analog_input("DIO11")
        brown = self.device.read_analog_input("DIO12")
        sep = self.device.read_analog_input("DIO4")#azimuth-0
        return [mat,yellow,green,orange,red,brown, sep]
    def __del__(self):
        """
        Destructor of labjack. Make sure we disconnect from 
        the usb resource.

        Returns
        -------
        None.

        """
        try:
            self.close()
        except Exception:
            logging.critical("Exception occured in __del__")
            pass
    
    def close(self):
        """
        Release the USB resource. Once this call is made, you need
        to call the class again before reading the device.
        
        Warning
        -------
        Always use the __enter__ and __exit__ methods whenever you
        can. If you don't, make sure you call this method before 
        exiting the script.

        Returns
        -------
        None.

        """
        try:
            self.device.disconnect_handle()
        except BaseException as e: #Catches variuous errors and what raised Keyboard Interrupt to initate the appropriate correction sequence
            if e.errorCode == 1224:
                pass
            else:
                eprint("Unable to disconnect from labjack")

if __name__ == "__main__": #To manually check if the limit switches are activated or not
    from os import system
    from time import sleep
    from sys import argv
    protocol = "Ethernet"
    protocol = get_string("/opt/HiCIBaS/config/network.conf","LJMODE",default=protocol)
    for arg in argv:
        if '--protocol' in arg:
            protocol = arg.replace("--protocol=","")
    with Labjack_lim(connection=protocol) as lbj:
        while(1):
            system("clear")
            status = lbj.read()#lnch_lim,tmax6_lower_lim,yzero_lim,rm_right_lim,rm_left_lim,tmax6_upper_lim,xzero_lim
            status = [int(c) for c in status]
            print(f"launch:    {status[0]}")
            print(f"Nutec low: {status[1]}")
            print(f"Nutec hi:  {status[5]}")
            print(f"RM8 right: {status[3]}")
            print(f"RM8 left:  {status[4]}")
            print(f"x-zero:    {status[6]}")
            print(f"y-zero:    {status[2]}")

            
            sleep(0.2)
    