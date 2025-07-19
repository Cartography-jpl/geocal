from geocal_swig import *
import numpy as np

def _new_memmap(version, args, kwargs):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    cls = memmap_wrap
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    return memmap_wrap(*args, **kwargs)

class memmap_wrap(np.memmap):
    '''This adds a wrapper for being able to pickle this object'''
    def __init__(self, *args, **kwargs):
        # Note we don't need to call np.memmap __init__, because there
        # isn't one. Instead the class has __new__ that sets things up
        # We don't override __new__, so nothing special needs to be done
        # by us to ensure the np.memmap base class is created.
        self.__args_save = args
        self.__kwargs_save = kwargs
        
    @classmethod
    def pickle_format_version(cls):
        return 1

    def __reduce__(self):
        return _new_memmap, (1, self.__args_save, self.__kwargs_save)
    
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
    np.int32, np.uint8, np.uint16, np.uint32

    Note that we actually wrap the np.memmap object up so that we can 
    efficiently pickle it (the default is to pickle the underlying array,
    which can be huge)

    In addition to the dtype, you can pass in the mode
         'r'   Open existing file for reading only.
         'r+'  Open existing file for reading and writing.
         'c'   Copy-on-write: assignments affect data in memory, but
               changes are not saved to disk. The file on disk is read-only.

    Default mode if 'r' for existing files, 'r+' for files that we create
    (i.e., passing in the mapinfo).

    A "nodata" value can be passed in, if supplied we write that to the
    VicarFile.
    '''
    
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
        if(kwargs.get('nodata') is not None):
            t["NODATA"] = kwargs.get('nodata')
        t = None                    # Force write to disk
        mode = kwargs.get('mode', 'r+')
    else:
        mode = kwargs.get('mode', 'r')
        if(len(args) != 1):
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
    return memmap_wrap(fname, dtype = dtype, offset = lsize, shape=shp, 
                       mode=mode)

__all__ = ["mmap_file"]

