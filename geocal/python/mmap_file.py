# This is miscellaneous functions that should eventually get sucked into
# afids python or C++

from geocal import *
import numpy as np

def mmap_file(*args, **kwargs):
    '''Open a VICAR file as a memmap. This either opens for reading or
    creates a new Vicar file with the given map info and data type. 

    Usage:
      mmap_file(fname) - Open an existing file for reading. If the
         dtype is supplied that is used, otherwise we read the VICAR
         label and use either np.float64, np.float32, np.int8, np.int16,
         or np.int32. VICAR doesn't differentiate between signed and 
         unsigned integers, so if you want the unsigned version you need
         to specify that.
      mmap_file(fname, mi, dtype=value) - Create a new file for
         reading and writing. If dtype isn't supplied, we default to
         np.int8.

    The data type should be one of np.float64, np.float32, np.int8, np.int16,
    np.int32, np.uint8, np.uint16, np.uint32'''
    
    # Create a file
    if(len(args) ==2):
        fname, mi =args
        dtype = kwargs.get("dtype", np.int8)
        if dtype ==np.float64:
            t = VicarRasterImage(fname, mi, "DOUB")
        elif dtype ==np.float32:
            t = VicarRasterImage(fname, mi, "REAL")
        elif dtype ==np.int8 or dtype ==np.uint8:
            t = VicarRasterImage(fname, mi, "BYTE")
        elif dtype ==np.int16 or dtype ==np.uint16:
            t = VicarRasterImage(fname, mi, "HALF")
        elif dtype ==np.int32 or dtype ==np.uint32:
            t = VicarRasterImage(fname, mi, "FULL")
        else:
            raise ValueError("Unsupport data type")
        t = None                    # Force write to disk
    elif(len(args) != 1):
        raise ValueError("Wrong number of arguments given")
    fname = args[0]
    f = VicarFile(fname)
    lsize = f.label_int("LBLSIZE")[0]
    shp = (f.label_int("NL")[0], f.label_int("NS")[0])
    if(kwargs.get('dtype') == None):
        t = f.label_string("FORMAT")[0]
        if(t == "DOUB"):
            dtype = np.float64
        elif(t == "REAL"):
            dtype = np.float32
        elif(t == "FULL"):
            dtype = np.int32
        elif(t == "HALF"):
            dtype = np.int16
        elif(t == "BYTE"):
            dtype = np.int8
        else:
            raise ValueError("Unknown file format")
    else:
        dtype = kwargs.get('dtype')
    return np.memmap(fname, dtype = dtype, offset = lsize, shape=shp)

