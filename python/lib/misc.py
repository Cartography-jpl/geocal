import os
import errno
from geocal_swig import *
import cPickle

# This contains miscellenous routines that don't really belong anywhere else.

def makedirs_p(dir):
    '''This is a version of makedirs that acts like mkdir -p, not complaining
    if directory already exists'''

    try:
        os.makedirs(dir)
    except OSError as exc:
        if exc.errno == errno.EEXIST: pass
        else: raise

def cib01_data():
    '''This return a raster image that can be used to read the CIB
    1 meter data.'''
    return VicarMultiFile(os.environ["CIB1_ROOT"] + "/cib01_db.int",
                          os.environ["CIB1_ROOT"], ".img")

def _write_cib01_mapinfo():
    '''Write out cib01 map info for when we don't have CIB 1 data available'''
    with open("cib01_mapinfo.dat", "w") as f:
        cPickle.dump(cib01_data().map_info, f, cPickle.HIGHEST_PROTOCOL)


def cib01_mapinfo():
    '''This gets the CIB 1 meter MapInfo. This is useful when we don't
    have the CIB 1 data available, but need a reasonable 1 meter
    MapInfo'''
    with open(os.path.dirname(__file__) + "/cib01_mapinfo.dat") as f:
        return cPickle.load(f)


