#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Feb  1 15:26:16 2023

@author: espressjo
"""
from astropy.stats import sigma_clipped_stats
import numpy as np
from astropy.io import fits
from photutils.detection import DAOStarFinder
from astropy.modeling.models import Gaussian2D
from os import popen
from IDS import ids
from os.path import isdir,join,isfile,basename
from os import listdir
from astropy.wcs import WCS
from astropy.stats import sigma_clipped_stats as sc
from scipy.ndimage import center_of_mass,label
from Hlog import LHiCIBaS
from moteurs import moteurs
from shm_HiCIBaS import devices,telescope

tcs = telescope()
dev = devices()

log = LHiCIBaS(__file__)

class astrom:
    def __init__(self):
        self.RA = 0.0#hour
        self.DEC = 0.0#degree
        self.valid = False
        self.wcs = WCS()

class guideCam(ids):
    """
    Description
    ===========
        This class can be use to create a string representation of the actual FOV
        of the guiding camera, or used to re-build this said FOV.
    
    flat
    ----
        You can set a flat to be applied like this; (1) guideCam.flat = "/home/flat.fits", 
        or (2) guideCam.flat = fits.getdata('flat.fits')
    Example
    -------
        Use the (with as) statement pls. 
    """
    def __init__(self,serial =4103216958,name ='CoarseGuideCam',hw_simul=False,shape=(1216,1936)):
        self.shape = shape
        self.cfg_gain = 1
        self.name = name
        self.serial = "%d"%serial
        self.sim_f = "/opt/HiCIBaS/etc/dummy.fits"
        
        self._sim_im =np.zeros(self.shape)
        if not hw_simul:
            ids.__init__(self,self.serial)#init the camera
        else:
            self._sim_im = fits.getdata(self.sim_f)
        
        self.tmp_astrom = "/var/tmp/.astrom"
        self.last_im = np.ones(self.shape)#sets which array as been "touched" last. For ds9 use only
        self._simul = hw_simul
        self._apply_corr = False
        self._flat_im =np.ones(self.shape)
        
        


    def __enter__(self):
        if self._simul:
            self._sim_im = fits.getdata(self.sim_f)
            return self
        self.connect()#connect to hardware
        self.set_adc(12)        
        self.set_memory()
        self.cmap = np.zeros((2,self.height.value,self.width.value))
        for x in range(self.width.value):
            for y in range(self.height.value):
                self.cmap[0,y,x] = x
                self.cmap[1,y,x] = y
            
        return self
    def __exit__(self,a,b,c):
        if self._simul:
            return
        self.disconnect()#disconnect from hardware
        
    @property
    def simulation(self):
        return self._simul
    @simulation.setter
    def simulation(self,b):
        self._simul = b 
        if b:
            self._sim_im = fits.getdata(self.sim_f)
        
    @property
    def flat(self):
        return self._flat_im
    @flat.setter
    def flat(self,flat):
        if isinstance(flat,str):
            flat = fits.getdata(flat)
        elif isinstance(flat, np.ndarray):
            flat = flat
        else:
            return 
        self._flat_im = flat 
        self.self._apply_corr = True 
        return 
    @flat.deleter
    def flat(self):
        self._apply_corr = False
        self._flat_im =np.ones(self.shape) 
        return
    def set_ROI(self,x,y,width,height):
        """
        Set a region of interest. 

        Parameters
        ----------
        x : INT
            Center of the ROI.  
        y : TYPE
            Center of the ROI.
        width : TYPE
            Width of the ROI. Must be a multiple of 8
        height : TYPE
            height of the ROI. Must be a multiple of 2

        Returns
        -------
        None.

        """
        #[TODO] Test if ROI behave the way we want: AOI use x,y as left corner, ROI should use at the center
        if width%8!=0:
            multi = int(width/8.)
            width = int(multi*8)
        if height%2!=0:
            multi = int(height/2.)
            height = int(multi*2.)
        h_width = int(width/2.)
        h_height = int(height/2.)
        x = x-h_width
        y = y-h_height
        cmap = np.zeros((2,height,width))
        for i in range(width):
            for ii in range(height):
                cmap[0,ii,i] = x+i
                cmap[1,ii,i] = y+ii
        self.cmap = cmap
        
        self.set_AOI(x, y, width, height)
        self.set_memory()
    def get_cm(self,c_map='absolute'):
        """
        Utility function to return the center of mass of an image
        using the current setup. We assum there is only 1 object in the FOV.
        
        c_map
        -----
        c_map can be set to relative or absolute. When the coordinate map
        is set to absolute, the x and y position of the CM relative to the 
        full sensor size will be returned. Otherwise the position within
        the AOI (ROI) will be returned. Default is absolute.
        
        Performance
        -----------
        A AOI of approximately 100px X 100px will take ~ 3 ms to return the 
        CM.

        Returns
        -------
        _x : float
            X position of the CM. Will return a NAN if unable to compute the CM.
        _y : float
            Y position of the CM. Will return a NAN if unable to compute the CM.

        """
        im  = self.get_data()
        _,md,std = sc(im)
        lbl = np.copy(im)
        #create a mask to weedout every none-detection.
        lbl[lbl<(md+5*std)]=0
        lbl[lbl>=(md+5*std)]=1
        _y,_x = center_of_mass(im,lbl,1)
        if 'absolute' in c_map:
            if np.isnan(_x) or np.isnan(_y):
                return np.nan,np.nan 
            __x = _x%1
            __y = _y%1
            _x,_y = self.cmap[:,int(_x),int(_y)] 
            return _x+__x,_y+__y
        else:
            return _x,_y
    def _extract_info(self,info:str):
        """
        Used to convert from serialized image info to uint16 values

        Parameters
        ----------
        info : str
            Should have the form --->  Mean;Std;h;w;peak1-x1-y1:peak2-x2-y2:...:peakn-xn-yn

        Returns
        -------
        uint16 arrays
            mean,std, height, width, peaks (arrays),x (arrays),y (arrays)

        """
        mean,std,h,w,pos = info.split(";")
        s = [p.split('-') for p in pos.split(":")]
        if (len(s)>1):
            peak,x,y = zip(*s)
        else:
            peak = [];
            x = [];
            y = [];
        return np.asarray(mean,dtype=np.uint16),np.asarray(std,dtype=np.uint16),np.asarray(h,dtype=np.uint16),np.asarray(w,dtype=np.uint16),np.asarray(peak,dtype=np.uint16),np.asarray(x,dtype=np.uint16),np.asarray(y,dtype=np.uint16)
   
    def fake(self,info,noise=True):
        '''
        From the output of extract_stars you can recreate an image from only the centroid and stats
        value.

        Parameters
        ----------
        info : STR
            See extract_stars for more information.
        noise : BOOL, optional
            Generate background noise based on the real image std. dev. The default is True.

        Returns
        -------
        FOV : 2d numpy array
            Fake FOV.

        '''
        mean,std,h,w,peak,x,y = self._extract_info(info)
        if noise:
            rng = np.random.normal(0,std,(h,w))+mean
        else:
            rng = np.zeros((h,w))
        _y, _x = np.mgrid[0:h, 0:w]
        
        for i in range(len(x)):
            rng+=Gaussian2D(peak[i], x[i], y[i], 1.8, 1.8)(_x, _y)
        self.last_im = rng
        return rng
    
    def get_current_fov(self,max_stars=-1):
        """
        Will return a string which represent the current FOV as best as possible.
        Check extract_stars for the format of the output

        Parameters
        ----------
        max_stars : INT, optional
            maximum of stars to return.. The default is -1.

        Returns
        -------
        STR
            Mean;Std;h;w;peak1-x1-y1:peak2-x2-y2:...:peakn-xn-yn

        """
        if self._simul:
            im = self._sim_im    
        else:
            im = self.get_data()
        if self._apply_corr:
            im/=self.flat
        self.last_im = im
        return self.extract_stars(im,max_stars)
    def extract_stars(self,f,max_stars=-1):
        """
        Extract as  much information about a fits file as possible.
        Parameters
        ----------
        f : STR OR ndarray
            full path to file.
        max_stars : int
            maximum of stars to return. Default is -1 (extract all the stars)
    
        Returns
        -------
        STR
            Mean;Std;h;w;peak1-x1-y1:peak2-x2-y2:...:peakn-xn-yn
    
        """
        if isinstance(f,str):
            data = fits.getdata(f)
        elif isinstance(f,np.ndarray):
            data = f
        else:
            return f"{0};{0};{1216};{1936};"  
        h,w = data.shape
        mean, median, std = sigma_clipped_stats(data, sigma=3.0)  
        
        daofind = DAOStarFinder(fwhm=3.0, threshold=5.*std)  
        sources = daofind(data - median)  
        sources.sort('peak',reverse=True)
        if len(sources)>1:
            peak = sources['peak']
            x = sources['xcentroid']
            y = sources['ycentroid']
            pos = ""
            if max_stars!=-1 and len(x)>max_stars:
                stars = max_stars
            else:
                stars = len(x)
            for i in range(stars):
                pos+="%d-%d-%d:"%(np.uint16(peak[i]),np.uint16(x[i]),np.uint16(y[i]))
            
            pos = pos[:-1]
            info = f"{np.uint16(mean)};{np.uint16(std)};{h};{w};{pos}"
            return info
        else:
            return f"{np.uint16(mean)};{np.uint16(std)};{h};{w};"   
     
    def save(self,fname,overwrite=True):
        data = self.get_data() 
        fits.PrimaryHDU(data=data).writeto(fname,overwrite=overwrite)
    @property 
    def ds9(self):
        data = self.get_data()
        fits.PrimaryHDU(data=data).writeto("/var/tmp/tmp.ds9.fits",overwrite=True)
        popen("ds9 -zscale -zoom to fit /var/tmp/tmp.ds9.fits").read()

class coarseCam(guideCam):
    def __init__(self,hwsimul=False):
        guideCam.__init__(self,serial =4103216958,name ='CoarseGuideCam',hw_simul=False,shape=(1216,1936))
    def __enter__(self):
        dev.cam1 = True
        super().__enter__()
        return self
    def __exit__(self,a,b,c):
        dev.cam1 = False
        super().__exit__(a,b,c)
    def astrometry(self):
        """
        Will capture an image and try to perform the astrometry.

        Returns
        -------
        A : astrom struct
            Return the astrom structure. Check the valid member to see
            if the astrometry was performed successfully. RA and DEC will
            show the center of the FOV.

        """
        fname = "/var/tmp/process.fits"
        if self.simulation:
            popen(f"cp {self.sim_f} {fname}").read()
        else:
            self.get_fits(fname)
        #make sure tmp folder exist
        if not isdir(self.tmp_astrom):
            popen(f"mkdir -p {self.tmp_astrom}").read()
        #make sure the astrom tmp folder is empty
        ls = [join(self.tmp_astrom,f) for f in listdir(self.tmp_astrom)]
        for f in ls:
            popen(f"rm -f {f}").read()
        popen(f"solve-field {fname} -D {self.tmp_astrom}").read()
        nfile = join(self.tmp_astrom,basename(fname).replace(".fits",".new"))
        if not isfile(nfile):
            A = astrom()
            return A 
        wcs = WCS(nfile)
        data = fits.getdata(nfile)
        y,x = data.shape
        c = wcs.pixel_to_world(int(x/2.),int(y/2.))
        A = astrom()
        A.RA = c.ra.hour
        A.DEC = c.dec.degree
        tcs.RA = A.RA 
        tcs.DEC = A.DEC
        A.valid = True
        A.wcs = wcs
        log.info(f"RA: {A.RA}, DEC: {A.DEC}")
        return A 
"""
class position:
    def __init__(self):
        self._ra = 0.0
        self._dec = 0.0
        self._x = 0
        self._y = 0
        self._nutec = 0 
        self._rm8 = 0
    def __sub__(self,a:position,b:position):
        p = copy(a)
        p.x-=b.x
        p.y-=b.y
    @property
    def nutec(self):
        return self._nutec
    @nutec.setter
    def nutec(self,nutec):
        self._nutec = nutec        
    @property
    def rm8(self):
        return self._rm8
    @rm8.setter
    def rm8(self,rm8):
        self._rm8 = rm8   
    @property
    def x(self):
        return self._x
    @x.setter
    def x(self,x):
        self._x = x        
    @property
    def y(self):
        return self._y
    @y.setter
    def y(self,y):
        self._y = y

    @property    
    def dec(self):
        return self._dec
    @dec.setter
    def dec(self,dec):
        self._dec = dec
    @property
    def ra(self):
        return self._ra
    @ra.setter
    def ra(self,ra):
        self._ra = ra 
"""        
class fineCam(guideCam):
    def __init__(self,hwsimul=False):
        guideCam.__init__(self,serial =4103218786,name ='fineGuideCam',hw_simul=False,shape=(1216,1936))
    def __enter__(self):
        dev.cam2 = True
        super().__enter__()
        return self
    def __exit__(self,a,b,c):
        super().__exit__(a,b,c)
        dev.cam2 = False
    def move_cm(self,p_target,tolerence=10,iters=5):
        cm = self.get_cm()
        y1,x1 = cm
        x2,y2 = p_target
        y = y2-y1
        x = x2-x1 
        #nutec
        steps_nutec = x /1.202
        steps_rm8 = y/1.0021
        from time import sleep
        with moteurs() as m:
            m.move(steps_rm8, steps_nutec,wait=True)
            sleep(0.3)
            print(self.get_cm())
        
if '__main__' in __name__:
    with guideCam() as GC:
        GC.simulation = True
        info = GC.get_current_fov()
        GC.ds9
        im = GC.fake(info)
        GC.ds9