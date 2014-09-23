import os
import errno
import sys
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


def cib01_mapinfo(desired_resolution = None):
    '''This gets the CIB 1 meter MapInfo. This is useful when we don't
    have the CIB 1 data available, but need a reasonable 1 meter
    MapInfo. You can optionally supply a desired resolution, to scale the 
    mapinfo. '''
    with open(os.path.dirname(__file__) + "/cib01_mapinfo.dat") as f:
        res = cPickle.load(f)
    if(desired_resolution):
        resbase = res.resolution_meter
        res = res.scale(desired_resolution / resbase,
                        desired_resolution / resbase)
    return res


def pid_exists(pid):
    """Check whether pid exists in the current process table.
    UNIX only.
    """
    if pid < 0:
        return False
    if pid == 0:
        # According to "man 2 kill" PID 0 refers to every process
        # in the process group of the calling process.
        # On certain systems 0 is a valid PID but we have no way
        # to know that in a portable fashion.
        raise ValueError('invalid PID 0')
    try:
        os.kill(pid, 0)
    except OSError as err:
        if err.errno == errno.ESRCH:
            # ESRCH == No such process
            return False
        elif err.errno == errno.EPERM:
            # EPERM clearly means there's a process to deny access to
            return True
        else:
            # According to "man 2 kill" possible error values are
            # (EINVAL, EPERM, ESRCH)
            raise
    else:
        return True
